#include "ScriptPCH.h"

enum Spells
{
    SPELL_VUELTA = 54640,
	DESMONTAR_PLAYER = 61286
};

enum NPCs // Todos los guardias de fuera tienen un rango de 35.0f desde estos NPCs
{
    NPC_CENTINELA = 910000
};

class defensor_shop : public CreatureScript
{
public:
    defensor_shop() : CreatureScript("defensor_shop") { }

    struct defensor_shopAI : public ScriptedAI
    {
        defensor_shopAI(Creature* pCreature): ScriptedAI(pCreature)
        {
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_NORMAL, true);
            pCreature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
			pCreature->ApplySpellImmune(0, IMMUNITY_ID, 54640, true);
			pCreature->ApplySpellImmune(0, IMMUNITY_ID, 54643, true);
			pCreature->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM, 250);
			pCreature->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_GHOST);
        }

        void Reset(){ }

        void EnterCombat(Unit* /*pWho*/){ }

        void AttackStart(Unit* /*pWho*/){ }

        void MoveInLineOfSight(Unit* who)
        {
            if(!who || !who->IsInWorld())
                return;

            if(!me->IsWithinDist(who, 65.0f, false))
                return;

            Player* pPlayer = who->GetCharmerOrOwnerPlayerOrPlayerItself();

            if(!pPlayer || pPlayer->IsGameMaster() || pPlayer->IsBeingTeleported())
                return;

            switch(me->GetEntry())
            {
                case 910000:
                    if(GetClosestCreatureWithEntry(me, NPC_CENTINELA, 32.0f))
                    {
						if(me->isInBackInMap(who, 12.0f))   // In my line of sight, "outdoors", and behind me
						{
							DoCast(who, SPELL_VUELTA); // Teleport the unit out
							DoCast(who, DESMONTAR_PLAYER);
							who->RemoveAurasByType(SPELL_AURA_MOUNTED);
							who->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
							who->RemoveAurasDueToSpell(46924);
							who->RemoveAurasDueToSpell(45438);
							who->RemoveAurasDueToSpell(642);
							//ChatHandler(pPlayer).PSendSysMessage("Se te ha enviado de vuelta porque no puedes pasar de aquí.");
						}
                    }
                    else	// In my line of sight, and "indoors"
					{
						DoCast(who, SPELL_VUELTA); // Teleport the unit out
						DoCast(who, DESMONTAR_PLAYER);
						who->RemoveAurasByType(SPELL_AURA_MOUNTED);
						who->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
						who->RemoveAurasDueToSpell(46924);
						who->RemoveAurasDueToSpell(45438);
						who->RemoveAurasDueToSpell(642);
						//ChatHandler(pPlayer).PSendSysMessage("Se te ha enviado de vuelta porque no puedes pasar de aquí.");
					}
				break;
            }
           
            me->SetOrientation(me->GetHomePosition().GetOrientation());
            return;
		}
		void UpdateAI(const uint32 /*diff*/){ }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new defensor_shopAI(pCreature);
    }
};

void AddSC_defensor_shop()
{
	new defensor_shop();
}
