/*******************************************************************************************
* 									Créditos: Ray                                  	       *
********************************************************************************************/

#include "ScriptPCH.h"
#include "../../Modules/mod-transmog/src/Transmogrification.h"
#define sT  sTransmogrification

class NPC_PvPIsland_Vendor : public CreatureScript
{
	public:
		NPC_PvPIsland_Vendor() : CreatureScript("NPC_PvPIsland_Vendor") { }

	
        bool OnGossipHello(Player* player, Creature* creature)
        {
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\INV_Misc_QuestionMark.png:26:26:-15:0|t|rInformación", GOSSIP_SENDER_MAIN, 22);			
			if(player->GetTeamId() == ALLIANCE)
				AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_featsofstrength_gladiator_08:26:26:-15:0|t|rTítulos", GOSSIP_SENDER_MAIN, 1);
			else
				AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_featsofstrength_gladiator_08:26:26:-15:0|t|rTítulos", GOSSIP_SENDER_MAIN, 2);
			//AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\Inv_gizmo_poltryiser_01:26:26:-15:0|t|rObjetos", GOSSIP_SENDER_MAIN, 3);
			//AddGossipItemFor(player,7, "Olv""\xC3\xAD""dalo", GOSSIP_SENDER_MAIN, 4);
			player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
            return true;
		}

		bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
			
			switch (action)
            {				
				case 5:
					OnGossipHello(player, creature);
                break;
				
				case 4:
					CloseGossipMenuFor(player);
				break;
				
				case 22: // Información			
					AddGossipItemFor(player,4, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:26:26:-15:0|tVolver al Menú Principal", GOSSIP_SENDER_MAIN, 5);
					AddGossipItemFor(player,4, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:26:26:-15:0|tOlvídalo", GOSSIP_SENDER_MAIN, 4);
					player->PlayerTalkClass->SendGossipMenu(6, creature->GetGUID());
				break;
				
				// Menu Ali Titles
                case 1:
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_01:26:26:-15:0|t|r|CFF0000FFSoldado|R", GOSSIP_SENDER_MAIN, 6, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFASoldado|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -1)+sT->GetItemLink(32569, player->GetSession())+" x40", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_02:26:26:-15:0|t|r|CFF0000FFCabo|R", GOSSIP_SENDER_MAIN, 7, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFACabo|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x60\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_01:26:26:-5:-6|t|r|CFF0066CCSoldado|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_03:26:26:-15:0|t|r|CFF0000FFSargento|R", GOSSIP_SENDER_MAIN, 8, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFASargento|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x70\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_02:26:26:-5:-6|t|r|CFF0066CCCabo|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_04:26:26:-15:0|t|r|CFF0000FFSargento primero|R", GOSSIP_SENDER_MAIN, 9, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFASargento primero|CFFFFFF01?|R\n\n"+sT->GetItemIcon(15704, 28, 28, -3, -6)+sT->GetItemLink(15704, player->GetSession())+" x3\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_03:26:26:-5:-6|t|r|CFF0066CCSargento|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_10:26:26:-15:0|t|r|CFF0000FFTeniente coronel|R", GOSSIP_SENDER_MAIN, 10, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFATeniente coronel|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x260\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_04:26:26:-5:-6|t|r|CFF0066CCSargento primero|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_11:26:26:-15:0|t|r|CFF0000FFComandante|R", GOSSIP_SENDER_MAIN, 11, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAComandante|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x280\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_10:26:26:-5:-6|t|r|CFF0066CCTeniente coronel|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_12:26:26:-15:0|t|r|CFF0000FFMariscal|R", GOSSIP_SENDER_MAIN, 12, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAMariscal|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x480\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_a_11:26:26:-5:-6|t|r|CFF0066CCComandante|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_a_14:26:26:-15:0|t|r|CFF0000FFGran Mariscal|R", GOSSIP_SENDER_MAIN, 13, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAGran mariscal|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -13)+sT->GetItemLink(32569, player->GetSession())+" x720\n\n+\n\n"+sT->GetItemIcon(15704, 28, 28, -3, -13)+sT->GetItemLink(15704, player->GetSession())+" x25\n\n+\n\n"+"|cff00ff00|TInterface\\icons\\achievement_pvp_a_13:26:26:-5:-13|t|r|CFF0066CCMariscal|R", 0, false);
					//AddGossipItemFor(player,2, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 5);
					AddGossipItemFor(player,4, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:26:26:-15:0|tVolver al Menú principal", GOSSIP_SENDER_MAIN, 5);
					AddGossipItemFor(player,4, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:26:26:-15:0|tOlvídalo", GOSSIP_SENDER_MAIN, 4);
					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
                break;
				
				// Menu Horda Titles
				case 2:
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_01:26:26:-15:0|t|r|CFF0000FFRastreador|R", GOSSIP_SENDER_MAIN, 14, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFARastreador|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -1)+sT->GetItemLink(32569, player->GetSession())+" x40", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_02:26:26:-15:0|t|r|CFF0000FFBruto|R", GOSSIP_SENDER_MAIN, 15, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFABruto|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x60\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_01:26:26:-5:-6|t|r|CFF0066CCRastreador|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_03:26:26:-15:0|t|r|CFF0000FFCapataz|R", GOSSIP_SENDER_MAIN, 16, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFACapataz|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x60\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_02:26:26:-5:-6|t|r|CFF0066CCBruto|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_04:26:26:-15:0|t|r|CFF0000FFCapataz primero|R", GOSSIP_SENDER_MAIN, 17, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFACapataz primero|CFFFFFF01?|R\n\n"+sT->GetItemIcon(15704, 28, 28, -3, -6)+sT->GetItemLink(15704, player->GetSession())+" x3\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_03:26:26:-5:-6|t|r|CFF0066CCCapataz|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_10:26:26:-15:0|t|r|CFF0000FFCampeón|R", GOSSIP_SENDER_MAIN, 18, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFACampeón|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x260\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_04:26:26:-5:-6|t|r|CFF0066CCCapataz primero|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_11:26:26:-15:0|t|r|CFF0000FFGeneral|R", GOSSIP_SENDER_MAIN, 19, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAGeneral|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x280\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_10:26:26:-5:-6|t|r|CFF0066CCCampeón|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_13:26:26:-15:0|t|r|CFF0000FFSe""\xC3\xB1""or de la Guerra|R", GOSSIP_SENDER_MAIN, 20, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFASe""\xC3\xB1""or de la Guerra|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -6)+sT->GetItemLink(32569, player->GetSession())+" x480\n\n+\n\n|cff00ff00|TInterface\\icons\\achievement_pvp_h_11:26:26:-5:-6|t|r|CFF0066CCGeneral|R", 0, false);
					AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\achievement_pvp_h_14:26:26:-15:0|t|r|CFF0000FFGran Se""\xC3\xB1""or de la Guerra|R", GOSSIP_SENDER_MAIN, 21, "|CFFFFFF01""\xc2\xbf""Deseas comprar el título |CFF87CEFAGran Se""\xC3\xB1""or de la Guerra|CFFFFFF01?|R\n\n"+sT->GetItemIcon(32569, 28, 28, -3, -13)+sT->GetItemLink(32569, player->GetSession())+" x720\n\n+\n\n"+sT->GetItemIcon(15704, 28, 28, -3, -13)+sT->GetItemLink(15704, player->GetSession())+" x25\n\n+\n\n"+"|cff00ff00|TInterface\\icons\\achievement_pvp_h_13:26:26:-5:-13|t|r|CFF0066CCSe""\xC3\xB1""or de la Guerra|R", 0, false);
					//AddGossipItemFor(player,2, "<-- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, 5);
					AddGossipItemFor(player,4, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:26:26:-15:0|tVolver al Menú principal", GOSSIP_SENDER_MAIN, 5);
					AddGossipItemFor(player,4, "|TInterface/PaperDollInfoFrame/UI-GearManager-LeaveItem-Opaque:26:26:-15:0|tOlvídalo", GOSSIP_SENDER_MAIN, 4);
					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
                break;
				
				/////////////////////////////////////////////////////////// MENU ALIANZA //////////////////////////////////////////////////////////////////////
				case 6: // Soldado Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(1);
					if (player->HasTitle(2) || player->HasTitle(3) || player->HasTitle(4) || player->HasTitle(10) || player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
						else
						{
							if (player->HasItemCount(32569, 40))
							{
								player->DestroyItemCount(32569, 40, true, false);
								player->SetTitle(titleadditeado);
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
								OnGossipSelect(player, creature, 0, 1);
							}
							else
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 40.|R");
								OnGossipSelect(player, creature, 0, 1);
							}
						}
					}
				}
				break;
				
				case 7: // Cabo Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(2);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(1);
					if (player->HasTitle(3) || player->HasTitle(4) || player->HasTitle(10) || player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
						else
						{
							if (!player->HasItemCount(32569, 60))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 60.|R");
								OnGossipSelect(player, creature, 0, 1);
							}

							if (!player->HasTitle(1))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCSoldado|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 1);
							}
						}
						if (player->HasItemCount(32569, 60) && player->HasTitle(1))
						{
							player->DestroyItemCount(32569, 60, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
					}
				}
				break;
				
				case 8: // Sargento Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(3);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(2);
					if (player->HasTitle(4) || player->HasTitle(10) || player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
						else
						{
							if (!player->HasItemCount(32569, 70))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 70.|R");
								OnGossipSelect(player, creature, 0, 1);
							}

							if (!player->HasTitle(2))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCCabo|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 1);
							}
						}
						
						if (player->HasItemCount(32569, 70) && player->HasTitle(2))
						{
							player->DestroyItemCount(32569, 70, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
					}
				}
				break;
				
				case 9: // Sargento primero Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(4);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(3);
					if (player->HasTitle(10) || player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
						else
						{
							if (!player->HasItemCount(15704, 3))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Insignia del Valeroso]|CFFFF0000, necesitas al menos 3.|R");
								OnGossipSelect(player, creature, 0, 1);
							}

							if (!player->HasTitle(3))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCSargento|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 1);
							}
						}
						
						if (player->HasItemCount(15704, 3) && player->HasTitle(3))
						{
							player->DestroyItemCount(15704, 3, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
					}
				}
				break;
				
				case 10: // Teniente Coronel Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(10);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(4);
					if (player->HasTitle(11) || player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
						else
						{
							if (!player->HasItemCount(32569, 260))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 260.|R");
								OnGossipSelect(player, creature, 0, 1);
							}

							if (!player->HasTitle(4))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCSargento primero|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 1);
							}
						}
						
						if (player->HasItemCount(32569, 260) && player->HasTitle(4))
						{
							player->DestroyItemCount(32569, 260, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
					}
				}
				break;

				case 11: // Comandante Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(11);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(10);
					if (player->HasTitle(12) || player->HasTitle(14))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
						else
						{
							if (!player->HasItemCount(32569, 280))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 280.|R");
								OnGossipSelect(player, creature, 0, 1);
							}

							if (!player->HasTitle(10))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCTeniente coronel|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 1);
							}
						}
						
						if (player->HasItemCount(32569, 280) && player->HasTitle(10))
						{
							player->DestroyItemCount(32569, 280, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
					}
				}
				break;

				case 12: // Mariscal Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(12);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(11);
					if (player->HasTitle(14))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
						else
						{
							if (!player->HasItemCount(32569, 480))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 480.|R");
								OnGossipSelect(player, creature, 0, 1);
							}

							if (!player->HasTitle(11))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCComandante|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 1);
							}
						}
						
						if (player->HasItemCount(32569, 480) && player->HasTitle(11))
						{
							player->DestroyItemCount(32569, 480, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
					}
				}
				break;

				case 13: // Gran Mariscal Ali
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(13);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(11);
					if (player->HasTitle(titleadditeado))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
					else
					{
						if (!player->HasItemCount(32569, 720))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 720.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
						
						if (!player->HasItemCount(15704, 25))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Insignia del Valeroso]|CFFFF0000, necesitas al menos 25.|R");
							OnGossipSelect(player, creature, 0, 1);
						}

						if (!player->HasTitle(11))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCMariscal|R|CFFFF0000.|R");
							OnGossipSelect(player, creature, 0, 1);
						}
					}
						
					if (player->HasItemCount(32569, 720) && player->HasItemCount(15704, 25) && player->HasTitle(11))
					{
						player->DestroyItemCount(32569, 720, true, false);
						player->DestroyItemCount(15704, 25, true, false);
						player->SetTitle(titleborrado, true);
						player->SetTitle(titleadditeado);
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
						OnGossipSelect(player, creature, 0, 1);
					}
				}
				break;
				
				/////////////////////////////////////////////////////////// MENU HORDA //////////////////////////////////////////////////////////////////////
				case 14: // Rastreador Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(15);
					if (player->HasTitle(16) || player->HasTitle(17) || player->HasTitle(18) || player->HasTitle(24) || player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
						else
						{
							if (player->HasItemCount(32569, 40))
							{
								player->DestroyItemCount(32569, 40, true, false);
								player->SetTitle(titleadditeado);
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
								OnGossipSelect(player, creature, 0, 2);
							}
							else
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 40.|R");
								OnGossipSelect(player, creature, 0, 2);
							}
						}
					}
				}
				break;

				case 15: // Bruto Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(16);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(15);
					if (player->HasTitle(17) || player->HasTitle(18) || player->HasTitle(24) || player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
						else
						{
							if (!player->HasItemCount(32569, 60))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 60.|R");
								OnGossipSelect(player, creature, 0, 2);
							}

							if (!player->HasTitle(15))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCRastreador|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 2);
							}
						}
						if (player->HasItemCount(32569, 60) && player->HasTitle(15))
						{
							player->DestroyItemCount(32569, 60, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
					}
				}
				break;
				
				case 16: // Capataz Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(17);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(16);
					if (player->HasTitle(18) || player->HasTitle(24) || player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
						else
						{
							if (!player->HasItemCount(32569, 70))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 70.|R");
								OnGossipSelect(player, creature, 0, 2);
							}

							if (!player->HasTitle(16))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCBruto|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 2);
							}
						}
						
						if (player->HasItemCount(32569, 70) && player->HasTitle(16))
						{
							player->DestroyItemCount(32569, 70, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
					}
				}
				break;
				
				case 17: // Capataz primero Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(18);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(17);
					if (player->HasTitle(24) || player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
						else
						{
							if (!player->HasItemCount(15704, 3))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Insignia del Valeroso]|CFFFF0000, necesitas al menos 3.|R");
								OnGossipSelect(player, creature, 0, 2);
							}

							if (!player->HasTitle(17))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCCapataz|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 2);
							}
						}
						
						if (player->HasItemCount(15704, 3) && player->HasTitle(17))
						{
							player->DestroyItemCount(15704, 3, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
					}
				}
				break;
				
				case 18: // Campeón Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(24);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(18);
					if (player->HasTitle(25) || player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
						else
						{
							if (!player->HasItemCount(32569, 260))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 260.|R");
								OnGossipSelect(player, creature, 0, 2);
							}

							if (!player->HasTitle(18))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCCapataz primero|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 2);
							}
						}
						
						if (player->HasItemCount(32569, 260) && player->HasTitle(18))
						{
							player->DestroyItemCount(32569, 260, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
					}
				}
				break;
				
				case 19: // General Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(25);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(24);
					if (player->HasTitle(27) || player->HasTitle(28))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
						else
						{
							if (!player->HasItemCount(32569, 280))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 280.|R");
								OnGossipSelect(player, creature, 0, 2);
							}

							if (!player->HasTitle(24))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCCampeón|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 2);
							}
						}
						
						if (player->HasItemCount(32569, 280) && player->HasTitle(24))
						{
							player->DestroyItemCount(32569, 280, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
					}
				}
				break;
				
				case 20: // Señor de la guerra
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(27);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(25);
					if (player->HasTitle(28))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de un título de mayor categoría.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
					else
					{
						if (player->HasTitle(titleadditeado))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
						else
						{
							if (!player->HasItemCount(32569, 480))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 480.|R");
								OnGossipSelect(player, creature, 0, 2);
							}

							if (!player->HasTitle(25))
							{
								ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCGeneral|R|CFFFF0000.|R");
								OnGossipSelect(player, creature, 0, 2);
							}
						}
						
						if (player->HasItemCount(32569, 480) && player->HasTitle(25))
						{
							player->DestroyItemCount(32569, 480, true, false);
							player->SetTitle(titleborrado, true);
							player->SetTitle(titleadditeado);
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
					}
				}
				break;
				
				case 21: // Gran Señor de la Guerra Horda
				{
					CharTitlesEntry const* titleadditeado = sCharTitlesStore.LookupEntry(28);
					CharTitlesEntry const* titleborrado = sCharTitlesStore.LookupEntry(27);
					if (player->HasTitle(titleadditeado))
					{
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Ya dispones de ese título.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
					else
					{
						if (!player->HasItemCount(32569, 720))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Token Zona PvP]|CFFFF0000, necesitas al menos 720.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
						
						if (!player->HasItemCount(15704, 25))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes suficentes |CFFff8000[Insignia del Valeroso]|CFFFF0000, necesitas al menos 25.|R");
							OnGossipSelect(player, creature, 0, 2);
						}

						if (!player->HasTitle(27))
						{
							ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000No tienes el título de |CFF0066CCSe""\xC3\xB1""or de la Guerra|R|CFFFF0000.|R");
							OnGossipSelect(player, creature, 0, 2);
						}
					}
						
					if (player->HasItemCount(32569, 720) && player->HasItemCount(15704, 25) && player->HasTitle(27))
					{
						player->DestroyItemCount(32569, 720, true, false);
						player->DestroyItemCount(15704, 25, true, false);
						player->SetTitle(titleborrado, true);
						player->SetTitle(titleadditeado);
						ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Título añadido correctamente.|R");
						OnGossipSelect(player, creature, 0, 2);
					}
				}
				break;
			}
			return true;
		}
};

void AddSC_NPC_PvPIsland_Vendor()
{
    new NPC_PvPIsland_Vendor();
}
