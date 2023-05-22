#include "ScriptPCH.h"

class libro_comandos : public ItemScript
{
public:
libro_comandos() : ItemScript ("libro_comandos") {} 

	bool OnUse(Player * player, Item * item, SpellCastTargets const& targets)
	{
		player->PlayerTalkClass->ClearMenus();
		
		AddGossipItemFor(player, 3, "Comando: .arena", GOSSIP_SENDER_MAIN, 1);
		AddGossipItemFor(player,3, "Comando: .challenge", GOSSIP_SENDER_MAIN, 2);
		AddGossipItemFor(player,3, "Comando: .utility", GOSSIP_SENDER_MAIN, 3);
		SendGossipMenuFor(player,7, item->GetGUID());
		return true;
	}
	
	void OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		
		switch(action)
		{
			case 0: // Menú principal
			{
				player->PlayerTalkClass->ClearMenus();
				
				AddGossipItemFor(player,3, "Comando: .arena", GOSSIP_SENDER_MAIN, 1);
				AddGossipItemFor(player,3, "Comando: .challenge", GOSSIP_SENDER_MAIN, 2);
				AddGossipItemFor(player,3, "Comando: .utility", GOSSIP_SENDER_MAIN, 3);
				SendGossipMenuFor(player, 7, item->GetGUID());
				break;
			}
			
			/************************************ COMANDO .ARENA - INICIO **********************************/
			
			case 1: // Comando: .arena
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,3, "Comando: .arena 2v2", GOSSIP_SENDER_MAIN, 4);
				AddGossipItemFor(player,3, "Comando: .arena 3v3", GOSSIP_SENDER_MAIN, 5);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,8, item->GetGUID());
				break;
			} break;
			
			case 4: // Comando: .arena 2v2
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 1);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,9, item->GetGUID());
				break;
			} break;
			
			case 5: // Comando: .arena 3v3
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 1);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,10, item->GetGUID());
				break;
			} break;
			
			/************************************ COMANDO .ARENA - FIN *************************************/


			/************************************ COMANDO .CHALLENGE - INICIO ******************************/
			
			case 2: // Comando: .challenge
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,3, "Comando: .challenge modo", GOSSIP_SENDER_MAIN, 6);
				AddGossipItemFor(player,3, "Comando: .challenge 1c1 nombre_rival", GOSSIP_SENDER_MAIN, 7);
				AddGossipItemFor(player,3, "Comando: .challenge 2c2 nombre_rival1 nombre_rival2", GOSSIP_SENDER_MAIN, 8);
				AddGossipItemFor(player,3, "Comando: .challenge 3c3 nombre_rival1 nombre_rival2 nombre_rival3", GOSSIP_SENDER_MAIN, 9);
				AddGossipItemFor(player,3, "Comando: .challenge aceptar", GOSSIP_SENDER_MAIN, 10);
				AddGossipItemFor(player,3, "Comando: .challenge on", GOSSIP_SENDER_MAIN, 11);
				AddGossipItemFor(player,3, "Comando: .challenge off", GOSSIP_SENDER_MAIN, 12);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,8, item->GetGUID());
				break;
			} break;
			
			case 6: // Comando: .challenge modo
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,11, item->GetGUID());
				break;
			} break;
			
			case 7: // Comando: .challenge 1c1 nombre_rival
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,12, item->GetGUID());
				break;
			} break;
			
			case 8: // Comando: .challenge 2c2 nombre_rival1 nombre_rival2
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,13, item->GetGUID());
				break;
			} break;
			
			case 9: // Comando: .challenge 3c3 nombre_rival1 nombre_rival2 nombre_rival3
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,14, item->GetGUID());
				break;
			} break;
			
			case 10: // Comando: .challenge aceptar
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,15, item->GetGUID());
				break;
			} break;
			
			case 11: // Comando: .challenge on
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,16, item->GetGUID());
				break;
			} break;
			
			case 12: // Comando: .challenge off
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 2);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,17, item->GetGUID());
				break;
			} break;
			
			/************************************ COMANDO .CHALLENGE - FIN *********************************/
			
			/************************************ COMANDO .UTILITY - INICIO ********************************/
			
			case 3: // Comando: .utility
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,3, "Comando: .utility race", GOSSIP_SENDER_MAIN, 13);
				AddGossipItemFor(player,3, "Comando: .utility faction", GOSSIP_SENDER_MAIN, 14);
				AddGossipItemFor(player,3, "Comando: .utility customize", GOSSIP_SENDER_MAIN, 15);
				AddGossipItemFor(player,3, "Comando: .utility mmr", GOSSIP_SENDER_MAIN, 16);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,8, item->GetGUID());
				break;
			} break;
			
			case 13: // Comando: .utility race
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 3);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,18, item->GetGUID());
				break;
			} break;
			
			case 14: // Comando: .utility faction
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 3);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,19, item->GetGUID());
				break;
			} break;
			
			case 15: // Comando: .utility customize
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 3);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,20, item->GetGUID());
				break;
			} break;
			
			case 16: // Comando: .utility mmr
			{
				player->PlayerTalkClass->ClearMenus();

				AddGossipItemFor(player,2, "<- Volver", GOSSIP_SENDER_MAIN, 3);
				AddGossipItemFor(player,2, "<-- Volver al Menú Principal", GOSSIP_SENDER_MAIN, 0);
				SendGossipMenuFor(player,21, item->GetGUID());
				break;
			} break;
			
			/************************************ COMANDO .UTILITY - FIN ***********************************/
		}
	}
};

void AddSC_libro_comandos()
{
	new libro_comandos();
}
