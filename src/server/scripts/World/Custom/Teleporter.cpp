/*******************************************************************************************
* 									Creditos: Ray                                  	       *
********************************************************************************************/

#include "ScriptPCH.h"
#include "Group.h"
#include "GroupMgr.h"
#include "Battleground.h"
#include "Player.h"
#include "Chat.h"

const int MAX_PLAYER_SPAWN_POINTS = 20;
static const Position playerSpawnPoint[MAX_PLAYER_SPAWN_POINTS] = 
{
	{-1172.90f, -5331.40f, 4.85f, 4.64f}, 
	{-1319.34f, -5385.09f, 3.34f, 5.88f},
	{-1135.17f, -5587.93f, 10.5f, 1.53f},
	{-1235.21f, -5592.60f, 8.49f, 1.39f},
	{-1056.33f, -5639.83f, 5.49f, 1.74f},
	{-1031.71f, -5530.50f, 4.75f, 3.14f},
	{-989.878f, -5418.93f, 3.90f, 3.26f},
	{-1077.64f, -5347.45f, 3.93f, 4.18f},
	{-1136.49f, -5420.34f, 11.4f, 3.23f},
	{-1235.88f, -5462.10f, 5.76f, 1.29f},
	{-1162.98f, -5585.53f, 12.3f, 2.29f},
	{-840.228f, -5616.32f, 2.97f, 6.28f},
	{-719.558f, -5685.03f, 23.5f, 1.78f},
	{-638.830f, -5622.10f, 6.42f, 2.90f},
	{-662.432f, -5509.45f, 4.08f, 3.95f},
	{-772.277f, -5353.24f, 2.03f, 2.83f},
	{-949.171f, -5179.04f, 2.11f, 3.98f},
	{-1174.97f, -5116.36f, 3.65f, 6.14f},
	{-1372.61f, -5169.68f, 1.81f, 0.45f},
	{-1602.78f, -5250.65f, 6.50f, 5.05f},
};

const int MAX_PLAYER_SPAWN_POINTS_EVENTO_LABERINTO = 8;
static const Position playerSpawnPointEventoLaberinto[MAX_PLAYER_SPAWN_POINTS_EVENTO_LABERINTO] = 
{
	{-7364.641113f, 1105.026978f, 131.407379f, 1.551686f},
	{-7403.822754f, 1041.762939f, 131.408051f, 0.037827f},
	{-7441.120605f, 1101.512695f, 131.407288f, 4.742365f},
	{-7494.867676f, 1116.905029f, 131.406952f, 4.716446f},
	{-7508.709473f, 1028.469971f, 131.407089f, 6.271536f},
	{-7476.597656f, 851.968018f, 131.407394f, 1.533622f},
	{-7422.970703f, 877.438223f, 131.407532f, 3.188865f},
	{-7447.919434f, 1048.742310f, 153.471115f, 0.000129f},
};

uint32 Jugadores()
{
	QueryResult contar_area = CharacterDatabase.Query("SELECT COUNT(area) FROM characters WHERE area = 368");

	if (!contar_area)
		return 0;
	else
	{
		Field* fields = contar_area->Fetch();
			return fields[0].Get<uint32>();
	}
}

int Evento_Escalada_Teleport()
{
	QueryResult evento_escalada_ = WorldDatabase.Query("SELECT Habilitado_Teleport FROM eventos_custom WHERE id = 1");

	if (!evento_escalada_)
		return 0;
	else
	{
		Field* fields = evento_escalada_->Fetch();
			return fields[0].Get<uint32>();
	}
}

int Evento_Laberinto_Teleport()
{
	QueryResult evento_escalada_ = WorldDatabase.Query("SELECT Habilitado_Teleport_laberinto FROM eventos_custom WHERE id = 1");

	if (!evento_escalada_)
		return 0;
	else
	{
		Field* fields = evento_escalada_->Fetch();
			return fields[0].Get<uint32>();
	}
}

int Evento_Laberinto1()
{
	QueryResult evento_laberinto1 = WorldDatabase.Query("SELECT evento_laberinto1 FROM eventos_custom WHERE id = 1");

	if (!evento_laberinto1)
		return 0;
	else
	{
		Field* fields = evento_laberinto1->Fetch();
			return fields[0].Get<uint32>();
	}
}

int Evento_Escalada1()
{
	QueryResult evento_escalada1 = WorldDatabase.Query("SELECT evento_escalada1 FROM eventos_custom WHERE id = 1");

	if (!evento_escalada1)
		return 0;
	else
	{
		Field* fields = evento_escalada1->Fetch();
			return fields[0].Get<uint32>();
	}
}

int Evento_Escalada2()
{
	QueryResult evento_escalada2 = WorldDatabase.Query("SELECT evento_escalada2 FROM eventos_custom WHERE id = 1");

	if (!evento_escalada2)
		return 0;
	else
	{
		Field* fields = evento_escalada2->Fetch();
			return fields[0].Get<uint32>();
	}
}

class Teleporter : public CreatureScript
{
	public:
		Teleporter() : CreatureScript("Teleporter") { }

	
        bool OnGossipHello(Player* player, Creature* creature)
        {
			if(player->IsInCombat())
			{
				ChatHandler(player->GetSession()).PSendSysMessage("No puedes usar el Taxi en combate.");
				player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
				CloseGossipMenuFor(player);
				return true;
			}
			
			Group* grupo = player->GetGroup();
		
			std::ostringstream ss;
			
			ss << "|cff9b0000Zona PvP"/* (Hay " << Jugadores() << (Jugadores() != 1 ? " jugadores" : " jugador") << " en la Zona)"*/;
			
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player,5, "|cff2e2efeShops", GOSSIP_SENDER_MAIN, 24);
			/*if (player->GetTeam() == HORDE)
				AddGossipItemFor(player,4, "Capital (Zona en Disputa)", GOSSIP_SENDER_MAIN, 1);
			else
				AddGossipItemFor(player,4, "Capital (Zona en Disputa)", GOSSIP_SENDER_MAIN, 2);*/
			AddGossipItemFor(player,2, "|cff8410a4Shop Monturas Custom", GOSSIP_SENDER_MAIN, 3, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop Monturas Custom?|R", 0, false);
			AddGossipItemFor(player,2, "Zona de Duelos", GOSSIP_SENDER_MAIN, 6, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Zona de Duelos?|R", 0, false);
			AddGossipItemFor(player,5, "|cff006800Eventos", GOSSIP_SENDER_MAIN, 22);
			if (grupo)
				AddGossipItemFor(player,2, ss.str().c_str(), GOSSIP_SENDER_MAIN, 26);
			else
				AddGossipItemFor(player,2, ss.str().c_str(), GOSSIP_SENDER_MAIN, 5, "|CFFFFFF01""\xc2\xbf""Deseas ir a la Zona PvP?|R", 0, false);
			AddGossipItemFor(player,2, "Arena de Gurubashi", GOSSIP_SENDER_MAIN, 7, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Arena Gurubashi?\n NOTA: El Evento de la Arqueta se realiza aquí(en Gurubashi) cada día a las 16:30, 19:30 y 23:30 Hora del Servidor.|R", 0, false);
			AddGossipItemFor(player,5, "Ciudades", GOSSIP_SENDER_MAIN, 212);

			player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
			return true;
		}

		bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
			
			switch (action)
            {				
				case 14:
					OnGossipHello(player, creature);
                break;

				/*case 8:
					CloseGossipMenuFor(player);
				break;*/

				// Menu Shop Horda
                /*case 1:
                   AddGossipItemFor(player,2, "Shop Inicial", GOSSIP_SENDER_MAIN, 9, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop Inicial?|R", 0, false);
					AddGossipItemFor(player,2, "Shop Extended", GOSSIP_SENDER_MAIN, 10, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop Extended?|R", 0, false);
					AddGossipItemFor(player,2, "Tabernero & Arenas", GOSSIP_SENDER_MAIN, 11, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Tabernero & Arenas?|R", 0, false);
					AddGossipItemFor(player,2, "Zona de Profesiones", GOSSIP_SENDER_MAIN, 12, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Zona de Profesiones?|R", 0, false);
					AddGossipItemFor(player,2, "Zona de Transfiguraci""\xC3\xB3""n", GOSSIP_SENDER_MAIN, 13, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Zona de Transfiguraci""\xC3\xB3""n?|R", 0, false);
					AddGossipItemFor(player,7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 14);
					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
                break;
				
				case 9:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 5553.54f, -2988.94f, 1537.2f, 5.98948f);					
				break;
				
				case 10:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 5528.1f, -2815.87f, 1499.96f, 5.56455f);					
				break;

				case 11:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 5341.54f, -2990.85f, 1538.73f, 5.02699f);					
				break;
				
				case 12:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 5175.88f, -3033.42f, 1570.76f, 3.76841f);					
				break;
								
				case 13:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 5281.76f, -2911.24f, 1529.3f, 3.21289f);					
				break;*/
				
				// Menu Shop Alianza
				/*case 2:
                    AddGossipItemFor(player,2, "Shop Inicial", GOSSIP_SENDER_MAIN, 15, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop Inicial?|R", 0, false);
					AddGossipItemFor(player,2, "Shop Extended", GOSSIP_SENDER_MAIN, 16, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir al Shop Extended?|R", 0, false);
					AddGossipItemFor(player,2, "Tabernero & Arenas", GOSSIP_SENDER_MAIN, 17, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Tabernero & Arenas?|R", 0, false);
					AddGossipItemFor(player,2, "Zona de Profesiones", GOSSIP_SENDER_MAIN, 18, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Zona de Profesiones?|R", 0, false);
					AddGossipItemFor(player,2, "Zona de Transfiguraci""\xC3\xB3""n", GOSSIP_SENDER_MAIN, 19, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Zona de Transfiguraci""\xC3\xB3""n?|R", 0, false);
					AddGossipItemFor(player,7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 14);
					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
					return true;
                break;
				
				case 15:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 5114.72f, -1779.6f, 1357.01f, 1.63801f);					
				break;
				
				case 16:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 5129.6f, -1780.59f, 1348.49f, 3.4501f);					
				break;

				case 17:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 5110.17f, -1785.33f, 1322.64f, 0.119992f);					
				break;
				
				case 18:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 4984.92f, -1770.42f, 1331.2f, 1.7177f);					
				break;
								
				case 19:
					CloseGossipMenuFor(player);
					player->TeleportTo(534, 4931.09f, -1892.52f, 1326.59f, 3.4863f);					
				break;*/
				
				case 3:
				{
					player->TeleportTo(0, -12024.9f, -1702.49f, 39.3102f, 0.553043f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 4:
				{
					if (player->HasItemCount(1404, 1) || player->HasItemCount(13209, 1))
					{
					    player->TeleportTo(530, 10389.4f, -6403.07f, 161.052f, 3.48574f);
						CloseGossipMenuFor(player);
						return true;
					}
					else
					{
					   	ChatHandler(player->GetSession()).PSendSysMessage("Debes ser VIP para poder acceder a esta zona.");	
                        player->GetSession()->SendAreaTriggerMessage("No eres VIP.");
						CloseGossipMenuFor(player);
						return true;
					}
				} break;
				
				case 5:
				{
					ObjectGuid guid = sCharacterCache->GetCharacterGroupGuidByGuid(player->GetGUID());
					if(QueryResult resultGroup = CharacterDatabase.Query("SELECT guid FROM group_member WHERE memberGuid=%u", guid.GetCounter()))
						if(Group* group = sGroupMgr->GetGroupByGUID((*resultGroup)[0].Get<uint32>()))
							player->RemoveFromGroup(group, player->GetGUID());
					int i = urand(0, MAX_PLAYER_SPAWN_POINTS - 1);
					player->TeleportTo(1, playerSpawnPoint[i].GetPositionX(), playerSpawnPoint[i].GetPositionY(), playerSpawnPoint[i].GetPositionZ(), playerSpawnPoint[i].GetOrientation());
					player->CastSpell(player, 8611, false);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 26:
				{
					AddGossipItemFor(player,2, "Ir a Zona PvP - Solo", GOSSIP_SENDER_MAIN, 5, "|CFFFFFF01""\xc2\xbf""Deseas ir a la Zona PvP?\nSi aceptas, serás expulsado del grupo en el que estés.|R", 0, false);
					AddGossipItemFor(player,2, "Ir a Zona PvP - Grupo", GOSSIP_SENDER_MAIN, 27, "|CFFFFFF01""\xc2\xbf""Deseas ir a la Zona PvP?\nTodo tu grupo será teletransportado a la Zona PvP, a la misma posicion que tú.\nNOTA: no se permiten más de 2 jugadores por grupo.|R", 0, false);
					AddGossipItemFor(player,7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 14);
					
					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
					return true;
				}
				break;
				
				case 27:
				{
					Group* grupo = player->GetGroup();
					if (grupo && grupo->GetMembersCount() > 2)
					{
						ChatHandler(player->GetSession()).PSendSysMessage("No puedes ir con un grupo mayor de 2 personas a la Zona PvP.");
						CloseGossipMenuFor(player);
						return true;
					}
					else
					{
						Group* grupo = player->GetGroup();
						int i = urand(0, MAX_PLAYER_SPAWN_POINTS - 1);
						if (grupo)
							{
								for(GroupReference *itr = grupo->GetFirstMember(); itr != NULL; itr = itr->next())
								{
									Player* pl = itr->GetSource();

									if(!pl || !pl->GetSession())
										continue;

									if(pl->IsBeingTeleported() == true)
									{
									    ChatHandler(player->GetSession()).PSendSysMessage("Un miembro de tu grupo ya está siendo teletransportado.");
										return false;
									}

									if(pl->IsInFlight())
									{
										pl->GetMotionMaster()->MovementExpired();
										pl->CleanupAfterTaxiFlight();
									}
									else
										pl->SaveRecallPosition();

									pl->TeleportTo(1, playerSpawnPoint[i].GetPositionX(), playerSpawnPoint[i].GetPositionY(), playerSpawnPoint[i].GetPositionZ(), playerSpawnPoint[i].GetOrientation());
									pl->CastSpell(pl, 8611, false);
								}
							}
						else
							player->TeleportTo(1, playerSpawnPoint[i].GetPositionX(), playerSpawnPoint[i].GetPositionY(), playerSpawnPoint[i].GetPositionZ(), playerSpawnPoint[i].GetOrientation());
						player->CastSpell(player, 8611, false);
						CloseGossipMenuFor(player);
						return true;
					}
				} break;
				
				case 6:
					CloseGossipMenuFor(player);
					player->TeleportTo(530, -3330.32f, 4935.4f, -99.0472f, 3.057f);
				break;
				
				case 7:
					CloseGossipMenuFor(player);
					player->TeleportTo(0, -13244.107422f, 197.539185f, 30.955976f, 1.090968f);
				break;
				
				case 20:
					CloseGossipMenuFor(player);
					player->TeleportTo(1, -2315.19951f, -413.394012f, -6.3581f, 1.916640f);
				break;

				// Menu Shop
				case 24:
				{
				    AddGossipItemFor(player,2, "|cff006800Shop", GOSSIP_SENDER_MAIN, 20, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop?|R", 0, false); 
			        AddGossipItemFor(player,2, "|cff2e2efeShop VIP", GOSSIP_SENDER_MAIN, 4, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Shop VIP?|R", 0, false);
					AddGossipItemFor(player,7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 14);

					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
					return true;
				} break;

				// Menu Eventos
				case 22:
				    AddGossipItemFor(player,2, "Evento Laberinto", GOSSIP_SENDER_MAIN, 21, "|CFFFFFF01Solamente podrás ir si está activado el evento que lo activa un MJ. En el momento en que lo activa tendrás 2 minutos para poder ir. Si estás en grupo, todo el grupo se teletransportará contigo al mismo sitio.|R", 0, false);			
			        //AddGossipItemFor(player,2, "Evento de la Arqueta", GOSSIP_SENDER_MAIN, 7, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a la Arena Gurubashi?\n NOTA: Este evento es cada día a las 16:30, 19:30 y 23:30 Hora del Servidor.|R", 0, false);
					AddGossipItemFor(player,2, "Evento de Escalada", GOSSIP_SENDER_MAIN, 23);
					//AddGossipItemFor(player,2, "El Conflicto de Hyjal", GOSSIP_SENDER_MAIN, 25);
					AddGossipItemFor(player,7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 14);
					
					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
				break;
				
				// Evento de Escalada
				case 23:
				{
					if(Evento_Escalada_Teleport() == 0)
					{
						ChatHandler(player->GetSession()).PSendSysMessage("No puedes ir a este lugar porque aún no está activado el Evento de Escalada. Este evento lo inicia un MJ cuando él lo vea conveniente y siempre con previo aviso.");
						CloseGossipMenuFor(player);
						return true;
					}
					else if(Evento_Escalada_Teleport() == 1)
					{
						if(Evento_Escalada1() == 1 && Evento_Escalada2() == 0)
							player->TeleportTo(0, -1311.385132f, -1263.237427f, 164.758301f, 3.251770f);
						else if(Evento_Escalada2() == 1 && Evento_Escalada1() == 0)
							player->TeleportTo(0, 4302.835449f, -2844.920654f, 5.528601f, 5.331080f);
						else
							ChatHandler(player->GetSession()).PSendSysMessage("Hay dos o más eventos de escalada activados al mismo tiempo, díselo a un administrador.");

						CloseGossipMenuFor(player);
						return true;
					}
				} break;
				 
				// Anuncio Hyjal
				/*case 25:
				    ChatHandler(player->GetSession()).PSendSysMessage("El Conflicto de Hyjal se encuentra actualmente en desarrollo, os informaremos cuando se encuentre finalizado. El motivo es porque es demasiado complicado y se necesita más gente en el servidor, cuando haya más gente se activará");
					player->GetSession()->SendAreaTriggerMessage("El Conflicto de Hyjal no se encuentra disponible por el momento.");
					CloseGossipMenuFor(player);
				    return true;*/

				case 21: //Evento Laberinto (id de game_event = 89)
				{
					if(Evento_Laberinto_Teleport() == 0)
					{
						ChatHandler(player->GetSession()).PSendSysMessage("No puedes ir a este lugar porque aún no está activado el Evento Laberinto. Este evento lo inicia un MJ cuando él lo vea conveniente y siempre con previo aviso.");
						CloseGossipMenuFor(player);
						return true;
					}
					else if(Evento_Laberinto_Teleport() == 1)
					{
						if(Evento_Laberinto1() == 1)
							player->TeleportTo(1, -8592.215820f, -1179.367554f, 8.877270f, 2.427462f);

						CloseGossipMenuFor(player);
						return true;
					}
					
				} break;
				
				case 212:
				{
					if (player->GetTeamId() == HORDE)
					{
						AddGossipItemFor(player,2, "Orgrimmar", GOSSIP_SENDER_MAIN, 213, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Orgrimmar?", 0, false);
						AddGossipItemFor(player,2, "Cima del Trueno", GOSSIP_SENDER_MAIN, 214, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Cima del Trueno?", 0, false);
						AddGossipItemFor(player,2, "Entra""\xC3\xB1""as", GOSSIP_SENDER_MAIN, 215, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Entra""\xC3\xB1""s?", 0, false);
						AddGossipItemFor(player,2, "Lunargenta", GOSSIP_SENDER_MAIN, 216, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Lunargenta?", 0, false);
					}
					else
					{
						AddGossipItemFor(player,2, "Ventormenta", GOSSIP_SENDER_MAIN, 217, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Ventormenta?", 0, false);
						AddGossipItemFor(player,2, "Darnassus", GOSSIP_SENDER_MAIN, 218, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Darnassus?", 0, false);
						AddGossipItemFor(player,2, "Forjaz", GOSSIP_SENDER_MAIN, 219, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Forjaz?", 0, false);
						AddGossipItemFor(player,2, "El Exodar", GOSSIP_SENDER_MAIN, 220, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a El Exodar?", 0, false);					
					}

					AddGossipItemFor(player,2, "Dalaran", GOSSIP_SENDER_MAIN, 221, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Dalaran?", 0, false);
					AddGossipItemFor(player,2, "Shattrath", GOSSIP_SENDER_MAIN, 222, "|CFFFFFF01""\xc2\xbf""Est""\xC3\xA1""s seguro de querer ir a Shattrath?", 0, false);					
					AddGossipItemFor(player,7, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 14);
					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
				} break;

				case 213:
				{
					player->TeleportTo(1, 1601.08f, -4378.69f, 9.9846f, 2.14362f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 214:
				{
					player->TeleportTo(1, -1274.45f, 71.8601f, 128.159f, 2.80623f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 215:
				{
					player->TeleportTo(0, 1633.75f, 240.167f, -43.1034f, 6.26128f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 216:
				{
					player->TeleportTo(530, 9738.28f, -7454.19f, 13.5605f, 0.043914f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 217:
				{
					player->TeleportTo(0, -8842.09f, 626.358f, 94.0867f, 3.61363f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 218:
				{
					player->TeleportTo(1, 9869.91f, 2493.58f, 1315.88f, 2.78897f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 219:
				{
					player->TeleportTo(0, -4900.47f, -962.585f, 501.455f, 5.40538f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 220:
				{
					player->TeleportTo(530, -3864.92f, -11643.7f, -137.644f, 5.50862f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 221:
				{
					player->TeleportTo(571, 5809.55f, 503.975f, 657.526f, 2.38338f);
					CloseGossipMenuFor(player);
					return true;
				} break;
				
				case 222:
				{
					player->TeleportTo(530, -1887.62f, 5359.09f, -12.4279f, 4.40435f);
					CloseGossipMenuFor(player);
					return true;
				} break;
			}
			return true;
		}
};

void AddSC_teleport_array()
{
    new Teleporter();
}
