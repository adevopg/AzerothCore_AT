//Modificado por Raypaladin
#include "ScriptPCH.h"
#include "Pet.h"

#define GOSSIP_ITEM_STABLE "Stable"
#define GOSSIP_ITEM_NEWPET "New Pet"
#define GOSSIP_ITEM_BOAR "Boar"
#define GOSSIP_ITEM_SERPENT "Serpent"
#define GOSSIP_ITEM_SCRAB "Scrab"
#define GOSSIP_ITEM_LION "Lion"
#define GOSSIP_ITEM_WOLF "Wolf"
#define GOSSIP_ITEM_RAVAGER "Ravenger"
 
#define GOSSIP_ITEM_UNTRAINEPET "Restart Pet"

class Npc_Beastmaster : public CreatureScript
{
public:
        Npc_Beastmaster() : CreatureScript("Npc_Beastmaster") { }

	void CreatePet(Player *player, Creature * m_creature, uint32 entry) 
	{
        if(player->getClass() != CLASS_HUNTER) {
            player->GetSession()->SendAreaTriggerMessage("""\xc2\xa1""No eres un cazador!", player->GetGUID());
            player->PlayerTalkClass->SendCloseGossip();
            return;
        }
 
        if(player->GetPet()) {
            player->GetSession()->SendAreaTriggerMessage("""\xc2\xa1""Primero debes retirar a tu mascota!", player->GetGUID());
            player->PlayerTalkClass->SendCloseGossip();
            return;
        }
 
        Creature *creatureTarget = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
        if(!creatureTarget) return;
        
        Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
        if(!pet) return;
 
        // Matamos a la criatura original
        creatureTarget->setDeathState(JUST_DIED);
        creatureTarget->RemoveCorpse();
        creatureTarget->SetHealth(0);                       
 
        pet->SetPower(POWER_HAPPINESS, 1048000);
 
        //pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE,0);
        //pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, uint32((Trinity::XP::xp_to_level(70))/4));
 
        // Preparamos el efecto visual para la subida de nivel
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel() - 1);
		//pet->GetMap()->Add((Creature*)pet);//TRINITY
		pet->GetMap()->AddToMap((Creature*)pet);//MYTH
        // Efecto visual para la subida de nivel
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());
 
        
        if(!pet->InitStatsForLevel(player->getLevel()))
			//sLog->outError (LOG_FILTER_GENERAL,"Fallo en la creacion de la mascota: no existen estadísticas para el entry %u", entry);//TRINITY
			LOG_ERROR("server","Fallo en la creacion de la mascota: no existen estadisticas para el entry %u", entry);//MYTH
 
        pet->UpdateAllStats();
        
        // El usuario ya tiene la mascota
        player->SetMinion(pet, true);
 
        pet->SavePetToDB(PET_SAVE_AS_CURRENT);
        pet->InitTalentForLevel();
        player->PetSpellInitialize();
        
        //Finalizado el proceso y la mascota ya ha sido entregada
        player->PlayerTalkClass->SendCloseGossip();
        m_creature->Whisper("Ya tienes a tu mascota. Es posible que quieras cambiarle el nombre o darle de comer.",LANG_UNIVERSAL, player);
	}
 
 
    bool OnGossipHello(Player *player, Creature * m_creature)
    {
 
        if(player->getClass() != CLASS_HUNTER)
        {
            player->GetSession()->SendAreaTriggerMessage("""\xc2\xa1""No eres un cazador!", player->GetGUID());
            return true;
        }
        AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\Inv_box_petcarrier_01:26:26:-15:0|t|r Obtener una mascota.", GOSSIP_SENDER_MAIN, 30);
        if (player->CanTameExoticPets())
        {
            AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\Inv_box_birdcage_01:26:26:-15:0|t|r Obtener una mascota exotica.", GOSSIP_SENDER_MAIN, 50);
        }
        AddGossipItemFor(player,2, "|cff00ff00|TInterface\\icons\\Ability_hunter_mendpet:26:26:-15:0|t|r Ir al establo.", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
                AddGossipItemFor(player,5, "Olvidalo.", GOSSIP_SENDER_MAIN, 150);
        SendGossipMenuFor(player,1, m_creature->GetGUID());
        return true;
    }
 
    bool OnGossipSelect(Player *player, Creature * m_creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
        case 100:
            AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\Inv_box_petcarrier_01:26:26:-15:0|t|r Obtener una mascota.", GOSSIP_SENDER_MAIN, 30);
            if (player->CanTameExoticPets())
            {
                AddGossipItemFor(player,4, "|cff00ff00|TInterface\\icons\\Inv_box_birdcage_01:26:26:-15:0|t|r Obtener una mascota exotica.", GOSSIP_SENDER_MAIN, 50);
            }
            AddGossipItemFor(player,2, "|cff00ff00|TInterface\\icons\\Ability_hunter_mendpet:26:26:-15:0|t|r Ir al establo.", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
                        AddGossipItemFor(player,5, "Olvidalo.", GOSSIP_SENDER_MAIN, 150);
            SendGossipMenuFor(player, 1, m_creature->GetGUID());
                break;

                case 150:
                        CloseGossipMenuFor(player);
                break;
        
        case 30:
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_bat:26:26:-15:0|t|r Murci""\xC3\xA9""lago.", GOSSIP_SENDER_MAIN, 18);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_bear:26:26:-15:0|t|r Oso.", GOSSIP_SENDER_MAIN, 1);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_boar:26:26:-15:0|t|r Jabal""\xC3\xAD"".", GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_cat:26:26:-15:0|t|r Felino.", GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_vulture:26:26:-15:0|t|r Buitre.", GOSSIP_SENDER_MAIN, 5);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_crab:26:26:-15:0|t|r Cangrejo.", GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_crocolisk:26:26:-15:0|t|r Cocodrilo.", GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_dragonhawk:26:26:-15:0|t|r Dracohalc""\xC3\xB3""n.", GOSSIP_SENDER_MAIN, 17);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_gorilla:26:26:-15:0|t|r Gorila.", GOSSIP_SENDER_MAIN, 8);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_wolf:26:26:-15:0|t|r Lobo.", GOSSIP_SENDER_MAIN, 9);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_hyena:26:26:-15:0|t|r Hiena.", GOSSIP_SENDER_MAIN, 10);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_moth:26:26:-15:0|t|r Polilla.", GOSSIP_SENDER_MAIN, 11);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_owl:26:26:-15:0|t|r Ave Rapaz.", GOSSIP_SENDER_MAIN, 12);
            AddGossipItemFor(player,4, "Siguiente pagina. ->", GOSSIP_SENDER_MAIN, 31);
			AddGossipItemFor(player,2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 100);			
            SendGossipMenuFor(player,1, m_creature->GetGUID());
                break;
        
        case 31:  
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_raptor:26:26:-15:0|t|r Dinosaurio.", GOSSIP_SENDER_MAIN, 20);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_ravager:26:26:-15:0|t|r Devastador.", GOSSIP_SENDER_MAIN, 19);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_tallstrider:26:26:-15:0|t|r Zancaalta.", GOSSIP_SENDER_MAIN, 13);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_scorpid:26:26:-15:0|t|r Escorpi""\xC3\xB3""n.", GOSSIP_SENDER_MAIN, 414);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_spider:26:26:-15:0|t|r Tar""\xC3\xA1""ntula.", GOSSIP_SENDER_MAIN, 16);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_cobrastrikes:26:26:-15:0|t|r Serpiente.", GOSSIP_SENDER_MAIN, 21);  
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_turtle:26:26:-15:0|t|r Tortuga.", GOSSIP_SENDER_MAIN, 15);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_wasp:26:26:-15:0|t|r Avispa.", GOSSIP_SENDER_MAIN, 93);
			AddGossipItemFor(player,4, "<- Pagina Anterior.", GOSSIP_SENDER_MAIN, 30);
            AddGossipItemFor(player,2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 100);
            SendGossipMenuFor(player,1, m_creature->GetGUID());
                break;
        
        case 50:                                    
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_chimera:26:26:-15:0|t|r Quimera Alavil.", GOSSIP_SENDER_MAIN, 51);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_corehound:26:26:-15:0|t|r Can del N""\xC3\xBA""cleo.", GOSSIP_SENDER_MAIN, 52);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_devilsaur:26:26:-15:0|t|r Demosaurio.", GOSSIP_SENDER_MAIN, 53);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_rhino:26:26:-15:0|t|r Rinoceronte.", GOSSIP_SENDER_MAIN, 54);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_silithid:26:26:-15:0|t|r Sil""\xC3\xAD""tido.", GOSSIP_SENDER_MAIN, 55);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_hunter_pet_worm:26:26:-15:0|t|r Gusano.", GOSSIP_SENDER_MAIN, 56);  
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Ability_druid_catform:26:26:-15:0|t|r Loque'nahak.", GOSSIP_SENDER_MAIN, 57);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Spell_nature_spiritwolf:26:26:-15:0|t|r Skoll.", GOSSIP_SENDER_MAIN, 58);
            AddGossipItemFor(player,6, "|cff00ff00|TInterface\\icons\\Spell_shadow_spectralsight:26:26:-15:0|t|r Gondria.", GOSSIP_SENDER_MAIN, 59);
			AddGossipItemFor(player,4, "<- Conseguir una mascota normal.", GOSSIP_SENDER_MAIN, 30);
			AddGossipItemFor(player,2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 100);
            SendGossipMenuFor(player,1, m_creature->GetGUID());
                break;
      
            case GOSSIP_OPTION_STABLEPET:
                player->GetSession()->SendStablePet(m_creature->GetGUID());
            break; 
            case GOSSIP_OPTION_VENDOR:
            break;
            case 51:
                CreatePet(player, m_creature, 21879);
            break;
            case 52: 
                CreatePet(player, m_creature, 21108);
            break;
            case 53: 
                CreatePet(player, m_creature, 20931);
            break;
            case 54: 
                CreatePet(player, m_creature, 30445);
            break;
            case 55: 
                CreatePet(player, m_creature, 5460);
            break;
            case 56: 
                CreatePet(player, m_creature, 30148);
            break;
            case 57: 
                CreatePet(player, m_creature, 32517);
            break;
            case 58: 
                CreatePet(player, m_creature, 35189);
            break;
            case 59: 
                CreatePet(player, m_creature, 33776);
            break;
            case 16: 
                CreatePet(player, m_creature, 2349);
            break;
            case 17: 
                CreatePet(player, m_creature, 27946);
            break;
            case 18: 
                CreatePet(player, m_creature, 28233);
            break;
            case 19: 
                CreatePet(player, m_creature, 17199);
            break;
            case 20: 
                CreatePet(player, m_creature, 14821);
            break;
            case 21: 
                CreatePet(player, m_creature, 28358);
            break;
            case 1: 
                CreatePet(player, m_creature, 29319);
            break;
            case 2: 
                CreatePet(player, m_creature, 29996);
            break;
            case 93: 
                CreatePet(player, m_creature, 28085);
                        break;
            case 4: 
                CreatePet(player, m_creature, 28097);
                        break;
            case 5: 
                CreatePet(player, m_creature, 26838);
                        break;
            case 6: 
                CreatePet(player, m_creature, 24478);
                        break;   
            case 7: 
                CreatePet(player, m_creature, 1417);
                        break;  
            case 8: 
                CreatePet(player, m_creature, 28213);
                        break;
            case 9: 
                CreatePet(player, m_creature, 29452);
                        break;
            case 10: 
                CreatePet(player, m_creature, 13036);
                        break;
            case 11: 
                CreatePet(player, m_creature, 27421);
                        break;
            case 12: 
                CreatePet(player, m_creature, 23136);
                        break;
            case 13: 
                CreatePet(player, m_creature, 22807);
                        break;
            case 414: 
                CreatePet(player, m_creature, 9698);
                        break;
            case 15: 
                CreatePet(player, m_creature, 25482);
                        break;
        }
        return true;
    }
};
 
void AddSC_Npc_Beastmaster()
{
    new Npc_Beastmaster();
}
