/*******************************************************************************************
* Créditos: Khanx                                                                          *
********************************************************************************************/
#include "ScriptPCH.h"

class Npc_vendor : public CreatureScript
{
public:
        Npc_vendor() : CreatureScript("Npc_vendor") { }
 
    bool OnGossipHello(Player* player, Creature* creature)
    {
		AddGossipItemFor(player,6, "Compra - 1", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
		AddGossipItemFor(player,6, "Compra - 2", GOSSIP_SENDER_MAIN, 1);
		AddGossipItemFor(player,6, "Compra - 3", GOSSIP_SENDER_MAIN, 2);
		AddGossipItemFor(player,6, "Compra - 4", GOSSIP_SENDER_MAIN, 4);
		AddGossipItemFor(player,6, "Compra - 5", GOSSIP_SENDER_MAIN, 5);
		SendGossipMenuFor(player,1, creature->GetGUID());
        return true;
    }
 
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
		player->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
			case 1:
				player->GetSession()->SendListInventory(creature->GetGUID(), 500101);
			break;
				
			case 2: //Este es el que funciona, requiere flags 128+7 en el npc
				player->GetSession()->SendListInventory(500101);
			break;
			
			case 4:
				player->GetSession()->SendListInventory(1337432, 500101);
			break;
			
			case 5:
				player->GetSession()->SendListInventory(1337432);
			break;
			
		}
        return true;
    }
};
 
void AddSC_Npc_vendor()
{
    new Npc_vendor();
}
