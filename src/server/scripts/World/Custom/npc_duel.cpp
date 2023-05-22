#include "ScriptPCH.h"
// Set USE_TOKEN to 1 if you want to have it use tokens in place of gold
#define USE_TOKEN 	0
#define TOKEN_ID	29434

struct BloodMoneyInfo
{
	ObjectGuid guid;
	uint32 amount;
	bool accepted;
};

typedef std::list<BloodMoneyInfo> BloodMoneyList;
typedef std::map<ObjectGuid, BloodMoneyList> BloodMoney;
static BloodMoney m_bloodMoney;

bool HasBloodMoneyChallenger(ObjectGuid playerGUID)
{
	return m_bloodMoney.find(playerGUID) != m_bloodMoney.end();
}

bool HasBloodMoneyChallenger(ObjectGuid targetGUID, ObjectGuid playerGUID)
{
	if (!HasBloodMoneyChallenger(targetGUID))
		return false;
	BloodMoneyList bml = m_bloodMoney[targetGUID];
	for (BloodMoneyList::const_iterator itr = bml.begin(); itr != bml.end(); ++itr)
		if (itr->guid == playerGUID)
			return true;
	return false;
}

void AddBloodMoneyEntry(ObjectGuid targetGUID, ObjectGuid playerGUID, uint32 amount)
{
    BloodMoneyInfo bmi = {};  // Inicializar la variable bmi con ceros

    bmi.guid = playerGUID;
    bmi.amount = amount;
    bmi.accepted = false;

    m_bloodMoney[targetGUID].push_back(bmi);
}

void RemoveBloodMoneyEntry(ObjectGuid targetGUID, ObjectGuid playerGUID)
{
    if (!HasBloodMoneyChallenger(targetGUID, playerGUID))
        return;
    BloodMoneyList& list = m_bloodMoney[targetGUID];
    BloodMoneyList::iterator itr;
    for (itr = list.begin(); itr != list.end(); ++itr) // Corrección: debe ser 'list.end()' en lugar de 'list.begin()'
    {
        if (itr->guid == playerGUID)
        {
            list.erase(itr);
            break;
        }
    }
}


void SetChallengeAccepted(ObjectGuid targetGUID, ObjectGuid playerGUID)
{
	if (!HasBloodMoneyChallenger(targetGUID, playerGUID))
		return;
	BloodMoneyList &list = m_bloodMoney[targetGUID];
	BloodMoneyList::iterator itr;
	for (itr = list.begin(); itr != list.end(); ++itr)
	{
		if (itr->guid == playerGUID)
		{
			itr->accepted = true;
			break;
		}
	}
}

class npc_blood_money : public CreatureScript
{
	public : 
		npc_blood_money() : CreatureScript("npc_blood_money") {}

	bool OnGossipHello(Player * player, Creature * creature)
	{
		player->PlayerTalkClass->ClearMenus();
		AddGossipItemFor(player,GOSSIP_ICON_BATTLE, player->GetSession()->GetAcoreString(15001), 11, 1000);	
		if (HasBloodMoneyChallenger(player->GetGUID()))
		{
			BloodMoneyList list = m_bloodMoney[player->GetGUID()];
			for (BloodMoneyList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
			{
				char msg[120];

                if (Player* plr = ObjectAccessor::GetPlayer(player, itr->guid))



				{
                    if (USE_TOKEN)
                    {
                        sprintf_s(msg, sizeof(msg), "Accept %s's Challenge of %d tokens", plr->GetName(), itr->amount);
                        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, msg, GOSSIP_SENDER_MAIN, itr->guid);
                        sprintf_s(msg, sizeof(msg), "Decline %s's Challenge of %d tokens", plr->GetName(), itr->amount);
                        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, msg, GOSSIP_SENDER_INFO, itr->guid);
                    }
                    else
                    {
                        sprintf_s(msg, sizeof(msg), "%s", (player->GetSession()->GetAcoreString(15002)).c_str(), plr->GetName().c_str(), itr->amount / 10000);
                        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, msg, GOSSIP_SENDER_MAIN, itr->guid);
                        sprintf_s(msg, sizeof(msg), "%s", (player->GetSession()->GetAcoreString(15003)).c_str(), plr->GetName().c_str(), itr->amount / 10000);
                        AddGossipItemFor(player, GOSSIP_ICON_INTERACT_1, msg, GOSSIP_SENDER_INFO, itr->guid);
                    }

					
				}
			}
		}
		AddGossipItemFor(player,GOSSIP_ICON_CHAT, player->GetSession()->GetAcoreString(15004), GOSSIP_SENDER_MAIN, 1);
		
		SendGossipMenuFor(player,80025, creature->GetGUID());

		return true;
	}

	bool OnGossipSelect(Player * player, Creature * creature, uint32 uiSender, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();
		if (uiAction == 1)
		{
			CloseGossipMenuFor(player);
			return true;
		}
		switch(uiSender)
		{
			case GOSSIP_SENDER_MAIN:
				if (Player* target = Player::GetPlayer(*player, uiAction))
				{
					SetChallengeAccepted(player->GetGUID(), target->GetGUID());
					char msg[110];
					sprintf(msg, (player->GetSession()->GetAcoreString(15005)), player->GetName());
					creature->Whisper(msg, LANG_UNIVERSAL, target, true);
					CloseGossipMenuFor(player);
				}
				break;
			case GOSSIP_SENDER_INFO:
				if (Player* target = Player::GetPlayer(*player, uiAction))
				{
					char msg[110];
					sprintf(msg, (player->GetSession()->GetAcoreString(15006)), player->GetName());
					creature->Whisper(msg, LANG_UNIVERSAL, target, true);
					RemoveBloodMoneyEntry(player->GetGUID(), uiAction);
					OnGossipHello(player, creature);
				}
				break;
			case 11:
				if (USE_TOKEN)
				{
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 5 tokens", GOSSIP_SENDER_MAIN, 5, "Type in the player's name", 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 10 tokens", GOSSIP_SENDER_MAIN, 10, "Type in the player's name", 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 15 tokens", GOSSIP_SENDER_MAIN, 15, "Type in the player's name", 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 25 tokens", GOSSIP_SENDER_MAIN, 25, "Type in the player's name", 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 50 tokens", GOSSIP_SENDER_MAIN, 50, "Type in the player's name", 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 100 tokens", GOSSIP_SENDER_MAIN, 100, "Type in the player's name", 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 150 tokens", GOSSIP_SENDER_MAIN, 150, "Type in the player's name", 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 200 tokens", GOSSIP_SENDER_MAIN, 200, "Type in the player's name", 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, "Bet 250 tokens", GOSSIP_SENDER_MAIN, 250, "Type in the player's name", 0, true);
				}
				else
				{
					const char* S15034 = (player->GetSession()->GetAcoreString(15034));
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15007), GOSSIP_SENDER_MAIN, 5, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15008), GOSSIP_SENDER_MAIN, 10, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15009), GOSSIP_SENDER_MAIN, 15, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15010), GOSSIP_SENDER_MAIN, 25, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15011), GOSSIP_SENDER_MAIN, 50, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15012), GOSSIP_SENDER_MAIN, 100, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15013), GOSSIP_SENDER_MAIN, 150, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15014), GOSSIP_SENDER_MAIN, 200, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15015), GOSSIP_SENDER_MAIN, 250, S15034, 0, true);
					AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15016), GOSSIP_SENDER_MAIN, 500, S15034, 0, true);
				    AddGossipItemFor(player,GOSSIP_ICON_MONEY_BAG, player->GetSession()->GetAcoreString(15017), GOSSIP_SENDER_MAIN, 2000, S15034, 0, true);
				}
				
				SendGossipMenuFor(player,80025, creature->GetGUID());
				break;
		}
		return true;
	}

	bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code)
	{

		if (player->GetName() == code)
		{
			ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15018));
			return false;
		}
		if (ObjectGuid targetGUID = sObjectMgr->GetPlayerGUIDByName(code))
		{
			if (Player* target = Player::GetPlayer(*player, targetGUID))
			{
				if (target->GetGUID() == player->GetGUID())
				{
					ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15018));
					return false;
				}
				if (target->GetZoneId() == player->GetZoneId())
				{
					if (USE_TOKEN)
					{
						if (target->GetItemCount(TOKEN_ID) < action)
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|cff800C0C[] |cffFFFFFFThat player does not have enough tokens to make the bet!");
							CloseGossipMenuFor(player);
							return false;
						}
						if (player->GetItemCount(TOKEN_ID) < action)
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|cff800C0C[] |cffFFFFFFYou do not have enough tokens to make the bet!");
							CloseGossipMenuFor(player);
							return false;
						}
						
						bool found = false;
						if (HasBloodMoneyChallenger(player->GetGUID()))
						{
							BloodMoneyList list = m_bloodMoney[player->GetGUID()];
							for (BloodMoneyList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
								if (itr->guid == target->GetGUID())
									found = true;
						}
						if (!found)
						{
							if (!HasBloodMoneyChallenger(target->GetGUID(), player->GetGUID()))
							{
								AddBloodMoneyEntry(target->GetGUID(), player->GetGUID(), action);
								char msg[110];
								sprintf(msg, (player->GetSession()->GetAcoreString(15019)), player->GetName());
								creature->Whisper(msg, LANG_UNIVERSAL, target, true);
							}
							else
								ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15020));
						}
						else
							ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15021));
						CloseGossipMenuFor(player);
						return true;
					}
					else
					{
						uint32 money = action*10000;
						if (target->GetMoney() < money)
						{
							ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15022));
							CloseGossipMenuFor(player);
							return false;
						}
						if (player->GetMoney() < money)
						{
							ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15023));
							CloseGossipMenuFor(player);
							return false;
						}

						bool found = false;
						if (HasBloodMoneyChallenger(player->GetGUID()))
						{
							BloodMoneyList list = m_bloodMoney[player->GetGUID()];
							for (BloodMoneyList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
								if (itr->guid == target->GetGUID())
									found = true;
						}
						if (!found)
						{
							if (!HasBloodMoneyChallenger(target->GetGUID(), player->GetGUID()))
							{
								AddBloodMoneyEntry(target->GetGUID(), player->GetGUID(), money);
								char msg[110];
								sprintf(msg, (player->GetSession()->GetAcoreString(15024)), player->GetName());
								creature->Whisper(msg,LANG_UNIVERSAL, target, true);
							}
							else
								ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15025));
						}
						else
							ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15026));
						CloseGossipMenuFor(player);
						return true;
					}
					
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15027));
					CloseGossipMenuFor(player);
					return false;
				}
			}
			else
			{
				ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15028));
				CloseGossipMenuFor(player);
				return false;
			}
		}
		else
		{
			ChatHandler(player->GetSession()).PSendSysMessage(player->GetSession()->GetAcoreString(15028));
			CloseGossipMenuFor(player);
			return false;
		}
		CloseGossipMenuFor(player);
		return true;
	}

};

class BloodMoneyReward : public PlayerScript
{
 public:
	 BloodMoneyReward() : PlayerScript("BloodMoneyReward") {}
	 
	void OnDuelEnd(Player* winner, Player* loser, DuelCompleteType type) 
	{  
		if (HasBloodMoneyChallenger(winner->GetGUID()) || HasBloodMoneyChallenger(loser->GetGUID()))
		{			 
			BloodMoneyList list1 = m_bloodMoney[winner->GetGUID()];
			BloodMoneyList list2 = m_bloodMoney[loser->GetGUID()];

			BloodMoneyList::const_iterator itr;
			for (itr = list1.begin(); itr != list1.end(); ++itr)
			{
				if (itr->guid == loser->GetGUID() && itr->accepted)
				{
					if (USE_TOKEN)
					{
						if (winner->GetItemCount(TOKEN_ID) < itr->amount)
						{
							winner->AddAura(15007, winner);		// Apply Rez sickness for possible cheating
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15029));
							ChatHandler(loser->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15030));
							RemoveBloodMoneyEntry(winner->GetGUID(), itr->guid);
							return;
						}
						if (loser->GetItemCount(TOKEN_ID) >= itr->amount)
						{
							if(loser->IsInWorld())
                            {
							winner->AddItem(TOKEN_ID, itr->amount);
							ChatHandler(winner->GetSession()).PSendSysMessage("|cff800C0C[] |cffFFFFFFCongratulations on winning %d tokens!", itr->amount);
							Item* item = loser->GetItemByEntry(TOKEN_ID);
							loser->DestroyItemCount(TOKEN_ID, itr->amount, true);
							RemoveBloodMoneyEntry(winner->GetGUID(), itr->guid);
							} else
                            ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15032));
						}
						else
						{
							loser->AddAura(15007, loser);		// Apply Rez sickness for possible cheating
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15029));
							ChatHandler(loser->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15030));
							RemoveBloodMoneyEntry(winner->GetGUID(), itr->guid);
						}
						return;
					}
					else
					{
						if (winner->GetMoney() < itr->amount)
						{   
							winner->AddAura(15007, winner);		// Apply Rez sickness for possible cheating
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15029));
							ChatHandler(loser->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15030));
							RemoveBloodMoneyEntry(winner->GetGUID(), itr->guid);
							return;
						}
						if (loser->GetMoney() >= itr->amount)
						{
                            if((loser->IsInWorld()) && (winner->IsInWorld()))
                            {
							winner->ModifyMoney(itr->amount);
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15031), itr->amount/10000);
							loser->ModifyMoney(-(int32)(itr->amount));
							RemoveBloodMoneyEntry(winner->GetGUID(), itr->guid);
                            }
                            else
                            ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15032));
						}
						else
						{
							loser->AddAura(15007, loser);		// Apply Rez sickness for possible cheating
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15030));
							ChatHandler(loser->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15029));
							RemoveBloodMoneyEntry(winner->GetGUID(), itr->guid);
						}
						return;
					}
				}
			}
			for (itr = list2.begin(); itr != list2.end(); ++itr)
			{
				if (itr->guid == winner->GetGUID() && itr->accepted)
				{
					if (USE_TOKEN)
					{
						if (winner->GetItemCount(TOKEN_ID) < itr->amount)
						{
							winner->AddAura(15007, winner);		// Apply Rez sickness for possible cheating
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15029));
							ChatHandler(loser->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15030));
							RemoveBloodMoneyEntry(loser->GetGUID(), itr->guid);
							return;
						}
						if (loser->GetItemCount(TOKEN_ID) >= itr->amount)
						{
							winner->AddItem(TOKEN_ID, itr->amount);
							ChatHandler(winner->GetSession()).PSendSysMessage("|cff800C0C[] |cffFFFFFFCongratulations on winning %d tokens!", itr->amount);
							Item* item = loser->GetItemByEntry(TOKEN_ID);
							loser->DestroyItemCount(TOKEN_ID, itr->amount, true);
							RemoveBloodMoneyEntry(loser->GetGUID(), itr->guid);
						}
						else
						{
							loser->AddAura(15007, loser);		// Apply Rez sickness for possible cheating
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15029));
							ChatHandler(loser->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15030));
							RemoveBloodMoneyEntry(loser->GetGUID(), itr->guid);
						}
						return;
					}
					else
					{
						if (winner->GetMoney() < itr->amount)
						{
							winner->AddAura(15007, winner);		// Apply Rez sickness for possible cheating
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15029));
							ChatHandler(loser->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15030));
							RemoveBloodMoneyEntry(loser->GetGUID(), itr->guid);
							return;
						}
						if (loser->GetMoney() >= itr->amount)
						{
                            if((loser->IsInWorld()) && (winner->IsInWorld()))
                            {
							winner->ModifyMoney(itr->amount);
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15031), itr->amount/10000);
							loser->ModifyMoney(-(int32)(itr->amount));
							RemoveBloodMoneyEntry(winner->GetGUID(), itr->guid);
                            }
                            else
                            ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15032));
						}
						else
						{
							loser->AddAura(15007, loser);		// Apply Rez sickness for possible cheating
							ChatHandler(winner->GetSession()).PSendSysMessage(winner->GetSession()->GetAcoreString(15030));
							ChatHandler(loser->GetSession()).PSendSysMessage(loser->GetSession()->GetAcoreString(15029));
							RemoveBloodMoneyEntry(loser->GetGUID(), itr->guid);
						}
						return;
					}
				}
			}

		 }
	 }
};

void AddSC_npc_blood_money()
{
	new BloodMoneyReward;
	new npc_blood_money;
}
