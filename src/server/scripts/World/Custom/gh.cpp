#include "ScriptPCH.h"
#include <cstring>
#include "GuildMgr.h"
#include "ObjectMgr.h"
#include "Maps/MapMgr.h"
#include "World.h"
#include "IWorld.h"

const uint32 GUILD_TOKEN = 43231;
bool requiresGUpdate = true;
std::vector<uint64> GuildHouses;
char string[200];

enum Objects
{
	OBJECT_TABLE        = 180698,
	OBJECT_TORCH        = 187988,
	OBJECT_MAILBOX      = 142094,
	OBJECT_CRATE        = 183992,
	OBJECT_A_FLAG       = 192686,
	OBJECT_H_FLAG       = 192688,
	OBJECT_CHAIR        = 10201,
	OBJECT_FIREPIT      = 1967,
	OBJECT_GUILDVAULT   = 187390,
	OBJECT_BARBER_CHAIR = 191817,
	OBJECT_CRANBERRY_BUSH = 195199,
};

void SpawnObject(Player * pPlayer, uint32 objectId)
{
	const GameObjectTemplate* objectInfo = sObjectMgr->GetGameObjectTemplate(objectId);
	
	float x = float(pPlayer->GetPositionX());
	float y = float(pPlayer->GetPositionY());
	float z = float(pPlayer->GetPositionZ());
	float o = float(pPlayer->GetOrientation());
	Map* map = pPlayer->GetMap();

	GameObject* object = new GameObject;
    ObjectGuid::LowType guidLow = pPlayer->GetMap()->GenerateLowGuid<HighGuid::GameObject>();
	
	if (!object->Create(guidLow, objectInfo->entry, map, pPlayer->GetPhaseMaskForSpawn(), x, y, z, o, G3D::Quat(), 0, GO_STATE_READY))
	{
		delete object;
		return;
	}
	
	object->SetRespawnTime(1);
	object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), pPlayer->GetPhaseMaskForSpawn());
	
	if (!object->LoadGameObjectFromDB(guidLow, map))
	{
		delete object;
		return;
	}
	
	sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGameObjectData(guidLow));
	WorldDatabase.Execute("INSERT INTO guildhouse_objects VALUES('%u', '%u')", pPlayer->GetGuildId(), guidLow);
}

uint32 getGuildAreaId(Player * pPlayer)
{
	QueryResult Result = WorldDatabase.Query("SELECT area FROM guildhouses WHERE guildId='%u'", pPlayer->GetGuildId());
	if(Result)
	{
		Field * pField = Result->Fetch();
		return pField[0].Get<uint32>();
	}

	return -1;

}

bool isInGuildArea(Player * pPlayer)
{
	for(int i = 0; i < 21; i++)
	{
		if(pPlayer->GetAreaId() == getGuildAreaId(pPlayer))
			return true;
	}
	return false;
}

bool isGuildHouseOwner(Player * pPlayer)
{
	if(GuildHouses.empty() || requiresGUpdate == true)
	{
		if(requiresGUpdate == true)
		{
			GuildHouses.clear();
			requiresGUpdate = false;
		}
		QueryResult Result = WorldDatabase.Query("SELECT guildId FROM guildhouses WHERE guildId != '0'");
		if(Result)
		{
			Field * pFields = Result->Fetch();
			int i = 0;
			do
			{
				GuildHouses.push_back(pFields[0].Get<uint64>());
				i++;
			}while(Result->NextRow());
			
			return isGuildHouseOwner(pPlayer);
		}
		else
		{
			return false;
		}
	}
	else
	{
		for(std::vector<uint64>::iterator itr = GuildHouses.begin(); itr != GuildHouses.end(); itr++)
		{
			if(*itr != pPlayer->GetGuildId())
				continue;
			else
				return true;
		}
		return false;
	}
}

bool isPlayerGuildLeader(Player *player)
{
	return (player->GetRank() == 0) && (player->GetGuildId() != 0);
}

bool GuildOwnsHouse(Player * pPlayer)
{
	QueryResult pResult = WorldDatabase.Query("SELECT * FROM guildhouses WHERE guildId='%u'", pPlayer->GetGuildId());
	if(!pResult)
		return false;
	else
		return true;
}

void CleanUpObjects(Player * pPlayer, uint32 guildId)
{
    Map* map = sMapMgr->FindMap(1, 0);
	QueryResult ObjectSpawns = WorldDatabase.Query("SELECT	`guid` FROM `guildhouse_objects` WHERE `guildId` = '%u'", guildId);
	if(ObjectSpawns)
	{
		do
		{
			Field * pField = ObjectSpawns->Fetch();
			uint32 Object_GUID = pField[0].Get<uint32>();
			GameObject* object = NULL;
            ChatHandler handler(pPlayer->GetSession());

            if (GameObjectData const* gameObjectData = sObjectMgr->GetGameObjectData(Object_GUID))
                GameObject* object = map->GetGameObject(ObjectGuid::Create<HighGuid::GameObject>(gameObjectData->id, Object_GUID));


			if (!object)
				continue;

			object->SetRespawnTime(0);
			object->Delete();
			object->DeleteFromDB();
			WorldDatabase.Execute("DELETE FROM `guildhouse_objects` WHERE guid='%u'", Object_GUID);
		}while(ObjectSpawns->NextRow());
	}
}

void SellGuildHouse(Player * pPlayer, Creature * pCreature)
{
	if(isInGuildArea(pPlayer))
	{
		WorldDatabase.Execute("UPDATE `guildhouses` SET `guildId`='0' WHERE `guildId`='%u'", pPlayer->GetGuildId());
		CleanUpObjects(pPlayer, pPlayer->GetGuildId());
        sWorld->SendPlayerText(pPlayer, "Ha vendido su casa de Hermandad!");
		pPlayer->PlayerTalkClass->SendCloseGossip();
		requiresGUpdate = true;
		pPlayer->TeleportTo(30, -349.165558, -654.866882, 126.338448, 0.817174); // generally the mall
	}
	else
	{
		pPlayer->GetSession()->SendNotification("Debes estar dentro de la Casa de Hermandad para venderla!");
	}
}

void PurchaseGuildHouse(Player * pPlayer, Creature * pCreature, uint32 &Id)
{
	if(pPlayer->HasItemCount(GUILD_TOKEN, 1))
	{
		WorldDatabase.Execute("UPDATE `guildhouses` SET `guildId`='%u' WHERE `id`='%u'", pPlayer->GetGuildId(), Id);
		sWorld->SendPlayerText(pPlayer, "Felicitaciones por la compra de tu Casa de Hermandad!");
		pPlayer->DestroyItemCount(GUILD_TOKEN, 1, true);
		requiresGUpdate = true;
	}
	else
	{
       sWorld->SendPlayerText(pPlayer,"Debes tener GuildTokens para comprar una Casa de Hermandad!",  pPlayer->GetGUID());
	}
		pPlayer->PlayerTalkClass->SendCloseGossip();
}

void TeleportToHouse(Player * pPlayer)
{
	if(pPlayer->IsInCombat())
		return;

	QueryResult result = WorldDatabase.Query("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `guildId` = '%u'", pPlayer->GetGuildId());
	if (result)
	{
		Field *fields = result->Fetch();
		float x = fields[0].Get<float>();
		float y = fields[1].Get<float>();
		float z = fields[2].Get<float>();
		uint32 map = fields[3].Get<uint32>();
		pPlayer->TeleportTo(map, x, y, z, pPlayer->GetOrientation());
	}
}

class GuildHouse_NPC : public CreatureScript
{
	public:
		GuildHouse_NPC() : CreatureScript("GuildHouse"){}


		void ShowHouseListings(Player * pPlayer, Creature * pCreature, unsigned int start = 0)
		{
			pPlayer->PlayerTalkClass->ClearMenus();

			QueryResult pResult = WorldDatabase.Query("SELECT id, comment FROM guildhouses WHERE guildId='0'");
			uint32 houses = pResult->GetRowCount();
			int offset = 0;
			Field * pField = pResult->Fetch();
			bool ranOnce = false;
			for(unsigned int i = start; i < houses; i++)
			{
				if(start != 0 && ranOnce == false)
				{
					for(unsigned int x = 0; x < start; x++)
					{
						pResult->NextRow();
					}
					ranOnce = true;
				}

				if(offset != 10)
				{
					AddGossipItemFor(pPlayer,6, pField[1].Get<std::string>(), GOSSIP_SENDER_MAIN, pField[0].Get<uint32>());
					offset++;
					pResult->NextRow();
				}
				else
				{
					if((startid + 12) > houses)
					{
						AddGossipItemFor(pPlayer,7, "-------------FIN--------------", GOSSIP_SENDER_MAIN, 41);
					}
					else
					{
						AddGossipItemFor(pPlayer,7, "Siguiente pagina", GOSSIP_SENDER_MAIN, 4353);
						startid = start + offset;
						i = houses +10;
					}
				}
			}
			pPlayer->PlayerTalkClass->SendGossipMenu(2, pCreature->GetGUID());
		}
		
		bool OnGossipHello(Player * pPlayer, Creature * pCreature)
		{
			startid = 0;

			if(pPlayer->GetGuildId() == 0)
			{
				pPlayer->GetSession()->SendNotification("No estas en una hermandad!");
				return false;
			}
			else if (!GuildOwnsHouse(pPlayer) && !isPlayerGuildLeader(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tu hermandad no tiene una Casa propia!");
				return false;
			}
			AddGossipItemFor(pPlayer,4, "Sistema de Casas de Hermandad Automatico:", GOSSIP_SENDER_MAIN, 41);
			AddGossipItemFor(pPlayer,4, "----------------------------------------------", GOSSIP_SENDER_MAIN, 41);
			if(isPlayerGuildLeader(pPlayer))
			{
				if(!GuildOwnsHouse(pPlayer))
					AddGossipItemFor(pPlayer,6, "Comprar Casa de Hermandad", GOSSIP_SENDER_MAIN, 38);
				else
					AddGossipItemFor(pPlayer,6, "Vender Casa de Hermandad", GOSSIP_SENDER_MAIN, 39, "Estas seguro de querer vender tu Casa de Hermandad?", 0, false);
			}
			if(GuildOwnsHouse(pPlayer))
				AddGossipItemFor(pPlayer,7, "Llevame a la Casa de Hermandad", GOSSIP_SENDER_MAIN, 40);
			pPlayer->PlayerTalkClass->SendGossipMenu(2, pCreature->GetGUID());
			return true;
		}

		bool OnGossipSelect(Player * pPlayer, Creature * pCreature, uint32 /*uiSender*/, uint32 uiAction)
		{
			pPlayer->PlayerTalkClass->ClearMenus();

			switch(uiAction)
			{
				case 38:
					ShowHouseListings(pPlayer, pCreature);
					break;
				case 39:
					SellGuildHouse(pPlayer, pCreature);
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
				case 40:
					TeleportToHouse(pPlayer);
					break;
				case 41:
					OnGossipHello(pPlayer, pCreature);
					break;
				case 4353:
					ShowHouseListings(pPlayer, pCreature, startid);
					break;
				default:
					if(uiAction < 38 && uiAction > 0)
						PurchaseGuildHouse(pPlayer, pCreature, uiAction);
					break;
			}
			return true;
		}

		private:
			uint32 startid;
};


class Table_Token : public ItemScript
{
	public:
		Table_Token() : ItemScript("Table Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_TABLE);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);
			return true;
		}
};

class Torch_Token : public ItemScript
{
	public:
		Torch_Token() : ItemScript("Torch Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;
			
			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_TORCH);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);
			
			return true;
		}
};

class Mailbox_Token : public ItemScript
{
	public:
		Mailbox_Token() : ItemScript("Mailbox Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_MAILBOX);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);

			return true;
		}
};

class Crate_Token : public ItemScript
{
	public:
		Crate_Token() : ItemScript("Crate Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_CRATE);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);

			return true;
		}
};

class A_Flag_Token : public ItemScript
{
	public:
		A_Flag_Token() : ItemScript("A_Flag Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_A_FLAG);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);

			return true;
		}
};

class H_Flag_Token : public ItemScript
{
	public:
		H_Flag_Token() : ItemScript("H_Flag Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}


			SpawnObject(pPlayer, OBJECT_H_FLAG);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);

			return true;
		}
};

class Chair_Token : public ItemScript
{
	public:
		Chair_Token() : ItemScript("Chair Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_CHAIR);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);

			return true;
		}
};

class Firepit_Token : public ItemScript
{
	public:
		Firepit_Token() : ItemScript("Firepit Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_FIREPIT);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);

			return true;
		}
};

class Vault_Token : public ItemScript
{
	public:
		Vault_Token() : ItemScript("Vault Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_GUILDVAULT);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);

			return true;
		}
};

class Barber_Token : public ItemScript
{
	public:
		Barber_Token() : ItemScript("Barber Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			}

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_BARBER_CHAIR);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);
			
			return true;
		}
};

class Cranberry_Token : public ItemScript
{
	public:
		Cranberry_Token() : ItemScript("Cranberry Token"){}

		bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
		{
			if(!pPlayer)
				return false;

			if(!pItem)
				return false;

			if(!isPlayerGuildLeader(pPlayer) || !isGuildHouseOwner(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Tienes que tener una Casa de Hermandad y estar dentro para utilizar esto!");
				return false;
			};

			if(!isInGuildArea(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("No estas dentro del Area de tu Casa de Hermandad!");
				return false;
			}

			SpawnObject(pPlayer, OBJECT_CRANBERRY_BUSH);
			pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);

			return true;
		}
};

//token vendor

class Guild_Token_Vendor : public CreatureScript
{
	public:
		Guild_Token_Vendor() : CreatureScript("Token Vendor"){}

		bool OnGossipHello(Player * pPlayer, Creature * pCreature)
		{
			if(!isPlayerGuildLeader(pPlayer))
			{
				pPlayer->GetSession()->SendNotification("Debes ser el Maestro de tu hermandad!");
				pPlayer->PlayerTalkClass->SendCloseGossip();
				return false;
			}
			if(isGuildHouseOwner(pPlayer))
			{
				pPlayer->PlayerTalkClass->ClearMenus();
			    AddGossipItemFor(pPlayer, 4, "Me gustaria comprar fichas de objetos", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
				AddGossipItemFor(pPlayer,3, "No importa.", GOSSIP_SENDER_MAIN, 2);
				pPlayer->PlayerTalkClass->SendGossipMenu(2, pCreature->GetGUID());
				//return true;
			}
			else
			{
				pPlayer->GetSession()->SendNotification("Debes tener una Casa de Hermandad!");
				return false;
			}
			return true;
		}

		bool OnGossipSelect(Player * pPlayer, Creature * pCreature, uint32 /*uiSender*/, uint32 uiAction)
		{
			switch(uiAction)
			{
				case GOSSIP_ACTION_TRADE:
					pPlayer->GetSession()->SendListInventory(pCreature->GetGUID());
					break;
				case 2:
					pPlayer->PlayerTalkClass->SendCloseGossip();
					break;
			}
			return true;
		}
};


void AddSC_GuildHouse()
{
	new GuildHouse_NPC();
 	new Table_Token();
	new Torch_Token();
	new Mailbox_Token();
	new Crate_Token();
	new A_Flag_Token();
	new H_Flag_Token();
	new Chair_Token();
	new Firepit_Token();
	new Vault_Token();
	new Barber_Token();
	new Guild_Token_Vendor();
	new Cranberry_Token();
}
