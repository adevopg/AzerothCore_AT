/*******************************************************************************************
* 									Créditos: Ray                                  	       *
********************************************************************************************/

#include "ScriptPCH.h"

enum Monturas
{
    MONTURA_LOQUE    	= 	34897,
	AGUIJONEGRO		 	=	34899,
	ALFOMBRA_TERRESTRE 	=	39316,
	AVISPA_DE_MONTAR	=	34896,
	SKOLL				=	34898,
	CORCEL_CELESTIAL	=	75620,
	INVENCIBLE			=	72282,
	LICH_KING           =   18991,
	TIGRE_JADE          =   18992,
	GARGOLA             =   23249,
	BARCO_TAUREN        =   34790,
	POLLO               =   22718,
	TIGRE_ESPECTRAL     =   39315,
	AGUIJOROSA          =   39317,
	TIGRE_GONDRIA       =   39318,
	ELEFANTE_ROSA       =   39319,
	MANTA_AZUL          =   65641
};

class probador_monturas_custom : public CreatureScript
{

private: 
void probar_montura(uint32 montura, Player* player)
{
	//if(!(player->HasAura(MONTURA_LOQUE) || player->HasAura(AGUIJONEGRO) || player->HasAura(ALFOMBRA_TERRESTRE) || player->HasAura(AVISPA_DE_MONTAR) || player->HasAura(SKOLL) || player->HasAura(CORCEL_CELESTIAL) || player->HasAura(INVENCIBLE)))
	if(!player->IsMounted())
	{
		Aura* aura = player->AddAura(montura, player);
		if(player->HasAura(montura))
			aura->SetDuration(10*IN_MILLISECONDS);
		player->GetSession()->SendAreaTriggerMessage("Listo, puedes probar tu montura. Recuerda que solo dispones de 10 segundos para probarla.");
		CloseGossipMenuFor(player);
	}
	else
	{
		player->GetSession()->SendNotification("|cff87CEFADebes bajar de tu montura.");
		CloseGossipMenuFor(player);
	}
}

public:
    probador_monturas_custom() : CreatureScript("probador_monturas_custom") { }
 
        bool OnGossipHello(Player* player, Creature* creature)
        {
			player->PlayerTalkClass->ClearMenus();
			AddGossipItemFor(player,3, "|TInterface\\icons\\inv_misc_head_tiger_01:26:26:-15:0|t|r Loque'nahak", GOSSIP_SENDER_MAIN, 1);
			AddGossipItemFor(player,3, "|TInterface\\icons\\inv_misc_monsterfang_01:26:26:-15:0|t|r Aguijonegro", GOSSIP_SENDER_MAIN, 2);
			AddGossipItemFor(player,3, "|TInterface\\icons\\ability_mount_flyingcarpet:26:26:-15:0|t|r Alfombra Terrestre", GOSSIP_SENDER_MAIN, 3);
			AddGossipItemFor(player,3, "|TInterface\\icons\\inv_misc_orb_05:26:26:-15:0|t|r Avispa de Montar", GOSSIP_SENDER_MAIN, 4);
			AddGossipItemFor(player,3, "|TInterface\\icons\\ability_mount_blackdirewolf:26:26:-15:0|t|r Skoll", GOSSIP_SENDER_MAIN, 5);
			AddGossipItemFor(player,3, "|TInterface\\icons\\ability_mount_celestialhorse:26:26:-15:0|t|r Corcel Celestial", GOSSIP_SENDER_MAIN, 6);
			AddGossipItemFor(player,3, "|TInterface\\icons\\spell_deathknight_summondeathcharger:26:26:-15:0|t|r Invencible", GOSSIP_SENDER_MAIN, 7);
			AddGossipItemFor(player,3, "|TInterface\\icons\\achievement_boss_lichking:26:26:-15:0|t|r Lich King", GOSSIP_SENDER_MAIN, 8);
			AddGossipItemFor(player,3, "|TInterface\\icons\\ability_mount_blackpanther:26:26:-15:0|t|r Tigre de Jade", GOSSIP_SENDER_MAIN, 9);
			AddGossipItemFor(player,2, "Siguiente p""\xC3\xA1""gina -->", GOSSIP_SENDER_MAIN, 19);
			AddGossipItemFor(player,7, "Olv""\xC3\xAD""dalo", GOSSIP_SENDER_MAIN, 18);
			player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
            return true;
		}

		bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

			switch (action)
            {
                case 1: // Loque'nahak
                    probar_montura(MONTURA_LOQUE, player);
                break;
				
				case 2: // Aguijonegro
					probar_montura(AGUIJONEGRO, player);
                break;

				case 3: // Alfombra Terrestre
					probar_montura(ALFOMBRA_TERRESTRE, player);
				break;				

				case 4: // Avispa de Montar
					probar_montura(AVISPA_DE_MONTAR, player);
                break;				
				
				case 5: // Skoll
					probar_montura(SKOLL, player);
                break;	

				case 6: // Corcel Celestial
					probar_montura(CORCEL_CELESTIAL, player);
                break;	
				
				case 7: // Invencible
					probar_montura(INVENCIBLE, player);
                break;	
				
				case 8: // Lich King
					probar_montura(LICH_KING, player);
                break;	
				
				case 9: // Tigre de jade
					probar_montura(TIGRE_JADE, player);
                break;	
				
				case 10: // Gargola
					probar_montura(GARGOLA, player);
                break;	
				
				case 11: // Barco Tauren
					probar_montura(BARCO_TAUREN, player);
                break;	
				
				case 12: // Pollo
					probar_montura(POLLO, player);
                break;	
				
				case 13: // Tigre de Juguete
					probar_montura(TIGRE_ESPECTRAL, player);
                break;	
				
				case 14: // Aguijorosa
					probar_montura(AGUIJOROSA, player);
                break;	
				
				case 15: // Tigre Gondria
					probar_montura(TIGRE_GONDRIA, player);
                break;	
				
				case 16: // Elefante Rosa
					probar_montura(ELEFANTE_ROSA, player);
                break;	
				
				case 17: // Manta azul
					probar_montura(MANTA_AZUL, player);
                break;	
				
				case 18: // Menu olvidalo
					CloseGossipMenuFor(player);
				break;
				
				case 19: // Siguiente Página
				{
					AddGossipItemFor(player,3, "|TInterface\\icons\\ability_hunter_pet_bat:26:26:-15:0|t|r G""\xC3\xA1""rgola", GOSSIP_SENDER_MAIN, 10);
					AddGossipItemFor(player,3, "|TInterface\\icons\\inv_misc_head_tauren_01:26:26:-15:0|t|r Barco Tauren", GOSSIP_SENDER_MAIN, 11);
					AddGossipItemFor(player,3, "|TInterface\\icons\\achievement_boss_xt002deconstructor_01:26:26:-15:0|t|r Robot Mec""\xC3\xA1""nico", GOSSIP_SENDER_MAIN, 12);
					AddGossipItemFor(player,3, "|TInterface\\icons\\inv_misc_sandbox_spectraltiger_01:26:26:-15:0|t|r Tigre de Juguete", GOSSIP_SENDER_MAIN, 13);
					AddGossipItemFor(player,3, "|TInterface\\icons\\inv_misc_gem_pearl_05:26:26:-15:0|t|r Aguijorosa", GOSSIP_SENDER_MAIN, 14);
					AddGossipItemFor(player,3, "|TInterface\\icons\\ability_mount_spectraltiger:26:26:-15:0|t|r Tigre Gondria", GOSSIP_SENDER_MAIN, 15);
					AddGossipItemFor(player,3, "|TInterface\\icons\\ability_mount_ridingelekk_purple:26:26:-15:0|t|r Elefante Rosa", GOSSIP_SENDER_MAIN, 16);
					AddGossipItemFor(player,3, "|TInterface\\icons\\Ability_Hunter_Pet_NetherRay:26:26:-15:0|t|r Manta Azul", GOSSIP_SENDER_MAIN, 17);
					AddGossipItemFor(player,2, "<- P""\xC3\xA1""gina Anterior", GOSSIP_SENDER_MAIN, 20);
					AddGossipItemFor(player,7, "Olv""\xC3\xAD""dalo", GOSSIP_SENDER_MAIN, 18);
					player->PlayerTalkClass->SendGossipMenu(1, creature->GetGUID());
					return true;
				}
				break;
				
				case 20: // Página Anterior - Menú Principal
					OnGossipHello(player, creature);
				break;
			}
			return true;
		}
};
                           
void AddSC_probador_monturas_custom()
{
    new probador_monturas_custom();
}
