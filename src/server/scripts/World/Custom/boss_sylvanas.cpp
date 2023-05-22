#include "ScriptPCH.h"
#include "ObjectMgr.h"

enum Summons
{
    ESQUELETO_1          = 100001,
	ESQUELETO_2          = 100002,

	VALKYR_1             = 100003,
	VALKYR_2             = 100004
};

enum Spells
{
	SPELL_1             	= 69131,	//Usar cada de 19000 a 30000 objetivo
	SPELL_2               	= 66012,	//Usar Cada 12 segundos hasta 20 objetivo ALETORIO despues de 5s del inicio
	SPELL_3               	= 31013,	//Cada 18 a 25 segundos despues de 20 segundos del inicio
	SPELL_4                 = 67263,	//Castear al 80% sobre si mismo
	SPELL_5                 = 29833,	//Entre el 80 y el 60% castear cada 9s
	SPELL_6                 = 28679,	//Cada 35 a 39 a partir del 80%
	SPELL_7					= 43978		//Usar al Evadir
};

#define START	"Ignorantes vivos... No conseguireis derrotarme!"
#define LIVE_80 		"Necios, SUFRIREIS LA IRA DE SYLVANAS!"
#define ESQUELETOS 		"De acuerdo... probemos poniendolo un poco mas complicado... ADELANTE MIS SECUACES!"
#define VRYKULS 		"Esas invocaciones eran solo un preambulo... probemos con mis queridas Valkyr, ADELANTE!"
#define DIE 			"Argh... p-parece que... os he... subestimado..."

class boss_sylvanas : public CreatureScript
{
    public:

        boss_sylvanas()
            : CreatureScript("boss_sylvanas") 
		{
		}

		CreatureAI* GetAI(Creature* creature) const
		{
			return new boss_sylvanasAI(creature);
		}

        struct boss_sylvanasAI : public ScriptedAI
        {
            boss_sylvanasAI(Creature* creature) : ScriptedAI(creature)
            {
				me->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
				me->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
				me->ApplySpellImmune(0, IMMUNITY_ID, 8129, true);
				me->ApplySpellImmune(0, IMMUNITY_ID, 5138, true);
				me->ApplySpellImmune(0, IMMUNITY_ID, 30449, true);
			}

			uint8 Phase;
			uint64 uiEsqueleto1GUID;
			uint64 uiEsqueleto2GUID;
			uint64 uiValkyr1GUID;
			uint64 uiValkyr2GUID;

			uint32 m_uiSpell1Timer; //Cada de 24s
			uint32 m_uiSpell2Timer; //Cada 12s
			uint32 m_uiSpell3Timer; //Cada 22s
			uint32 m_uiSpell5Timer; //Cada 9s
			uint32 m_uiSpell6Timer; //Cada 37s

			uint32 m_uiSpell2Start; //4s despues de empezar el combate
			uint32 m_uiSpell3Start; //20s despues de empezar el combate

			Position pos;
			Position pos2;

			uint8 summon1;
			uint8 summon2;

			void EnterCombat(Unit* /*who*/)
            {
				me->Yell(START, LANG_UNIVERSAL, 0);
				
				char msg[250];

				if(me->GetVictim())
					sprintf(msg,"|CFF7BBEF7[|rDefensa de Hyjal|CFF7BBEF7]|r: |cffFFFFE0%s|r |cff00E5EEesta siendo atacada!", me->GetName().c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
            }

            void Reset()
            {
				DoCast(me, SPELL_7);
			
                if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiEsqueleto1GUID)))
					temp->setDeathState(JUST_DIED);

                if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiEsqueleto2GUID)))
					temp->setDeathState(JUST_DIED);

                if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiValkyr1GUID)))
					temp->setDeathState(JUST_DIED);

                if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiValkyr2GUID)))
					temp->setDeathState(JUST_DIED);

				me->RemoveAurasDueToSpell(SPELL_4);

				uiEsqueleto1GUID = 0;
				uiEsqueleto2GUID = 0;
				uiValkyr1GUID = 0;
				uiValkyr2GUID = 0;

				summon1=0;
				summon2=0;

				Phase=0;

				m_uiSpell1Timer=0;
				m_uiSpell2Timer=0;
				m_uiSpell3Timer=0;
				m_uiSpell5Timer=0;
				m_uiSpell6Timer=0;

				m_uiSpell2Start=4000; //4s despues de empezar el combate
				m_uiSpell3Start=20000; //20s despues de empezar el combate
            }

			void JustDied (Unit* /*killer*/)
            {
				me->Yell(DIE, LANG_UNIVERSAL, 0);

                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiEsqueleto1GUID)))
					temp->setDeathState(JUST_DIED);

                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiEsqueleto2GUID)))
					temp->setDeathState(JUST_DIED);

                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiValkyr1GUID)))
					temp->setDeathState(JUST_DIED);

                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiValkyr2GUID)))
					temp->setDeathState(JUST_DIED);

				uiEsqueleto1GUID = 0;
				uiEsqueleto2GUID = 0;
				uiValkyr1GUID = 0;
				uiValkyr2GUID = 0;

				summon1=0;
				summon2=0;

				Phase=0;

				m_uiSpell1Timer=0;
				m_uiSpell2Timer=0;
				m_uiSpell3Timer=0;
				m_uiSpell5Timer=0;
				m_uiSpell6Timer=0;

				m_uiSpell2Start=4000; //4s despues de empezar el combate
				m_uiSpell3Start=20000; //20s despues de empezar el combate
            }

            void UpdateAI(uint32 uiDiff)
            {
				uint32 curValue = me->GetPower(POWER_MANA);
				uint32 maxValue = me->GetMaxPower(POWER_MANA);
				//if (me->getPowerType() == POWER_MANA <= 30)
				if(curValue < maxValue)
					me->SetPower(POWER_MANA, maxValue);
					
                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiEsqueleto1GUID)))
				{
					uint32 curValue_1 = temp->GetPower(POWER_MANA);
					uint32 maxValue_1 = temp->GetMaxPower(POWER_MANA);
					if(curValue_1 < maxValue_1)
					{
						temp->SetPower(POWER_MANA, maxValue);
					}
					temp->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 8129, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 5138, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 30449, true);
				}
						
                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiEsqueleto2GUID)))

				{
					uint32 curValue_2 = temp->GetPower(POWER_MANA);
					uint32 maxValue_2 = temp->GetMaxPower(POWER_MANA);
					if(curValue_2 < maxValue_2)
					{
						temp->SetPower(POWER_MANA, maxValue);
					}
					temp->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 8129, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 5138, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 30449, true);
				}
                  if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiValkyr1GUID)))
				{
					uint32 curValue_3 = temp->GetPower(POWER_MANA);
					uint32 maxValue_3 = temp->GetMaxPower(POWER_MANA);
					if(curValue_3 < maxValue_3)
					{
						temp->SetPower(POWER_MANA, maxValue);
					}
					temp->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 8129, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 5138, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 30449, true);
				}
				
                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiValkyr2GUID)))
				{
					uint32 curValue_4 = temp->GetPower(POWER_MANA);
					uint32 maxValue_4 = temp->GetMaxPower(POWER_MANA);
					if(curValue_4 < maxValue_4)
					{
						temp->SetPower(POWER_MANA, maxValue);
					}
					temp->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 8129, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 5138, true);
					temp->ApplySpellImmune(0, IMMUNITY_ID, 30449, true);
				}
				
                if (!UpdateVictim())
                    return;

				//Si el objetivo tiene menos del 80% lanzar SPELL_1 cada 24s
				uint32 vidaPct = me->GetHealthPct();
				if(vidaPct >= 63 && m_uiSpell1Timer<=uiDiff) //Cada 26s mientras tenga mas del 81% de la vida
				{
					Unit* target =  SelectTarget(SelectTargetMethod::Random); //Objetivo aleatorio
					DoCast(target, SPELL_1);
					m_uiSpell1Timer=20000;
				}
				else
					m_uiSpell1Timer-= uiDiff;

				if(m_uiSpell2Start<=uiDiff)//4s despues de empezar el comate
				{
					if(m_uiSpell2Timer<=uiDiff)//Cada 22s lanzar SPELL_2
						{
                            Unit* target = SelectTarget(SelectTargetMethod::Random); //Objetivo aleatorio
							DoCast(target, SPELL_2);
							m_uiSpell2Timer=12000;
						}
					else
						m_uiSpell2Timer-=uiDiff;
				}
				else
					m_uiSpell2Start-=uiDiff;

				if(m_uiSpell3Start<=uiDiff)//20s despues de empezar el comate
				{
					if(m_uiSpell3Timer<=uiDiff)//Cada 12s lanzar SPELL_3
						{
							DoCast(me->GetVictim(), SPELL_3);
							m_uiSpell3Timer=20000;
						}
					else
						m_uiSpell3Timer-=uiDiff;
				}
				else
					m_uiSpell3Start-=uiDiff;

				if(Phase==0 && me->GetHealthPct() <= 80)//Si el boss tiene menos del 80% empieza la fase 1
				{
					me->Yell(LIVE_80, LANG_UNIVERSAL, 0);
					DoCast(me, SPELL_4);
					Phase++;
				}

				if(Phase==1 && me->GetHealthPct() <= 60)//Si el boss esta en la fase uno y tiene menos del 60% empieza la fase 2
					Phase++;

				/*if(Phase==1 && m_uiSpell5Timer<=uiDiff) //Cada 24s entre el 80% y el 60% de la vida en fase 1
				{
					DoCast(me->getVictim(), SPELL_5);
					m_uiSpell5Timer=9000;
				}*/
				
				if(me->GetHealthPct()<= 80 && me->GetHealthPct()>=61 && m_uiSpell5Timer<=uiDiff)//Cada 24s entre el 80% y el 60% de la vida en fase 1
				{	
					DoCast(me->GetVictim(), SPELL_5, true);
					m_uiSpell5Timer=9000;
				}
				else
					m_uiSpell5Timer-= uiDiff;

				if(me->GetHealthPct()<= 80 && m_uiSpell6Timer<=uiDiff) //Cada 37s entre el 80% y 0%
				{
					DoCast(me->GetVictim(), SPELL_6, true);
					m_uiSpell6Timer=37000;
				}
				else
					m_uiSpell6Timer-= uiDiff;

				if (Phase==2 && summon1 == 0)
				{
					me->Yell(ESQUELETOS, LANG_UNIVERSAL, 0);
                    Position pos = me->GetPosition();

					Creature* pSummon1 = me->SummonCreature(ESQUELETO_1, pos, TEMPSUMMON_CORPSE_DESPAWN);
                    ObjectGuid summonGuid1 = pSummon1->GetGUID();
                    uiEsqueleto1GUID = summonGuid1.GetRawValue();

					Creature* pSummon2 = me->SummonCreature(ESQUELETO_2, pos, TEMPSUMMON_CORPSE_DESPAWN);
                    ObjectGuid summonGuid2 = pSummon2->GetGUID();
                    uiEsqueleto2GUID = summonGuid2.GetRawValue();

					summon1++;
				}

				if (me->GetHealthPct() <= 40 && summon2 == 0)
				{
					me->Yell(VRYKULS, LANG_UNIVERSAL, 0);
                    Position pos2 = me->GetPosition();;

					Creature* pSummon3 = me->SummonCreature(VALKYR_1, pos2, TEMPSUMMON_CORPSE_DESPAWN);
                    ObjectGuid summonGuid3 = pSummon3->GetGUID();
                    uiValkyr1GUID = summonGuid3.GetRawValue();

					Creature* pSummon4 = me->SummonCreature(VALKYR_2, pos2, TEMPSUMMON_CORPSE_DESPAWN);
                    ObjectGuid summonGuid4 = pSummon4->GetGUID();
                    uiValkyr2GUID = summonGuid4.GetRawValue();

					summon2++;
				}

			DoMeleeAttackIfReady();
			}
		};
};

void AddSC_boss_sylvanas()
{
	new boss_sylvanas();
}
