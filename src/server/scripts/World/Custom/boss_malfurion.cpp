#include "ScriptPCH.h"
#include "ObjectMgr.h"
#include "World.h"
#include "ThreatMgr.h"
#include "Spell.h"

enum Summons
{
    ANTARBOL_NPC			= 100006,	//Invoca antárboles, son parecidos a los de Darnassus
	POLLO_NPC				= 100007,	//Invoca forma de pollo
};

enum Spells
{
	BEAR	             	= 19030,	//Forma de oso, anterior forma (cuesta mana lo cual hace que una vez se transforme, la segunda no te deje): 5487
	DEFENSE               	= 41105,	//Reduce el daño recibido
	CARGAR               	= 38461,	//Carga contra un objetivo y lo aturde
	ZARZAS                 	= 57095,	//Raices enredadoras, Segunda opcion de raices (no instant): 22127
//	EMPUJAR                 = 52029,	//Inflige un 20% del daño normal a los enemigos cercanos y los repele.
	BONE_SLICE              = 70814,	//Daño en área
	LECHUCICO				= 24858,	//Forma de lechucico lunar
	COLERA					= 65862,	//Cólera (solo en forma de pollo)
	CICLON					= 65859,	//Ciclón
	LLUVIA_ESTRELLAS		= 53201,	//Lluvia de estrellas
	ENJAMBRE				= 65855,	//Enjambre
	HURACAN					= 32717,	//Huracan
	TIFON					= 51817,	//Tifon
	PIEL_CORTEZA_ANTARBOL	= 65860,	//Piel de Corteza 40%
	OLEADA_PODER			= 60936,	//Dispara un rayo de magia pura a un enemigo e inflige 12000 p. de daño Arcano cada medio segundo durante 3 s
	RAYO_SOL				= 62872,	//Inflige 7400 to 8600 p. de daño de Naturaleza a los enemigos en un radio de 8 m del objetivo.
	PIEL_CORTEZA			= 22812,	//Piel de corteza 20%
	MOONFIRE				= 67946,	//Moonfire
	FAERIE_FIRE				= 65863,	//Fuego Feérico
	REJUVENECIMIENTO		= 66065,	//Rejuvenecimiento
	NUTRIR					= 66066,	//Nutrir
	RAICES_ANTARBOL			= 57095,	//Crecimiento Salvaje
	FLOR_DE_VIDA			= 33763,	//Flor de Vida
	RECRECIMIENTO			= 66067,	//Recrecimiento
	TRANQUILIDAD_ANTARBOL	= 66086,	//Tranquilidad
	SPELL_7					= 43978		//Usar al Evadir
};

#define START			"No conseguireis invadir nuestra querida Base Alianza. Al ataque!"
#define POLLO 			"Lechucicos lunares venid en mi ayuda!"
#define OSO				"Bien... PROBEMOS CON ESTO!"
#define ANTARBOL 		"Antarboles a mi!"
#define TRANQUILIDAD	"Antarbol comienza a usar |cff87CEFATranquilidad|r."
//#define MATARLE			"Rapido, debemos acabar con ese Antarbol!."
#define DIE 			"No... tanto tiempo... malgastado..."

class boss_malfurion : public CreatureScript
{
    public:

        boss_malfurion()
            : CreatureScript("boss_malfurion") {}

		CreatureAI* GetAI(Creature* creature) const
		{
			return new boss_malfurionAI(creature);
		}

        struct boss_malfurionAI : public ScriptedAI
        {
            boss_malfurionAI(Creature* creature) : ScriptedAI(creature)
            {
			me->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
			me->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
			me->ApplySpellImmune(0, IMMUNITY_ID, 8129, true);
			me->ApplySpellImmune(0, IMMUNITY_ID, 5138, true);
			me->ApplySpellImmune(0, IMMUNITY_ID, 30449, true);
			}

			uint8 Phase;

			uint64 uiSummonGUID[7]; //3+4 = 2 Pollos + 4 Antarboles
			uint8 pS;
			
			uint32 m_uiCargarTimer;
//			uint32 m_uiEmpujarTimer;
			uint32 m_uiBoneTimer;
			uint32 m_uiCiclonTimer;
			uint32 m_uiLluviaTimer;
			uint32 m_uiOleadaTimer;
			uint32 m_uiRayoTimer;
			uint32 m_uiPielTimer;
			uint32 m_uiColeraTimer;
			uint32 m_uiMoonfireTimer;
			uint32 m_uiFaerieFireTimer;
			
			Position pos;

			bool summon[2];
			
			//float TempThreat;
			
            void Reset()
            {
				if(me->HasAura(BEAR)) {me->RemoveAurasDueToSpell(BEAR);	me->RemoveAurasDueToSpell(DEFENSE);}//Quita la forma de oso
				if(me->HasAura(LECHUCICO)) { me->RemoveAurasDueToSpell(LECHUCICO); }
				
				//Elimina las invocaciones
				for(int i=0;i<7;i++){
                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiSummonGUID[i])))
                        temp->setDeathState(JUST_DIED);
					uiSummonGUID[i]=0;
					}
				m_uiCargarTimer = urand(15000, 20000);
//				m_uiEmpujarTimer = urand(5000, 10000);
				m_uiBoneTimer = 10000;
				m_uiOleadaTimer = 15000;
				m_uiRayoTimer = 5000;
				m_uiColeraTimer = 4000;
				m_uiMoonfireTimer = 3000;
				m_uiFaerieFireTimer = 1000;
				//TempThreat = 0;
				
				Phase=0;
			
				//Indica que no se han invocado a nadie
				summon[0] = false;
				summon[1] = false;
				
				//Se cura al tope la vida al evadir (para evitar posibles bugeos si se salen del area)
				uint32 curValue = me->GetPower(POWER_HEALTH);
				uint32 maxValue = me->GetMaxPower(POWER_HEALTH);

				if(curValue < maxValue)
					me->SetPower(POWER_HEALTH, maxValue);
				
				DoCast(me, SPELL_7);
            }

			void JustDied (Unit* /*killer*/)
            {
				me->Yell(DIE, LANG_UNIVERSAL, 0);

				//Elimina las invocaciones
				for(int i=0;i<7;i++)
                    if (Creature* temp = ObjectAccessor::GetCreature(*me, ObjectGuid(uiSummonGUID[i])))
                        temp->setDeathState(JUST_DIED);

            }

			void EnterCombat(Unit* /* who */)
            {
				char msg[250];

				if(me->GetVictim())
                    sprintf(msg, "|CFF7BBEF7[|rDefensa de Hyjal|CFF7BBEF7]|r: |cffFFFFE0%s|r |cff00E5EEesta siendo atacado!", me->GetName().c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				
				me->Yell(START, LANG_UNIVERSAL, 0);
				m_uiCargarTimer = urand(15000, 20000);
//				m_uiEmpujarTimer = urand(5000, 10000);
				m_uiBoneTimer = 10000;
				m_uiOleadaTimer = 15000;
				m_uiRayoTimer = 5000;
				m_uiColeraTimer = 4000;
				
				//Indica que no se han invocado a nadie
				summon[0] = false;
				summon[1] = false;
            }

			void UpdatePower()
			{
				uint32 curValue = me->GetPower(POWER_MANA);
				uint32 maxValue = me->GetMaxPower(POWER_MANA);
				//if (me->getPowerType() == POWER_MANA <= 30)
				if(curValue < maxValue)
					me->SetPower(POWER_MANA, maxValue);
			}
			
            void UpdateAI(uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
				if(Phase==0 && me->GetHealthPct() <= 70)
					Phase++;
				if(Phase==1 && me->GetHealthPct() <= 40)
					Phase++;
			
				uint32 curValue = me->GetPower(POWER_MANA);
				uint32 maxValue = me->GetMaxPower(POWER_MANA);
				//if (me->getPowerType() == POWER_MANA <= 30)
				if(curValue < maxValue)
					me->SetPower(POWER_MANA, maxValue);

			switch(Phase)
			{
			case 1:		//Fase Oso
			{
			if(!me->HasAura(BEAR) && me->HasAura(LECHUCICO) && !me->IsNonMeleeSpellCast(false))
			{
				me->Yell(OSO, LANG_UNIVERSAL, 0);
				me->RemoveAurasDueToSpell(LECHUCICO); 
				DoCast(me, BEAR); 	//Forma de oso
				DoCast(me, DEFENSE);
			}	

			/*if(me->HealthAbovePct(68))
				DoCast(me, DEFENSE);*/
			
			if(m_uiCargarTimer<=uiDiff)
				{
					me->RemoveAurasDueToSpell(BEAR);
					DoCastVictim(ZARZAS);
					DoCast(me, BEAR);
					DoCast(me, DEFENSE);
					me->GetThreatMgr().AddThreat(me->GetVictim(), -100);
                    bool requireLineOfSight = false;
                    float maxDistance = 30.0f;
                    Unit* target = me->SelectNearestTarget(requireLineOfSight, maxDistance); //Objetivo mas alejado en max distancia 30
					sWorld->SendAreaText(3707, "Malfurion carga sobre |cff87CEFA$N|r y centra toda su ira sobre el.", target->GetGUID());
					DoCast(target, CARGAR);
					me->GetThreatMgr().AddThreat(target, 100);
					m_uiCargarTimer = urand(15000, 24000);
					/*TempThreat = DoGetThreat(target);
					if(TempThreat)
                        DoModifyThreatPercent(target, -100);
                    HoodGUID = target->GetGUID();
                    me->AddThreat(target, 1000000.0f);*/
					//me->Attack(target, true);
				}
			else m_uiCargarTimer-=uiDiff;
/*
			if(m_uiEmpujarTimer<=uiDiff)
				{
					DoCastVictim(EMPUJAR);
					m_uiEmpujarTimer = urand(15000, 25000);
				}
			else m_uiEmpujarTimer-=uiDiff;
*/
			if(m_uiBoneTimer<=uiDiff)
				{
					DoCastVictim(BONE_SLICE);
					m_uiBoneTimer = urand(10000, 15000);
				}
			else m_uiBoneTimer-=uiDiff;

			}
			break;
			case 0:		//Fase Pollo
			{
				if(!me->HasAura(LECHUCICO) && !me->IsNonMeleeSpellCast(false)) {me->RemoveAurasDueToSpell(BEAR);	me->RemoveAurasDueToSpell(DEFENSE); DoCast(me, LECHUCICO);}//Quita la forma de oso
				if(!summon[0]){
					me->Yell(POLLO, LANG_UNIVERSAL, 0);
                    Position pos = me->GetPosition();
					for(int i=0;i<3;i++)
						{
						Creature* pSummon = me->SummonCreature(POLLO_NPC, pos, TEMPSUMMON_CORPSE_DESPAWN);
                        ObjectGuid summonGuid = pSummon->GetGUID();
                        uiSummonGUID[i] = summonGuid.GetRawValue();
						}
					summon[0]=true;
				}
				
				if(m_uiCiclonTimer<=uiDiff)
				{
                    bool requireLineOfSight = false;
                    float maxRange = 20.0f;
                    Unit* target = me->SelectNearestTarget(requireLineOfSight, maxRange); //Objetivo mas alejado en rango de 20
					DoCast(target, CICLON);
					m_uiCiclonTimer = urand(25000, 30000);
				}
				else m_uiCiclonTimer-=uiDiff;
				
				if(m_uiOleadaTimer<=uiDiff)
				{
					DoCastVictim(OLEADA_PODER);
					m_uiOleadaTimer = urand(16000, 19000);
				}
				else m_uiOleadaTimer-=uiDiff;
				
				if(m_uiColeraTimer<=uiDiff)
				{
					if(!me->IsNonMeleeSpellCast(false))
					{
						DoCastVictim(COLERA);
						m_uiColeraTimer = 4000;
					}
				}
				else m_uiColeraTimer-=uiDiff;
			}
			break;
			case 2: 	//Fase antarboles
			{
				if(me->HasAura(BEAR)){me->RemoveAurasDueToSpell(BEAR);}
				if(!summon[1]){
					me->Yell(ANTARBOL, LANG_UNIVERSAL, 0);
                    Position pos = me->GetPosition();
					for(int i=3;i<7;i++)
						{
                        Creature* pSummon = me->SummonCreature(ANTARBOL_NPC, pos, TEMPSUMMON_CORPSE_DESPAWN);
                        ObjectGuid summonGuid = pSummon->GetGUID();
                        uiSummonGUID[i] = summonGuid.GetRawValue();

						}
					summon[1]=true;
				}
				
				if(m_uiRayoTimer<=uiDiff)
				{
					Unit* target =  SelectTarget(SelectTargetMethod::Random); //Objetivo aleatorio
					DoCast(target, RAYO_SOL);
					m_uiRayoTimer = urand(5000, 15000);
				}
				else m_uiRayoTimer-=uiDiff;
				
				
				if(m_uiMoonfireTimer<=uiDiff && !me->IsNonMeleeSpellCast(false))
				{
					Unit* target =  SelectTarget(SelectTargetMethod::Random);
					DoCast(target, MOONFIRE);
					m_uiMoonfireTimer = urand(6000, 12000);
				}
				else m_uiMoonfireTimer-=uiDiff;
				
				if(m_uiFaerieFireTimer<=uiDiff && !me->IsNonMeleeSpellCast(false))
				{
					DoCastVictim(FAERIE_FIRE);
					m_uiFaerieFireTimer = urand(32000, 40000);
				}
				else m_uiFaerieFireTimer-=uiDiff;
				
				if(m_uiPielTimer<=uiDiff)
				{
					DoCast(me, PIEL_CORTEZA);
					m_uiPielTimer = urand(20000, 35000);
				}
				else m_uiPielTimer-=uiDiff;
			
				if(m_uiBoneTimer<=uiDiff)
				{
					DoCastVictim(BONE_SLICE);
					m_uiBoneTimer = urand(5000, 19000);
				}
			else m_uiBoneTimer-=uiDiff;
			}
			break;
			}
			DoMeleeAttackIfReady();
			}
		};
};

class npc_pollo : public CreatureScript
{
    public:
        npc_pollo() : CreatureScript("npc_pollo") { }

        struct npc_polloAI : public ScriptedAI
        {
            npc_polloAI(Creature* pCreature): ScriptedAI(pCreature) 
			{ 
				me->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
				me->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
				me->ApplySpellImmune(0, IMMUNITY_ID, 8129, true);
				me->ApplySpellImmune(0, IMMUNITY_ID, 5138, true);
				me->ApplySpellImmune(0, IMMUNITY_ID, 30449, true);
			}

			uint32 m_uiLluviaTimer;
			uint32 m_uiEnjambreTimer;
			uint32 m_uiZarzaTimer;
			uint32 m_uiTifonTimer;
			uint32 m_uiHuracanTimer;
			
			void EnterCombat(Unit* /*who*/)
            {
				m_uiEnjambreTimer = urand(15000, 30000);
				m_uiTifonTimer = urand (25000, 40000);
				m_uiHuracanTimer = urand (5000, 30000);
				DoMeleeAttackIfReady();
            }
			
			void UpdatePower()
			{
				uint32 curValue = me->GetPower(POWER_MANA);
				uint32 maxValue = me->GetMaxPower(POWER_MANA);
				//if (me->getPowerType() == POWER_MANA <= 30)
				if(curValue < maxValue)
					me->SetPower(POWER_MANA, maxValue);
			}
			
			void UpdateAI(uint32 uiDiff)
				{
				uint32 curValue = me->GetPower(POWER_MANA);
				uint32 maxValue = me->GetMaxPower(POWER_MANA);
				//if (me->getPowerType() == POWER_MANA <= 30)
				if(curValue < maxValue)
					me->SetPower(POWER_MANA, maxValue);
					
				if(m_uiHuracanTimer<=uiDiff)
				{
                    Unit* target = SelectTarget(SelectTargetMethod::Random); //Objetivo aleatorio
					DoCast(target, HURACAN);
					m_uiHuracanTimer = urand(20000, 60000);
				}
				else m_uiHuracanTimer-=uiDiff;
			
				if(m_uiLluviaTimer<=uiDiff && me )
				{
					DoCast(me, LLUVIA_ESTRELLAS);
					m_uiLluviaTimer = urand(20000, 45000);
				}
				else m_uiLluviaTimer-=uiDiff;

				if(m_uiEnjambreTimer<=uiDiff)
				{
					DoCastVictim(ENJAMBRE);
					m_uiEnjambreTimer = urand(17000, 26000);
				}
				else m_uiEnjambreTimer-=uiDiff;
				
				if(m_uiTifonTimer<=uiDiff)
				{
                    bool requireLineOfSight = false;
                    float maxRange = 20.0f;
                    Unit* target = me->SelectNearestTarget(requireLineOfSight, maxRange); //Objetivo mas alejado en rango de 20
					DoCast(target, TIFON);
					m_uiTifonTimer = urand(17000, 26000);
				}
				else m_uiTifonTimer-=uiDiff;
				
				DoMeleeAttackIfReady();
				}

		};
	CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_polloAI(pCreature);
    }
};

class npc_antarbol : public CreatureScript
{
public:
    npc_antarbol() : CreatureScript("npc_antarbol") { }

    struct npc_antarbolAI : public ScriptedAI
    {
        npc_antarbolAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 8129, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 5138, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 30449, true);
        }

			uint32 m_uiCura1Timer;
			uint32 m_uiCura2Timer;
			uint32 m_uiCura3Timer;
			uint32 m_uiCura4Timer;
			uint32 m_uiCura5Timer;
			uint32 m_uiCura2EllosTimer;
			uint32 m_uiCura4EllosTimer;
			
			Creature* uiSummer;
			
			bool HasCastedTranquilidad;
			
            void IsSummonedBy(WorldObject* owner) override
            {
                if (owner->GetTypeId() != TYPEID_UNIT)
                    return;

                uiSummer = owner->ToCreature();
            }

			
			void EnterCombat(Unit* /*who*/)
            {
				m_uiCura1Timer = urand(8000, 20000);
				m_uiCura2Timer = urand(16000, 34000);
				m_uiCura3Timer = urand(6000, 10000);
				m_uiCura4Timer = urand(10000, 15000);
				m_uiCura5Timer = urand(14000, 24000);
            }
			
			void UpdatePower()
			{
				uint32 curValue = me->GetPower(POWER_MANA);
				uint32 maxValue = me->GetMaxPower(POWER_MANA);
				//if (me->getPowerType() == POWER_MANA <= 30)
				if(curValue < maxValue)
					me->SetPower(POWER_MANA, maxValue);
			}
			
			void UpdateAI(uint32 uiDiff)
			{
				uint32 curValue = me->GetPower(POWER_MANA);
				uint32 maxValue = me->GetMaxPower(POWER_MANA);
				//if (me->getPowerType() == POWER_MANA <= 30)
				if(curValue < maxValue)
					me->SetPower(POWER_MANA, maxValue);
					
				//Sobre Malfurion
				if(m_uiCura1Timer<=uiDiff && !me->IsNonMeleeSpellCast(false))
				{
					DoCast(uiSummer, REJUVENECIMIENTO);
					m_uiCura1Timer = urand(8000, 30000);
				}
				else m_uiCura1Timer-=uiDiff;
			
				if(m_uiCura2Timer<=uiDiff && !me->IsNonMeleeSpellCast(false))
				{
					DoCast(uiSummer, NUTRIR);
					m_uiCura2Timer = urand(26000, 40000);
				}
				else m_uiCura2Timer-=uiDiff;
				
				if(m_uiCura3Timer<=uiDiff && !me->IsNonMeleeSpellCast(false))
				{
                    bool requireLineOfSight = false;
                    float maxDistance = 40.0f;
                    Unit* target = me->SelectNearestTarget(requireLineOfSight, maxDistance); //Objetivo mas alejado en max distancia 40
					DoCast(target, RAICES_ANTARBOL);
					m_uiCura3Timer = urand(20000, 30000);
				}
				else m_uiCura3Timer-=uiDiff;
				
				if(m_uiCura4Timer<=uiDiff && !me->IsNonMeleeSpellCast(false))
				{
					DoCast(uiSummer, FLOR_DE_VIDA);
					m_uiCura4Timer = urand(6000, 15000);
				}
				else m_uiCura4Timer-=uiDiff;
				
				if(m_uiCura5Timer<=uiDiff && !me->IsNonMeleeSpellCast(false))
				{
					DoCast(uiSummer, RECRECIMIENTO);
					m_uiCura5Timer = urand(14000, 25000);
				}
				else m_uiCura5Timer-=uiDiff;
				
				uint32 VidaPct = me->GetHealthPct();
				//Sobre ellos
				if(m_uiCura2EllosTimer<=uiDiff && !me->IsNonMeleeSpellCast(false) && !HealthAbovePct(70))
				{
					DoCast(me, NUTRIR);
					m_uiCura2EllosTimer = urand(26000, 40000);
				}
				else m_uiCura2EllosTimer-=uiDiff;

				if(m_uiCura4EllosTimer<=uiDiff && !me->IsNonMeleeSpellCast(false) && !HealthAbovePct(80))
				{
					DoCast(me, FLOR_DE_VIDA);
					m_uiCura4EllosTimer = urand(6000, 15000);
				}
				else m_uiCura4EllosTimer-=uiDiff;

				if(VidaPct == 15 && HasCastedTranquilidad && !me->HasAura(TRANQUILIDAD_ANTARBOL))
				{
					//me->me->getVictim()->Say(MATARLE, LANG_UNIVERSAL);
					DoCast(me, PIEL_CORTEZA_ANTARBOL);
					DoCast(me, TRANQUILIDAD_ANTARBOL);
					sWorld->SendAreaText(3707, TRANQUILIDAD, me->GetGUID());
					HasCastedTranquilidad = false;
				}
				
			DoMeleeAttackIfReady();
			}
		};
        CreatureAI* GetAI(Creature* pCreature) const override
        {
            return new npc_antarbolAI(pCreature);
        }
};
void AddSC_boss_malfurion()
{
	new boss_malfurion();
	new npc_pollo();
	new npc_antarbol();
}
