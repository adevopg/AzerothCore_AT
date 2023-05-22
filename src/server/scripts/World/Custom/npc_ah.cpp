/*******************************************************************************************
* Créditos: Khanx                                                                          *
********************************************************************************************/
#include "ScriptPCH.h"
#include "Unit.h"

enum Cambio
{
Honor1 = 7500,
Arenas1 = 250,
Honor2 = 5000,
Arenas2 = 250,
};

class Npc_ah : public CreatureScript
{
public:
        Npc_ah() : CreatureScript("Npc_ah") { }
 
    bool OnGossipHello(Player* player, Creature* creature)
    {
		AddGossipItemFor(player,1, "|cff00ff00|TInterface\\icons\\Achievement_arena_2v2_1:26:26:-15:0|t|r 7500 Honor -> 250 Arenas", GOSSIP_SENDER_MAIN, 2);
		AddGossipItemFor(player,1, "|cff00ff00|TInterface\\icons\\Achievement_arena_2v2_2:26:26:-15:0|t|r 250 Arenas -> 5000 Honor", GOSSIP_SENDER_MAIN, 3);
		SendGossipMenuFor(player,1, creature->GetGUID());
        return true;
    }
 
    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
	int MAXHONOR = sWorld->getIntConfig(CONFIG_MAX_HONOR_POINTS);
	int MAXARENAS = sWorld->getIntConfig(CONFIG_MAX_ARENA_POINTS);
	int honor = player->GetHonorPoints();
	int arenas = player->GetArenaPoints();
	player->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
        
		
		case 2:
		{
		if (arenas < MAXARENAS)
		{
			if (honor >= Honor1)
				{
					player->ModifyHonorPoints(-Honor1);
					player->ModifyArenaPoints(Arenas1);
					player->GetSession()->SendNotification("Intercambio realizado");
					OnGossipHello(player, creature);
				}
			else
				{
					creature->Whisper("No tienes los suficientes puntos de Honor.", LANG_UNIVERSAL, player);
					OnGossipHello(player, creature);
				}
		}
		else
			{
				creature->Whisper("Ya has alcanzado el máximo de puntos de Arenas.",LANG_UNIVERSAL, player);
				OnGossipHello(player, creature);
			}
		}break;
		
		case 3:
		{
		if (honor < MAXHONOR )
		{
		if (arenas>=Arenas2)
		{
		player->ModifyArenaPoints(-Arenas2);
		player->ModifyHonorPoints(Honor2);
		player->GetSession()->SendNotification("Intercambio realizado");
		OnGossipHello(player, creature);
		}
		else
		{
		creature->Whisper("No tienes los suficientes puntos de Arenas.", LANG_UNIVERSAL, player);
		OnGossipHello(player, creature);
		}
		}
		else
		{
		creature->Whisper("Ya has alcanzado el máximo de puntos de Honor.", LANG_UNIVERSAL, player);
		OnGossipHello(player, creature);
		}
		}
		break;

		}
        return true;
    }
};
 
void AddSC_Npc_ah()
{
    new Npc_ah();
}
