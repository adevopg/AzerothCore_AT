/*
 * Copyright (C) 2012 12PacketsCore <https://bitbucket.org/bananavodka/12packetscore/>
 */

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Config.h"
#include "Pet.h"
class duel_reset : public PlayerScript
{
public:
    duel_reset() : PlayerScript("duel_reset") {}

    void OnDuelEnd(Player *winner, Player *looser, DuelCompleteType type)
    {
        if (sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_ON_FINISH))
        {
            if (sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_ONLY_IN_ZONA_DE_DUELOS))
            {
                if(winner->GetZoneId() == 3519)
                {
                    if (type == DUEL_WON)
                    {
						/*winner->RemoveAurasDueToSpell(56526);
						looser->RemoveAurasDueToSpell(56526);*/
						/*winner->SetPhaseMask(1, true);
						if (winner->GetPet())
							winner->GetPet()->SetPhaseMask(1, true);
						looser->SetPhaseMask(1, true);
						if (looser->GetPet())
							looser->GetPet()->SetPhaseMask(1, true);*/
						/*winner->SetVisible(true);
						if (winner->GetPet())
							winner->GetPet()->SetVisible(true);
						looser->SetVisible(true);
						if (looser->GetPet())
							looser->GetPet()->SetVisible(true);*/
						if (looser->HasAura(66233))
							looser->RemoveAurasDueToSpell(66233);
						if (winner->HasAura(66233))
							winner->RemoveAurasDueToSpell(66233);
                        winner->RemoveArenaSpellCooldowns(true);
                        looser->RemoveArenaSpellCooldowns(true);
						winner->RemoveAllSpellCooldown();
                        looser->RemoveAllSpellCooldown();
                        winner->SetHealth(winner->GetMaxHealth());
                        looser->SetHealth(looser->GetMaxHealth());
						if (winner->getPowerType() == POWER_MANA)
                            winner->SetPower(POWER_MANA, winner->GetMaxPower(POWER_MANA));
                        if (looser->getPowerType() == POWER_MANA)
                            looser->SetPower(POWER_MANA, looser->GetMaxPower(POWER_MANA));
							
						if (winner->GetPet())
							winner->GetPet()->SetHealth(winner->GetPet()->GetMaxHealth());
						if (looser->GetPet())
							looser->GetPet()->SetHealth(looser->GetPet()->GetMaxHealth());
						if (winner->GetPet())
							if (winner->GetPet()->getPowerType() == POWER_MANA)
								winner->GetPet()->SetPower(POWER_MANA, winner->GetPet()->GetMaxPower(POWER_MANA));
						if (looser->GetPet())
							if (looser->GetPet()->getPowerType() == POWER_MANA)
								looser->GetPet()->SetPower(POWER_MANA, looser->GetPet()->GetMaxPower(POWER_MANA));
                    }
					else if (type == DUEL_FLED)
                    {
						/*winner->RemoveAurasDueToSpell(56526);
						looser->RemoveAurasDueToSpell(56526);*/
						/*winner->SetPhaseMask(1, true);
						if (winner->GetPet())
							winner->GetPet()->SetPhaseMask(1, true);
						looser->SetPhaseMask(1, true);
						if (looser->GetPet())
							looser->GetPet()->SetPhaseMask(1, true);*/
						/*winner->SetVisible(true);
						if (winner->GetPet())
							winner->GetPet()->SetVisible(true);
						looser->SetVisible(true);
						if (looser->GetPet())
							looser->GetPet()->SetVisible(true);*/
						if (looser->HasAura(66233))
							looser->RemoveAurasDueToSpell(66233);
						if (winner->HasAura(66233))
							winner->RemoveAurasDueToSpell(66233);
                        winner->RemoveArenaSpellCooldowns(true);
                        looser->RemoveArenaSpellCooldowns(true);
						winner->RemoveAllSpellCooldown();
                        looser->RemoveAllSpellCooldown();
                        winner->SetHealth(winner->GetMaxHealth());
                        looser->SetHealth(looser->GetMaxHealth());

                        if (winner->getPowerType() == POWER_MANA)
                            winner->SetPower(POWER_MANA, winner->GetMaxPower(POWER_MANA));

                        if (looser->getPowerType() == POWER_MANA)
                            looser->SetPower(POWER_MANA, looser->GetMaxPower(POWER_MANA));
						
						if (winner->GetPet())
							winner->GetPet()->SetHealth(winner->GetPet()->GetMaxHealth());
						if (looser->GetPet())
							looser->GetPet()->SetHealth(looser->GetPet()->GetMaxHealth());
						if (winner->GetPet())
							if (winner->GetPet()->getPowerType() == POWER_MANA)
								winner->GetPet()->SetPower(POWER_MANA, winner->GetPet()->GetMaxPower(POWER_MANA));
						if (looser->GetPet())
							if (looser->GetPet()->getPowerType() == POWER_MANA)
								looser->GetPet()->SetPower(POWER_MANA, looser->GetPet()->GetMaxPower(POWER_MANA));
                    }
                winner->HandleEmoteCommand(EMOTE_ONESHOT_CHEER);
                }
            }

            else
                if (type == DUEL_WON)
                {
					/*winner->SetPhaseMask(1, true);
					if (winner->GetPet())
							winner->GetPet()->SetPhaseMask(1, true);
					looser->SetPhaseMask(1, true);
					if (looser->GetPet())
							looser->GetPet()->SetPhaseMask(1, true);*/
					/*winner->SetVisible(true);
					if (winner->GetPet())
						winner->GetPet()->SetVisible(true);
					looser->SetVisible(true);
					if (looser->GetPet())
						looser->GetPet()->SetVisible(true);*/
					if (looser->HasAura(66233))
						looser->RemoveAurasDueToSpell(66233);
					if (winner->HasAura(66233))
						winner->RemoveAurasDueToSpell(66233);
                    winner->RemoveArenaSpellCooldowns(true);
                    looser->RemoveArenaSpellCooldowns(true);
					winner->RemoveAllSpellCooldown();
                    looser->RemoveAllSpellCooldown();
                    winner->SetHealth(winner->GetMaxHealth());
                    looser->SetHealth(looser->GetMaxHealth());

                    if (winner->getPowerType() == POWER_MANA)
                        winner->SetPower(POWER_MANA, winner->GetMaxPower(POWER_MANA));

                    if (looser->getPowerType() == POWER_MANA)
                        looser->SetPower(POWER_MANA, looser->GetMaxPower(POWER_MANA));
						
					if (winner->GetPet())
						winner->GetPet()->SetHealth(winner->GetPet()->GetMaxHealth());
					if (looser->GetPet())
						looser->GetPet()->SetHealth(looser->GetPet()->GetMaxHealth());
					if (winner->GetPet())
						if (winner->GetPet()->getPowerType() == POWER_MANA)
							winner->GetPet()->SetPower(POWER_MANA, winner->GetPet()->GetMaxPower(POWER_MANA));
					if (looser->GetPet())
						if (looser->GetPet()->getPowerType() == POWER_MANA)
							looser->GetPet()->SetPower(POWER_MANA, looser->GetPet()->GetMaxPower(POWER_MANA));
                }
                winner->HandleEmoteCommand(EMOTE_ONESHOT_CHEER);
        }
	}

	void OnDuelStart(Player *player1, Player *player2)
	{
        if (sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_ON_START))
        {
            if (sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_ONLY_IN_ZONA_DE_DUELOS))
            {
                if(player1->GetZoneId() == 3519)
                {
					/*player1->AddAura(56526, player1);
					player2->AddAura(56526, player2);*/
					/*player1->SetPhaseMask(8, true);
					if(player1->getClass() == CLASS_HUNTER)
						player1->CastSpell(player1, 883, true);
					if (player1->GetPet())
						player1->GetPet()->SetPhaseMask(8, true);
					player2->SetPhaseMask(8, true);
					if(player2->getClass() == CLASS_HUNTER)
						player2->CastSpell(player2, 883, true);
					if (player2->GetPet())
						player2->GetPet()->SetPhaseMask(8, true);*/
					/*player1->SetVisible(false);
					if (player1->GetPet())
						player1->GetPet()->SetVisible(false);
					player2->SetVisible(false);
					if (player2->GetPet())
						player2->GetPet()->SetVisible(false);*/
					if (player1->HasAura(66233))
							player1->RemoveAurasDueToSpell(66233);
						if (player2->HasAura(66233))
							player2->RemoveAurasDueToSpell(66233);
					player1->RemoveArenaSpellCooldowns(true);
                    player2->RemoveArenaSpellCooldowns(true);
					player1->RemoveAllSpellCooldown();
                    player2->RemoveAllSpellCooldown();
                    player1->SetHealth(player1->GetMaxHealth());
                    player2->SetHealth(player2->GetMaxHealth());
					
					if (player1->GetPet())
						player1->GetPet()->SetHealth(player1->GetPet()->GetMaxHealth());
					if (player2->GetPet())
						player2->GetPet()->SetHealth(player2->GetPet()->GetMaxHealth());
					if (player1->GetPet())
						if (player1->GetPet()->getPowerType() == POWER_MANA)
							player1->GetPet()->SetPower(POWER_MANA, player1->GetPet()->GetMaxPower(POWER_MANA));
					if (player2->GetPet())
						if (player2->GetPet()->getPowerType() == POWER_MANA)
							player2->GetPet()->SetPower(POWER_MANA, player2->GetPet()->GetMaxPower(POWER_MANA));					

                    if (sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_RESET_ENERGY_ON_START))
                    {
                        switch (player1->getPowerType())
                        {
                        case POWER_RAGE:
                            player1->SetPower(POWER_RAGE, 0);
                            break;
                        case POWER_RUNIC_POWER:
                            player1->SetPower(POWER_RUNIC_POWER, 0);
                            break;
                        default:
                            break;
                        }

                        switch (player2->getPowerType())
                        {
                        case POWER_RAGE:
                            player2->SetPower(POWER_RAGE, 0);
                            break;
                        case POWER_RUNIC_POWER:
                            player2->SetPower(POWER_RUNIC_POWER, 0);
                            break;
                        default:
                            break;
                        }
                    }

                    if (sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_MAX_ENERGY_ON_START))
                    {
                        switch (player1->getPowerType())
                        {
                        case POWER_MANA:
                            player1->SetPower(POWER_MANA, player1->GetMaxPower(POWER_MANA));
                            break;
                        case POWER_RAGE:
                            player1->SetPower(POWER_RAGE, player1->GetMaxPower(POWER_RAGE));
                            break;
                        case POWER_RUNIC_POWER:
                            player1->SetPower(POWER_RUNIC_POWER, player1->GetMaxPower(POWER_RUNIC_POWER));
                            break;
                        default:
                            break;
                        }

                        switch (player2->getPowerType())
                        {
                        case POWER_MANA:
                            player2->SetPower(POWER_MANA, player2->GetMaxPower(POWER_MANA));
                           break;
                       case POWER_RAGE:
                           player2->SetPower(POWER_RAGE, player2->GetMaxPower(POWER_RAGE));
                            break;
                        case POWER_RUNIC_POWER:
                           player2->SetPower(POWER_RUNIC_POWER, player2->GetMaxPower(POWER_RUNIC_POWER));
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

            else
            {
				/*player1->SetPhaseMask(8, true);
				if(player1->getClass() == CLASS_HUNTER)
					player1->CastSpell(player1, 883, true);
				if (player1->GetPet())
					player1->GetPet()->SetPhaseMask(8, true);
				player2->SetPhaseMask(8, true);
				if(player2->getClass() == CLASS_HUNTER)
					player2->CastSpell(player2, 883, true);
				if (player2->GetPet())
					player2->GetPet()->SetPhaseMask(8, true);*/
				/*player1->SetVisible(false);
				if (player1->GetPet())
					player1->GetPet()->SetVisible(false);
				player2->SetVisible(false);
				if (player2->GetPet())
					player2->GetPet()->SetVisible(false);*/
				if (player1->HasAura(66233))
					player1->RemoveAurasDueToSpell(66233);
				if (player2->HasAura(66233))
					player2->RemoveAurasDueToSpell(66233);					
                player1->RemoveArenaSpellCooldowns(true);
                player2->RemoveArenaSpellCooldowns(true);
				player1->RemoveAllSpellCooldown();
                player2->RemoveAllSpellCooldown();				
                player1->SetHealth(player1->GetMaxHealth());
                player2->SetHealth(player2->GetMaxHealth());
				
				if (player1->GetPet())
					player1->GetPet()->SetHealth(player1->GetPet()->GetMaxHealth());
				if (player2->GetPet())
					player2->GetPet()->SetHealth(player2->GetPet()->GetMaxHealth());
				if (player1->GetPet())
					if (player1->GetPet()->getPowerType() == POWER_MANA)
						player1->GetPet()->SetPower(POWER_MANA, player1->GetPet()->GetMaxPower(POWER_MANA));
				if (player2->GetPet())
					if (player2->GetPet()->getPowerType() == POWER_MANA)
						player2->GetPet()->SetPower(POWER_MANA, player2->GetPet()->GetMaxPower(POWER_MANA));	

                if (sWorld->getBoolConfig(CONFIG_DUEL_RESET_COOLDOWN_MAX_ENERGY_ON_START))
                {
                    switch (player1->getPowerType())
                    {
                    case POWER_MANA:
                        player1->SetPower(POWER_MANA, player1->GetMaxPower(POWER_MANA));
                        break;
                    case POWER_RAGE:
                        player1->SetPower(POWER_RAGE, player1->GetMaxPower(POWER_RAGE));
                        break;
                    case POWER_RUNIC_POWER:
                        player1->SetPower(POWER_RUNIC_POWER, player1->GetMaxPower(POWER_RUNIC_POWER));
                        break;
                    default:
                        break;
                    }

                    switch (player2->getPowerType())
                    {
                    case POWER_MANA:
                        player2->SetPower(POWER_MANA, player2->GetMaxPower(POWER_MANA));
                        break;
                    case POWER_RAGE:
                        player2->SetPower(POWER_RAGE, player2->GetMaxPower(POWER_RAGE));
                        break;
                    case POWER_RUNIC_POWER:
                        player2->SetPower(POWER_RUNIC_POWER, player2->GetMaxPower(POWER_RUNIC_POWER));
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
};

void AddSC_duelreset()
{
    new duel_reset();
}
