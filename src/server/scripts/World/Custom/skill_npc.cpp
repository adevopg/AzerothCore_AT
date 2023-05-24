#include "ScriptPCH.h"
 
class skill_npc : public CreatureScript
 
        {
public:
 
skill_npc() : CreatureScript("skill_npc") {}
 
struct skill_npcAI : public ScriptedAI
        {
                skill_npcAI(Creature *c) : ScriptedAI(c)
                {
                }

               
        };
 
        CreatureAI* GetAI(Creature* _creature) const
    {
                return new skill_npcAI(_creature);
    }
 
        void CreatureWhisperBasedOnBool(const char *text, Creature *_creature, Player *pPlayer, bool value)
        {
                if (value)
                        _creature->Whisper(text, LANG_UNIVERSAL, pPlayer);
        }
 
        uint32 PlayerMaxLevel() const
        {
                return sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
        }
 
 
void MainMenu(Player *pPlayer, Creature* _creature)
{
    AddGossipItemFor(pPlayer,9, "[Profesiones Primarias] ->", GOSSIP_SENDER_MAIN, 196);
    AddGossipItemFor(pPlayer, 9, "[Profesiones Secundarias] ->", GOSSIP_SENDER_MAIN, 197);
    AddGossipItemFor(pPlayer,2, "Olvidalo", GOSSIP_SENDER_MAIN, 2050);
    pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
}
 
bool PlayerHasItemOrSpell(const Player *plr, uint32 itemId, uint32 spellId) const
{
    return plr->HasItemCount(itemId, 1, true) || plr->HasSpell(spellId);
}
 
bool OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    MainMenu(pPlayer, pCreature);
    return true;
}
 
bool PlayerAlreadyHasTwoProfessions(const Player *pPlayer) const
        {
                uint32 skillCount = 0;
 
                if (pPlayer->HasSkill(SKILL_MINING))
                        skillCount++;
                if (pPlayer->HasSkill(SKILL_SKINNING))
                        skillCount++;
                if (pPlayer->HasSkill(SKILL_HERBALISM))
                        skillCount++;
 
                if (skillCount >= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
                        return true;
 
                for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
                {
                        SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
                        if (!SkillInfo)
                                continue;
 
                        if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
                                continue;
 
                        if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
                                continue;
 
                        const uint32 skillID = SkillInfo->id;
                        if (pPlayer->HasSkill(skillID))
                                skillCount++;
 
                        if (skillCount >= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
                                return true;
                }
 
                return false;
        }

void LearnSkillRecipesHelper(Player* player, uint32 skill_id)
{
    uint32 classmask = player->getClassMask();

    for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
    {
        SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
        if (!skillLine)
            continue;

        // wrong skill
        if (skillLine->SkillLine != skill_id)
            continue;

        // not high rank
        if (skillLine->SupercededBySpell)
            continue;

        // skip racial skills
        if (skillLine->RaceMask != 0)
            continue;

        // skip wrong class skills
        if (skillLine->ClassMask && (skillLine->ClassMask & classmask) == 0)
            continue;

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(skillLine->Spell);
        if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo))
            continue;

        player->learnSpell(skillLine->Spell, false);
    }
}
 
        bool LearnAllRecipesInProfession(Player *pPlayer, SkillType skill)
        {
                ChatHandler handler(pPlayer->GetSession());
       const char* skill_name;
      
        SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
                skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];
 
        if (!SkillInfo)
                {
						//TRINITY
                        //sLog->outError(LOG_FILTER_GENERAL,"Teleport NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
						//MYTH
						LOG_ERROR("skill_npc", "Teleport NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
			
			return false;
                }
 
        LearnSkillRecipesHelper(pPlayer, SkillInfo->id);
 
        uint16 maxLevel = pPlayer->GetPureMaxSkillValue(SkillInfo->id);
        pPlayer->SetSkill(SkillInfo->id, pPlayer->GetSkillStep(SkillInfo->id), maxLevel, maxLevel);
        handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);
                
                return true;
        }
        
      
 
        bool IsSecondarySkill(SkillType skill) const
        {
                return skill == SKILL_COOKING || skill == SKILL_FIRST_AID || skill == SKILL_FISHING;
        }
 
        void CompleteLearnProfession(Player *pPlayer, Creature *pCreature, SkillType skill)
        {
                if (PlayerAlreadyHasTwoProfessions(pPlayer) && !IsSecondarySkill(skill))
                        pCreature->Whisper("Ya sabes dos profesiones!",LANG_UNIVERSAL, pPlayer);
                else
               {
                        if (!LearnAllRecipesInProfession(pPlayer, skill))
                                pCreature->Whisper("Error interno!", LANG_UNIVERSAL, pPlayer);
                }
       }
        
     bool OnGossipSelect(Player* pPlayer, Creature* _creature, uint32 uiSender, uint32 uiAction)
{ 
        pPlayer->PlayerTalkClass->ClearMenus();
        
        if (uiSender == GOSSIP_SENDER_MAIN)
        {
                
                switch (uiAction)
                {
                                case 196:
                                AddGossipItemFor(pPlayer,3, "Alquimia", GOSSIP_SENDER_MAIN, 1);
                                AddGossipItemFor(pPlayer,3, "Herreria", GOSSIP_SENDER_MAIN, 2);
                                AddGossipItemFor(pPlayer,3, "Peleteria", GOSSIP_SENDER_MAIN, 3);
                                AddGossipItemFor(pPlayer,3, "Sastreria", GOSSIP_SENDER_MAIN, 4);
                                AddGossipItemFor(pPlayer,3, "Ingenieria", GOSSIP_SENDER_MAIN, 5);
                                AddGossipItemFor(pPlayer,3, "Encantador", GOSSIP_SENDER_MAIN, 6);
                                AddGossipItemFor(pPlayer,3, "Joyeria", GOSSIP_SENDER_MAIN, 7);
                                AddGossipItemFor(pPlayer,3, "Inscripcion", GOSSIP_SENDER_MAIN, 8);
                                AddGossipItemFor(pPlayer,3, "Mineria", GOSSIP_SENDER_MAIN, 11);
                                AddGossipItemFor(pPlayer,3, "Herboristeria", GOSSIP_SENDER_MAIN, 12);
                                AddGossipItemFor(pPlayer,3, "Desuello", GOSSIP_SENDER_MAIN, 13);	
								AddGossipItemFor(pPlayer, 2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 2051);
								AddGossipItemFor(pPlayer, 2, "Olvidalo", GOSSIP_SENDER_MAIN, 2050);
 
                                pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
                                break;
								
								case 197:

                                AddGossipItemFor(pPlayer,3, "Cocinero", GOSSIP_SENDER_MAIN, 9);
                                AddGossipItemFor(pPlayer,3, "Primeros Auxilios", GOSSIP_SENDER_MAIN, 10);
								AddGossipItemFor(pPlayer,3, "Pesca", GOSSIP_SENDER_MAIN, 14);
								AddGossipItemFor(pPlayer, 2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 2051);
								AddGossipItemFor(pPlayer, 2, "Olvidalo", GOSSIP_SENDER_MAIN, 2050);
                                pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
                                break;		
								
                        case 1:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_ALCHEMY);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 2:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_BLACKSMITHING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 3:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_LEATHERWORKING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 4:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_TAILORING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 5:
								//TRINITY
                                //CompleteLearnProfession(pPlayer, _creature, SKILL_ENGINEERING);
								//MYTH
								CompleteLearnProfession(pPlayer, _creature, SKILL_ENGINEERING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 6:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_ENCHANTING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 7:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_JEWELCRAFTING);
 
                                CloseGossipMenuFor(pPlayer);
                                break;
                        case 8:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_INSCRIPTION);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 9:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_COOKING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 10:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_FIRST_AID);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 11:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_MINING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 12:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_HERBALISM);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
                        case 13:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_SKINNING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
						case 14:
								CompleteLearnProfession(pPlayer, _creature, SKILL_FISHING);
 
                               CloseGossipMenuFor(pPlayer);
                                break;
								
						case 2050:
								CloseGossipMenuFor(pPlayer);
								break;	
								
						case 2051:
                        AddGossipItemFor(pPlayer,9, "[Profesiones Primarias] ->", GOSSIP_SENDER_MAIN, 196);
						AddGossipItemFor(pPlayer,9, "[Profesiones Secundarias] ->", GOSSIP_SENDER_MAIN, 197);
						AddGossipItemFor(pPlayer, 2, "Olvidalo", GOSSIP_SENDER_MAIN, 2050);
                        pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());	
								break;
                }
 
        
        }
         return true;
         }
         };
 
void AddSC_skill_npc()
{
    new skill_npc();
};
