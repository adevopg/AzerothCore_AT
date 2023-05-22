#include "ScriptPCH.h"
 

#define MIN_GM_LEVEL 1
 
class GMItems : public PlayerScript
{
    public:
 
        GMItems() : PlayerScript("GM_Items"){}
                ObjectGuid playerGUID;
                void OnCreate(Player * player)
                {
                        playerGUID = player->GetGUID();
                        uint64 guidValue = playerGUID.GetRawValue();
                }
 
                void OnLogin(Player* player)
                {
                        if (playerGUID == player->GetGUID())
                        {
                             uint64 guidValue = playerGUID.GetRawValue();
                                QueryResult account = CharacterDatabase.Query("SELECT * FROM characters WHERE guid = '%u'", guidValue);
                                Field * accID = account->Fetch();
                                QueryResult gmaccount = LoginDatabase.Query("SELECT * FROM account_access WHERE id = '%u' AND gmlevel >= '%u'", accID[1].Get<int32>(), MIN_GM_LEVEL);
                                if (gmaccount)
                                {
									player->AddItem(10035, 1);
									player->AddItem(10036, 1);
									player->AddItem(10034, 1);
									player->AddItem(10026, 1);
									player->AddItem(10003, 1);
									player->AddItem(25681, 1);
                                    playerGUID = ObjectGuid::Empty;
                                }
                        }
                }
 
 
};
 
void AddSC_GMItems()
{
        new GMItems();
}
