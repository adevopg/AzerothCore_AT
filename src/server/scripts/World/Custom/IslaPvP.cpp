#include "ScriptPCH.h"
#include "AccountMgr.h"
#include "Group.h"

static bool curar_1=false, curar_2=false, curar_3=false, curar_4=false, rabiar_1=false, rabiar_2=false, rabiar_3=false, rabiar_4=false, rabiar_5=false, rabiar_6=false, caldero_1=false, caldero_2=false, caldero_3=false;

enum PvPIsland
{
    AREA_PVP_ISLAND             = 368,

    SPELL_BERSERK               = 24378,
    SPELL_FOOD                  = 23493,
    //SPELL_SPIRITUAL_IMMUNITY    = 36910,
    SPELL_SUMMON_PET            = 883,
    SPELL_ANCIENT               = 36910,
	CAMBIO_DE_FASE				= 8611,

    NPC_GRAVEYARD_TELEPORTER    = 731131,

    GAMEOBJECT_BERSERKING_1     = 1649780,
	GAMEOBJECT_BERSERKING_2     = 1649784,
	GAMEOBJECT_BERSERKING_3     = 1649785,
	GAMEOBJECT_BERSERKING_4     = 1649790,
	GAMEOBJECT_BERSERKING_5     = 1649791,
	GAMEOBJECT_BERSERKING_6     = 1649792,
    GAMEOBJECT_FOOD_1           = 1649781,
	GAMEOBJECT_FOOD_2           = 1649786,
	GAMEOBJECT_FOOD_3           = 1649787,
	GAMEOBJECT_FOOD_4           = 1649793,
    GAMEOBJECT_ANCIENT_1        = 1649783,
	GAMEOBJECT_ANCIENT_2        = 1649788,
	GAMEOBJECT_ANCIENT_3        = 1649789,
};
 
const int MAX_PLAYER_SPAWN_POINTS = 20;
static const Position playerSpawnPoint[MAX_PLAYER_SPAWN_POINTS] = 
{
    {-1172.90f, -5331.40f, 4.85f, 4.64f}, 
    {-1319.34f, -5385.09f, 3.34f, 5.88f},
    {-1135.17f, -5587.93f, 10.5f, 1.53f},
    {-1235.21f, -5592.60f, 8.49f, 1.39f},
    {-1056.33f, -5639.83f, 5.49f, 1.74f},
    {-1031.71f, -5530.50f, 4.75f, 3.14f},
    {-989.878f, -5418.93f, 3.90f, 3.26f},
    {-1077.64f, -5347.45f, 3.93f, 4.18f},
    {-1136.49f, -5420.34f, 11.4f, 3.23f},
    {-1235.88f, -5462.10f, 5.76f, 1.29f},
    {-1162.98f, -5585.53f, 12.3f, 2.29f},
    {-840.228f, -5616.32f, 2.97f, 6.28f},
    {-719.558f, -5685.03f, 23.5f, 1.78f},
    {-638.830f, -5622.10f, 6.42f, 2.90f},
    {-662.432f, -5509.45f, 4.08f, 3.95f},
    {-772.277f, -5353.24f, 2.03f, 2.83f},
    {-949.171f, -5179.04f, 2.11f, 3.98f},
    {-1174.97f, -5116.36f, 3.65f, 6.14f},
    {-1372.61f, -5169.68f, 1.81f, 0.45f},
    {-1602.78f, -5250.65f, 6.50f, 5.05f},
};
 
const int MAX_RABIAR_SPAWN_POINTS = 3;
static const Position RabiarSpawnPoint[MAX_RABIAR_SPAWN_POINTS] = 
{
	{-1126.84f, -5130.68f, 2.71f, 4.10f},
	{-960.9088f, -5186.46f, 1.38f, 4.09f},
	{-805.20f, -5334.99f, 1.99f, 4.51f},
};

const int MAX_CURAR_SPAWN_POINTS = 3;
static const Position CurarSpawnPoint[MAX_CURAR_SPAWN_POINTS] = 
{
	{-726.1297f, -5595.66f, 25.27f, 5.57f},
	{-1134.96f, -5571.29f, 9.90f, 1.60f},
	{-1195.94f, -5390.18f, 7.69f, 6.16f},
};

const int MAX_CALDERO_SPAWN_POINTS = 3;
static const Position CalderoSpawnPoint[MAX_CALDERO_SPAWN_POINTS] = 
{
    {-1029.60f, -5423.49f, 3.55f, 1.25f},
    {-1286.09f, -5534.46f, 15.19f, 0.27f}, 
	{-1544.06f, -5312.42f, 8.06f, 6.13f},	
};

class pvp_island_resurrect_event : public BasicEvent
{
    public:
        pvp_island_resurrect_event(Player* victim) : _victim(victim) { }
 
        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            _victim->ResurrectPlayer(1.0, false);
			int32 RespawnPos = urand(0, MAX_PLAYER_SPAWN_POINTS - 1);
            _victim->TeleportTo(1, playerSpawnPoint[RespawnPos].GetPositionX(), playerSpawnPoint[RespawnPos].GetPositionY(), playerSpawnPoint[RespawnPos].GetPositionZ(), playerSpawnPoint[RespawnPos].GetOrientation());
            _victim->ResetAllPowers();
			_victim->RemoveArenaSpellCooldowns();
			_victim->RemoveAllSpellCooldown();
			_victim->CastSpell(_victim, CAMBIO_DE_FASE, false);
            return true;
        }
 
    private:
        Player* _victim;
};
 
class pvp_island_resurrect_event_pet : public BasicEvent
{
    public:
        pvp_island_resurrect_event_pet(Player* victim) : _victim(victim) { }
 
        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            _victim->CastSpell(_victim, SPELL_SUMMON_PET, false); // Summons last used pet
            return true;
        }
 
    private:
        Player* _victim;
};
 
class pvp_island : public PlayerScript
{
    public:
        pvp_island() : PlayerScript("pvp_island") { }
 
        uint64 killerGUID;
        uint64 victimGUID;
        char msg[500];

		int32 Getmuertes(Player * player)
		{
			QueryResult cuenta = CharacterDatabase.Query("SELECT muertes FROM racha WHERE guid = '%u'", player->GetGUID().GetRawValue());

			if (!cuenta)
				return 0;
			else
			{
				Field *fields = cuenta->Fetch();
					return fields[0].Get<uint32>();
			}
		}

		/*int32 Getsanacion(Player * player)
		{
			QueryResult cuenta2 = CharacterDatabase.PQuery("SELECT sanacion FROM racha_sanacion WHERE guid = '%u'", player->GetGUID());

			if (!cuenta2)
				return 0;
			else
			{
				Field *fields = cuenta2->Fetch();
					return fields[0].GetUInt32();
			}
		}

		int32 Getsanacion2(Player * player)
		{
			QueryResult cuenta3 = CharacterDatabase.PQuery("SELECT sanacion2 FROM racha_sanacion WHERE guid = '%u'", player->GetGUID());

			if (!cuenta3)
				return 1;
			else
			{
				Field *fields = cuenta3->Fetch();
					return fields[0].GetUInt32();
			}
		}*/
		
		void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea)
        {
			if (newArea == AREA_PVP_ISLAND)
			{
				QueryResult online_en_area = CharacterDatabase.Query("SELECT COUNT(area) FROM characters WHERE online = 1 AND area = 368");
				Field* fields = online_en_area->Fetch();
				uint32 online_area = fields[0].Get<uint16>();
				ChatHandler(player->GetSession()).PSendSysMessage("En este momento hay %u jugadores en la Zona PvP.", online_area);
			}
			else if (newArea != AREA_PVP_ISLAND)
				CharacterDatabase.Execute("UPDATE characters SET area=0 WHERE guid='%u'", player->GetGUID().GetRawValue());
		}
		
		/*void OnPVPHeal(Player* healer, Unit* healed, uint32 healAmount)
        {
			if (healer->GetAreaId() == AREA_PVP_ISLAND)
            {
                if (healed->GetAreaId() == AREA_PVP_ISLAND)
                {
					// Evitar la racha con brujos, guerreros, rogues, dks, hunters y magos (no curan)
					if (healer->getClass() == CLASS_WARLOCK ||
					healer->getClass() == CLASS_WARRIOR ||
					healer->getClass() == CLASS_ROGUE ||
					healer->getClass() == CLASS_DEATH_KNIGHT ||
					healer->getClass() == CLASS_HUNTER ||
					healer->getClass() == CLASS_MAGE)
						return;

					if (!healed || !healer)
						return;
		 
					// If it's for example a (bugged) area-heal that also heals enemies we should not count this for the quest
					if (!healed->IsFriendlyTo(healer))
						return;
		 
					if (healed->isPet() || healed->GetTypeId() == TYPEID_PLAYER)
					{					
						int32 sanacion = Getsanacion(healer);
						int32 sanacion2 = Getsanacion2(healer);
						if (sanacion == 0) //Si el healer lleva 0 de sanacion -> Se hace un replace (como insert into)
							QueryResult inicio = CharacterDatabase.PQuery("REPLACE INTO racha_sanacion(guid, sanacion, sanacion2) VALUES ('%u', '%u', 1)", healer->GetGUID(), healAmount);
						else	//Si el healer lleva algo de sanacion, se hace un update sumando la cantidad
						{
							QueryResult inicio = CharacterDatabase.PQuery("UPDATE racha_sanacion SET sanacion = sanacion+'%u' WHERE guid='%u'", healAmount, healer->GetGUID());
								sanacion++;
						}

						//Creamos los iconos por faccion
						char* icono_healer;
						if(healer->GetTeamId() == ALLIANCE)
							icono_healer = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
						else
							icono_healer = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";
						
						//Creamos los colores custom
						//Colores healer
						char* color_healer;
						if(healer->getClass() == CLASS_WARRIOR)
							color_healer = "|CFFC79C6E";
						else if (healer->getClass() == CLASS_DEATH_KNIGHT)
							color_healer = "|CFFC41F3B";
						else if (healer->getClass() == CLASS_DRUID)
							color_healer = "|CFFFF7D0A";
						else if (healer->getClass() == CLASS_HUNTER)
							color_healer = "|CFFABD473";
						else if (healer->getClass() == CLASS_MAGE)
							color_healer = "|CFF69CCF0";
						else if (healer->getClass() == CLASS_PALADIN)
							color_healer = "|CFFF58CBA";
						else if (healer->getClass() == CLASS_PRIEST)
							color_healer = "|CFFFFFFFF";
						else if (healer->getClass() == CLASS_ROGUE)
							color_healer = "|CFFFFF569";
						else if (healer->getClass() == CLASS_SHAMAN)
							color_healer = "|CFF0070DE";
						else if (healer->getClass() == CLASS_WARLOCK)
							color_healer = "|CFF9482C9";

						if(sanacion>=150000 && sanacion2==1)
						{
							healer->ModifyHonorPoints(+2500);
							ChatHandler(healer).PSendSysMessage("Has obtenido |CFF87CEFA2500|r puntos de Honor EXTRA y |CFF87CEFA3|r |CFFff8000[Token Zona PvP]|r por tu racha de sanacion de %u.", sanacion);
							sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000lleva una racha de sanación de|r |cFFFFD700%u|r |cFFFF0000sin morir!|r", icono_healer, color_healer, healer->GetName(), sanacion);
							QueryResult inicio = CharacterDatabase.PQuery("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
							healer->AddItem(32569, 3);
						}

						if(sanacion>=250000 && sanacion2==2)
						{
							healer->ModifyHonorPoints(+4000);
							ChatHandler(healer).PSendSysMessage("Has obtenido |CFF87CEFA4000|r puntos de Honor EXTRA y |CFF87CEFA6|r |CFFff8000[Token Zona PvP]|r por tu racha de sanacion de %u.", sanacion);
							sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000lleva una racha de sanación de|r |cFFFFD700%u|r |cFFFF0000sin morir!|r", icono_healer, color_healer, healer->GetName(), sanacion);
							QueryResult inicio = CharacterDatabase.PQuery("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
							healer->AddItem(32569, 6);
						}

						if(sanacion>=400000 && sanacion2==3)
						{
							healer->ModifyHonorPoints(+6500);
							ChatHandler(healer).PSendSysMessage("Has obtenido |CFF87CEFA6500|r puntos de Honor EXTRA y |CFF87CEFA12|r |CFFff8000[Token Zona PvP]|r por tu racha de sanacion de %u.", sanacion);
							sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000lleva una racha de sanación de|r |cFFFFD700%u|r |cFFFF0000sin morir!|r", icono_healer, color_healer, healer->GetName(), sanacion);
							QueryResult inicio = CharacterDatabase.PQuery("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
							healer->AddItem(32569, 12);
						}

						if(sanacion>=570000 && sanacion2==4)
						{
							healer->ModifyHonorPoints(+9000);
							ChatHandler(healer).PSendSysMessage("Has obtenido |CFF87CEFA9000|r puntos de Honor EXTRA y |CFF87CEFA25|r |CFFff8000[Token Zona PvP]|r por tu racha de sanacion de %u.", sanacion);
							sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000lleva una racha de sanación de|r |cFFFFD700%u|r |cFFFF0000sin morir!|r", icono_healer, color_healer, healer->GetName(), sanacion);
							QueryResult inicio = CharacterDatabase.PQuery("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
							healer->AddItem(32569, 25);
						}
						
						if(sanacion>=760000 && sanacion2==5)
						{
							healer->ModifyHonorPoints(+12000);
							ChatHandler(healer).PSendSysMessage("Has obtenido |CFF87CEFA12000|r puntos de Honor EXTRA, |CFF87CEFA50|r |CFFff8000[Token Zona PvP]|r por tu racha de sanacion de %u y |CFF87CEFA1|r |CFFff8000[Insignia del Valeroso]|r.", sanacion);
							sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000lleva una racha de sanación de|r |cFFFFD700%u|r |cFFFF0000sin morir!|r", icono_healer, color_healer, healer->GetName(), sanacion);
							QueryResult inicio = CharacterDatabase.PQuery("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
							healer->AddItem(32569, 50);
							healer->AddItem(15704, 1);
						}
					}
				}
			}
        }*/

		void OnPVPKill(Player * asesino, Player * muerto)
        {
            if (asesino->GetAreaId() == AREA_PVP_ISLAND)
            {
                if (muerto->GetAreaId() == AREA_PVP_ISLAND)
                {
                    uint64 killerGUID = asesino->GetGUID().GetRawValue();
                    uint64 victimGUID = muerto->GetGUID().GetRawValue();


					Group* killerGroup = asesino->GetGroup();

                    // If player killed himself, do not execute any code (think of when a warlock uses Hellfire, when player falls to dead, etc.)
                    if (killerGUID == victimGUID)
                    {
						QueryResult limpiar = CharacterDatabase.Query("DELETE FROM racha WHERE guid='%u'", muerto->GetGUID().GetRawValue());
						//QueryResult limpiar2 = CharacterDatabase.PQuery("DELETE FROM racha_sanacion WHERE guid='%u'", muerto->GetGUID());

                        muerto->m_Events.AddEvent(new pvp_island_resurrect_event(muerto), muerto->m_Events.CalculateTime(250));
 
                        if (muerto->getClass() == CLASS_HUNTER || muerto->getClass() == CLASS_WARLOCK)
                            muerto->m_Events.AddEvent(new pvp_island_resurrect_event_pet(muerto), muerto->m_Events.CalculateTime(1500));
                        return;
                    }
					
					if (killerGUID != victimGUID)
					{
						if(killerGroup)
						{
							for(GroupReference* itr = killerGroup->GetFirstMember(); itr != NULL; itr = itr->next()) 
							{
								if(itr->GetSource()->IsAtGroupRewardDistance(muerto))
								{
									itr->GetSource()->AddItem(32569, 1);
								}
							}
						}
						else if(!killerGroup)
							asesino->AddItem(32569, 1);
					}

					// This will cause the victim to be resurrected, teleported and health set to 100% after 1 second of dieing
                    muerto->m_Events.AddEvent(new pvp_island_resurrect_event(muerto), muerto->m_Events.CalculateTime(250));
 
                    // This will cause warlocks and hunters to have their last-used pet to be re-summoned when arriving on the island
                    if (muerto->getClass() == CLASS_HUNTER || muerto->getClass() == CLASS_WARLOCK)
                        muerto->m_Events.AddEvent(new pvp_island_resurrect_event_pet(muerto), muerto->m_Events.CalculateTime(1500));

					//Creamos los iconos por faccion, asesino
					const char* icono_asesino;
					if(asesino->GetTeamId() == ALLIANCE)
						icono_asesino = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
					else
						icono_asesino = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

					//Creamos los iconos por faccion, muerto
					const char* icono_muerto;
					if(muerto->GetTeamId() == ALLIANCE)
						icono_muerto = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
					else
						icono_muerto = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";
							
					//Creamos los colores custom
					//Colores asesino
                    const char* color_asesino;
					if(asesino->getClass() == CLASS_WARRIOR)
						color_asesino = "|CFFC79C6E";
					else if (asesino->getClass() == CLASS_DEATH_KNIGHT)
						color_asesino = "|CFFC41F3B";
					else if (asesino->getClass() == CLASS_DRUID)
						color_asesino = "|CFFFF7D0A";
					else if (asesino->getClass() == CLASS_HUNTER)
						color_asesino = "|CFFABD473";
					else if (asesino->getClass() == CLASS_MAGE)
						color_asesino = "|CFF69CCF0";
					else if (asesino->getClass() == CLASS_PALADIN)
						color_asesino = "|CFFF58CBA";
					else if (asesino->getClass() == CLASS_PRIEST)
						color_asesino = "|CFFFFFFFF";
					else if (asesino->getClass() == CLASS_ROGUE)
						color_asesino = "|CFFFFF569";
					else if (asesino->getClass() == CLASS_SHAMAN)
						color_asesino = "|CFF0070DE";
					else if (asesino->getClass() == CLASS_WARLOCK)
						color_asesino = "|CFF9482C9";

					//Colores muerto
					const char* color_muerto;
					if(muerto->getClass() == CLASS_WARRIOR)
						color_muerto = "|CFFC79C6E";
					else if (muerto->getClass() == CLASS_DEATH_KNIGHT)
						color_muerto = "|CFFC41F3B";
					else if (muerto->getClass() == CLASS_DRUID)
						color_muerto = "|CFFFF7D0A";
					else if (muerto->getClass() == CLASS_HUNTER)
						color_muerto = "|CFFABD473";
					else if (muerto->getClass() == CLASS_MAGE)
						color_muerto = "|CFF69CCF0";
					else if (muerto->getClass() == CLASS_PALADIN)
						color_muerto = "|CFFF58CBA";
					else if (muerto->getClass() == CLASS_PRIEST)
						color_muerto = "|CFFFFFFFF";
					else if (muerto->getClass() == CLASS_ROGUE)
						color_muerto = "|CFFFFF569";
					else if (muerto->getClass() == CLASS_SHAMAN)
						color_muerto = "|CFF0070DE";
					else if (muerto->getClass() == CLASS_WARLOCK)
						color_muerto = "|CFF9482C9";

					int32 muertes = Getmuertes(asesino);	// Calcula el numero de muertes del asesino
					int32 muertes2 = Getmuertes(muerto);	//Calcula el numero de muertes del aseseino
					/*int32 sanacion = Getsanacion(muerto);	//Calcula la sanacion que llevaba el que acaba de morir
					int32 sanacion2 = Getsanacion2(muerto);*/	//Calculamos el numero sanacion2 para mayor comprobacion al matar a alguien en racha

					if (muertes == 0) //Si el asesino lleva 0 muertes -> Se hace un replace (como insert into)
						QueryResult inicio = CharacterDatabase.Query("REPLACE INTO racha(guid, muertes) VALUES ('%u', 1)", asesino->GetGUID().GetRawValue());
					else	//Si el asesino lleva alguna muerte, se hace un update sumando una muerte más
					{
						QueryResult inicio = CharacterDatabase.Query("UPDATE racha SET muertes = muertes+1 WHERE guid='%u'", asesino->GetGUID().GetRawValue());
							muertes++;
					}

					if (muertes2>0/* || sanacion>0*/)	//Si el que es asesinado tiene alguna muerte se le borran
					{
						QueryResult limpiar = CharacterDatabase.Query("DELETE FROM racha WHERE guid='%u'", muerto->GetGUID().GetRawValue());
						//QueryResult limpiar2 = CharacterDatabase.PQuery("DELETE FROM racha_sanacion WHERE guid='%u'", muerto->GetGUID());
					}

					if (muertes2>=5 && muertes2<=9)	//Si el que es asesinado está en una racha de muertes, se premia al asesino	
					{
						asesino->ModifyHonorPoints(+600);
						ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado con la racha de asesinatos de: %s%s%s|r. Has obtenido |CFF87CEFA600|r puntos de Honor EXTRA y |CFF87CEFA3|r |CFFff8000[Token Zona PvP]|r por matarle.", icono_muerto, color_muerto, muerto->GetName());
						sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000ha acabado con la racha de asesinatos de|r %s%s%s|r|cFFFF0000!|r", icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
						asesino->AddItem(32569, 3);
					}

					if (muertes2>=10 && muertes2<=24)	//Si el que es asesinado está en una racha de muertes, se premia al asesino	
					{
						asesino->ModifyHonorPoints(+1200);
						ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado con la racha de asesinatos de: %s%s%s|r. Has obtenido |CFF87CEFA1200|r puntos de Honor EXTRA y |CFF87CEFA7|r |CFFff8000[Token Zona PvP]|r por matarle.", icono_muerto, color_muerto, muerto->GetName());
						sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000ha acabado con la racha de asesinatos de|r %s%s%s|r|cFFFF0000!|r", icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
						asesino->AddItem(32569, 7);
					}

					if (muertes2>=25)	//Si el que es asesinado está en una racha de muertes, se premia al asesino	
					{
						asesino->ModifyHonorPoints(+2200);
						ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado con la racha de asesinatos de: %s%s%s|r. Has obtenido |CFF87CEFA2200|r puntos de Honor EXTRA y |CFF87CEFA15|r |CFFff8000[Token Zona PvP]|r por matarle.", icono_muerto, color_muerto, muerto->GetName());
						sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000ha acabado con la racha de asesinatos de|r %s%s%s|r|cFFFF0000!|r", icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
						asesino->AddItem(32569, 15);
					}

					if(muertes>=5)	//Si el asesino mata a más de 5 personas inicia la racha 100p de Honor por muerte
					{
						asesino->ModifyHonorPoints(+150);
						asesino->GetSession()->SendAreaTriggerMessage("Llevas una racha de %u asesinatos", muertes);
						sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000lleva una racha de|r |cFFFFD700%u|r |cFFFF0000asesinatos!|r", icono_asesino, color_asesino, asesino->GetName(), muertes);
					}

					/*if (sanacion>=50000 && sanacion<100000 && sanacion2<3)	//Si el que es asesinado está en una racha de sanacion, se premia al asesino	
					{
						asesino->ModifyHonorPoints(+300);
						ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado la racha de sanacion de: %s%s%s|r. Has obtenido |CFF87CEFA300|r puntos de Honor EXTRA y |CFF87CEFA3|r |CFFff8000[Token Zona PvP]|r por matarle.", icono_muerto, color_muerto, muerto->GetName());
						sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000ha acabado con la racha de sanación de|r %s%s%s|r|cFFFF0000!|r", icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
						asesino->AddItem(32569, 3);
					}

					if (sanacion>=100000 && sanacion<300000 && sanacion2<4)	//Si el que es asesinado está en una racha de sanacion, se premia al asesino	
					{
						asesino->ModifyHonorPoints(+300);
						ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado la racha de sanacion de: %s%s%s|r. Has obtenido |CFF87CEFA300|r puntos de Honor EXTRA y |CFF87CEFA5|r |CFFff8000[Token Zona PvP]|r por matarle.", icono_muerto, color_muerto, muerto->GetName());
						sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000ha acabado con la racha de sanación de|r %s%s%s|r|cFFFF0000!|r", icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
						asesino->AddItem(32569, 5);
					}

					if (sanacion>=300000 && sanacion2<7)	//Si el que es asesinado está en una racha de sanacion, se premia al asesino	
					{
						asesino->ModifyHonorPoints(+300);
						ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado la racha de sanacion de: %s%s%s|r. Has obtenido |CFF87CEFA300|r puntos de Honor EXTRA y |CFF87CEFA15|r |CFFff8000[Token Zona PvP]|r por matarle.", icono_muerto, color_muerto, muerto->GetName());
						sWorld->SendAreaText(368, "|cFFFF0000¡|r%s%s%s |cFFFF0000ha acabado con la racha de sanación de|r %s%s%s|r|cFFFF0000!|r", icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
						asesino->AddItem(32569, 15);
					}*/

					switch(muertes)		//El asesino recibe Honor e Items por muertes
					{
						case 5:
							asesino->ModifyHonorPoints(+500);
							ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA500|r puntos de Honor EXTRA y |CFF87CEFA2|r |CFFff8000[Token Zona PvP]|r por tu racha de 5.");
							asesino->AddItem(32569, 2);
						break;
					
						case 10:
							asesino->ModifyHonorPoints(+2000);
							ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA2000|r puntos de Honor EXTRA y |CFF87CEFA5|r |CFFff8000[Token Zona PvP]|r por tu racha de 10.");
							asesino->AddItem(32569, 5);
						break;
					
						case 15:
							asesino->ModifyHonorPoints(+2500);
							ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA2500|r puntos de Honor EXTRA y |CFF87CEFA10|r |CFFff8000[Token Zona PvP]|r por tu racha de 15.");
							asesino->AddItem(32569, 10);
						break;

						case 20:
							asesino->ModifyHonorPoints(+6000);
							ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA6000|r puntos de Honor EXTRA y |CFF87CEFA15|r |CFFff8000[Token Zona PvP]|r por tu racha de 20.");
							asesino->AddItem(32569, 15);
						break;

						case 25:
							asesino->ModifyHonorPoints(+8000);
							ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA8000|r puntos de Honor EXTRA y |CFF87CEFA25|r |CFFff8000[Token Zona PvP]|r por tu racha de 25 y |CFF87CEFA1|r |CFFff8000[Insignia del Valeroso]|r.");
							asesino->AddItem(32569, 25);
							asesino->AddItem(15704, 1);
						break;
					}
                }
            }
        }
};
 
class npc_summon_curar_1 : public CreatureScript
{
    public:
        npc_summon_curar_1() : CreatureScript("npc_summon_curar_1") { }
 
        struct npc_summon_curar_1AI : public ScriptedAI
        {
            npc_summon_curar_1AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Curar_1;
			
			void Reset()
            {
                summonTimer_Curar_1 = urand(40000, 70000);
            }

            void UpdateAI(uint32 uiDiff)
            {
				if (!curar_1)
				{
					if(summonTimer_Curar_1<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_FOOD_1, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFF00FF00Sanaci""\xC3\xB3""n|R|CFF87CEFA!");
						summonTimer_Curar_1 = urand(40000, 70000);
						curar_1=true;
					}
					else summonTimer_Curar_1-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_curar_1AI(creature);
        }
};

class npc_summon_curar_2 : public CreatureScript
{
    public:
        npc_summon_curar_2() : CreatureScript("npc_summon_curar_2") { }
 
        struct npc_summon_curar_2AI : public ScriptedAI
        {
            npc_summon_curar_2AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Curar_2;
 
 			void Reset()
            {
                summonTimer_Curar_2 = urand(40000, 70000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!curar_2)
				{
					if(summonTimer_Curar_2<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_FOOD_2, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFF00FF00Sanaci""\xC3\xB3""n|R|CFF87CEFA!");
						summonTimer_Curar_2 = urand(40000, 70000);
						curar_2=true;
					}
					else summonTimer_Curar_2-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_curar_2AI(creature);
        }
};

class npc_summon_curar_3 : public CreatureScript
{
    public:
        npc_summon_curar_3() : CreatureScript("npc_summon_curar_3") { }
 
        struct npc_summon_curar_3AI : public ScriptedAI
        {
            npc_summon_curar_3AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Curar_3;
 
 			void Reset()
            {
                summonTimer_Curar_3 = urand(40000, 70000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!curar_3)
				{
					if(summonTimer_Curar_3<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_FOOD_3, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFF00FF00Sanaci""\xC3\xB3""n|R|CFF87CEFA!");
						summonTimer_Curar_3 = urand(40000, 70000);
						curar_3=true;
					}
					else summonTimer_Curar_3-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_curar_3AI(creature);
        }
};

class npc_summon_curar_4 : public CreatureScript
{
    public:
        npc_summon_curar_4() : CreatureScript("npc_summon_curar_4") { }
 
        struct npc_summon_curar_4AI : public ScriptedAI
        {
            npc_summon_curar_4AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Curar_4;
 
 			void Reset()
            {
                summonTimer_Curar_4 = urand(40000, 70000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!curar_4)
				{
					if(summonTimer_Curar_4<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_FOOD_4, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFF00FF00Sanaci""\xC3\xB3""n|R|CFF87CEFA!");
						summonTimer_Curar_4 = urand(40000, 70000);
						curar_4=true;
					}
					else summonTimer_Curar_4-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_curar_4AI(creature);
        }
};

class npc_summon_rabiar_1 : public CreatureScript
{
    public:
        npc_summon_rabiar_1() : CreatureScript("npc_summon_rabiar_1") { }
 
        struct npc_summon_rabiar_1AI : public ScriptedAI
        {
            npc_summon_rabiar_1AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Rabiar_1;
 
 			void Reset()
            {
                summonTimer_Rabiar_1 = urand(80000, 170000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!rabiar_1)
				{
					if(summonTimer_Rabiar_1<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_BERSERKING_1, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!");
						summonTimer_Rabiar_1 = urand(80000, 170000);
						rabiar_1=true;
					}
					else summonTimer_Rabiar_1-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_rabiar_1AI(creature);
        }
};

class npc_summon_rabiar_2 : public CreatureScript
{
    public:
        npc_summon_rabiar_2() : CreatureScript("npc_summon_rabiar_2") { }
 
        struct npc_summon_rabiar_2AI : public ScriptedAI
        {
            npc_summon_rabiar_2AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Rabiar_2;
  
 			void Reset()
            {
                summonTimer_Rabiar_2 = urand(80000, 170000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!rabiar_2)
				{
					if(summonTimer_Rabiar_2<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_BERSERKING_2, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!");
						summonTimer_Rabiar_2 = urand(80000, 170000);
						rabiar_2=true;
					}
					else summonTimer_Rabiar_2-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_rabiar_2AI(creature);
        }
};

class npc_summon_rabiar_3 : public CreatureScript
{
    public:
        npc_summon_rabiar_3() : CreatureScript("npc_summon_rabiar_3") { }
 
        struct npc_summon_rabiar_3AI : public ScriptedAI
        {
            npc_summon_rabiar_3AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Rabiar_3;
  
 			void Reset()
            {
                summonTimer_Rabiar_3 = urand(80000, 170000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!rabiar_3)
				{
					if(summonTimer_Rabiar_3<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_BERSERKING_3, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!");
						summonTimer_Rabiar_3 = urand(80000, 170000);
						rabiar_3 = true;
					}
					else summonTimer_Rabiar_3-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_rabiar_3AI(creature);
        }
};

class npc_summon_rabiar_4 : public CreatureScript
{
    public:
        npc_summon_rabiar_4() : CreatureScript("npc_summon_rabiar_4") { }
 
        struct npc_summon_rabiar_4AI : public ScriptedAI
        {
            npc_summon_rabiar_4AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Rabiar_4;
  
 			void Reset()
            {
                summonTimer_Rabiar_4 = urand(80000, 170000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!rabiar_4)
				{
					if(summonTimer_Rabiar_4<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_BERSERKING_4, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!");
						summonTimer_Rabiar_4 = urand(80000, 170000);
						rabiar_4 = true;
					}
					else summonTimer_Rabiar_4-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_rabiar_4AI(creature);
        }
};

class npc_summon_rabiar_5 : public CreatureScript
{
    public:
        npc_summon_rabiar_5() : CreatureScript("npc_summon_rabiar_5") { }
 
        struct npc_summon_rabiar_5AI : public ScriptedAI
        {
            npc_summon_rabiar_5AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Rabiar_5;
  
 			void Reset()
            {
                summonTimer_Rabiar_5 = urand(80000, 170000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!rabiar_5)
				{
					if(summonTimer_Rabiar_5<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_BERSERKING_5, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!");
						summonTimer_Rabiar_5 = urand(80000, 170000);
						rabiar_5 = true;
					}
					else summonTimer_Rabiar_5-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_rabiar_5AI(creature);
        }
};

class npc_summon_rabiar_6 : public CreatureScript
{
    public:
        npc_summon_rabiar_6() : CreatureScript("npc_summon_rabiar_6") { }
 
        struct npc_summon_rabiar_6AI : public ScriptedAI
        {
            npc_summon_rabiar_6AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Rabiar_6;
  
 			void Reset()
            {
                summonTimer_Rabiar_6 = urand(80000, 170000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!rabiar_6)
				{
					if(summonTimer_Rabiar_6<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_BERSERKING_6, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!");
						summonTimer_Rabiar_6 = urand(80000, 170000);
						rabiar_6 = true;
					}
					else summonTimer_Rabiar_6-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_rabiar_6AI(creature);
        }
};

class npc_summon_caldero_1 : public CreatureScript
{
    public:
        npc_summon_caldero_1() : CreatureScript("npc_summon_caldero_1") { }
 
        struct npc_summon_caldero_1AI : public ScriptedAI
        {
            npc_summon_caldero_1AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Caldero_1;
  
 			void Reset()
            {
                summonTimer_Caldero_1 = urand(120000, 180000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!caldero_1)
				{
					if(summonTimer_Caldero_1<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_ANCIENT_1, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un |CFF800080Caldero Ancestral|R|CFF87CEFA!");
						summonTimer_Caldero_1 = urand(120000, 180000);
						caldero_1=true;
					}
					else summonTimer_Caldero_1-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_caldero_1AI(creature);
        }
};

class npc_summon_caldero_2 : public CreatureScript
{
    public:
        npc_summon_caldero_2() : CreatureScript("npc_summon_caldero_2") { }
 
        struct npc_summon_caldero_2AI : public ScriptedAI
        {
            npc_summon_caldero_2AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Caldero_2;
   
 			void Reset()
            {
                summonTimer_Caldero_2 = urand(120000, 180000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!caldero_2)
				{
					if(summonTimer_Caldero_2<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_ANCIENT_2, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un |CFF800080Caldero Ancestral|R|CFF87CEFA!");
						summonTimer_Caldero_2 = urand(120000, 180000);
						caldero_2=true;
					}
					else summonTimer_Caldero_2-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_caldero_2AI(creature);
        }
};

class npc_summon_caldero_3 : public CreatureScript
{
    public:
        npc_summon_caldero_3() : CreatureScript("npc_summon_caldero_3") { }
 
        struct npc_summon_caldero_3AI : public ScriptedAI
        {
            npc_summon_caldero_3AI(Creature* creature) : ScriptedAI(creature) { }

			uint32 summonTimer_Caldero_3;
    
 			void Reset()
            {
                summonTimer_Caldero_3 = urand(120000, 180000);
            }
			
            void UpdateAI(uint32 uiDiff)
            {
				if (!caldero_3)
				{
					if(summonTimer_Caldero_3<=uiDiff)
					{
						me->SummonGameObject(GAMEOBJECT_ANCIENT_3, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), me->GetOrientation(), 0);
						sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""Se ha spawneado un |CFF800080Caldero Ancestral|R|CFF87CEFA!");
						summonTimer_Caldero_3 = urand(120000, 180000);
						caldero_3=true;
					}
					else summonTimer_Caldero_3-=uiDiff;
				}
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_summon_caldero_3AI(creature);
        }
};

class go_powerup_food_1 : public GameObjectScript
{
    public:
        go_powerup_food_1() : GameObjectScript("go_powerup_food_1") { }
		
        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_FOOD, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFF00FF00Sanaci""\xC3\xB3""n|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			curar_1=false;
            return false;
        }
};

class go_powerup_food_2 : public GameObjectScript
{
    public:
        go_powerup_food_2() : GameObjectScript("go_powerup_food_2") { }
		
        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_FOOD, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFF00FF00Sanaci""\xC3\xB3""n|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			curar_2=false;
            return false;
        }
};

class go_powerup_food_3 : public GameObjectScript
{
    public:
        go_powerup_food_3() : GameObjectScript("go_powerup_food_3") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_FOOD, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFF00FF00Sanaci""\xC3\xB3""n|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			curar_3=false;
            return false;
        }
};

class go_powerup_food_4 : public GameObjectScript
{
    public:
        go_powerup_food_4() : GameObjectScript("go_powerup_food_4") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_FOOD, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFF00FF00Sanaci""\xC3\xB3""n|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			curar_4=false;
            return false;
        }
};

class go_powerup_berserking_1 : public GameObjectScript
{
    public:
        go_powerup_berserking_1() : GameObjectScript("go_powerup_berserking_1") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_BERSERK, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			rabiar_1=false;
            return false;
        }
};

class go_powerup_berserking_2 : public GameObjectScript
{
    public:
        go_powerup_berserking_2() : GameObjectScript("go_powerup_berserking_2") { }

		uint32 DespawnTimer;
		
        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_BERSERK, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			rabiar_2=false;
            return false;
        }
};

class go_powerup_berserking_3 : public GameObjectScript
{
    public:
        go_powerup_berserking_3() : GameObjectScript("go_powerup_berserking_3") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_BERSERK, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			rabiar_3=false;
            return false;
        }
};

class go_powerup_berserking_4 : public GameObjectScript
{
    public:
        go_powerup_berserking_4() : GameObjectScript("go_powerup_berserking_4") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_BERSERK, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			rabiar_4=false;
            return false;
        }
};

class go_powerup_berserking_5 : public GameObjectScript
{
    public:
        go_powerup_berserking_5() : GameObjectScript("go_powerup_berserking_5") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_BERSERK, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			rabiar_5=false;
            return false;
        }
};

class go_powerup_berserking_6 : public GameObjectScript
{
    public:
        go_powerup_berserking_6() : GameObjectScript("go_powerup_berserking_6") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

            player->CastSpell(player, SPELL_BERSERK, false);
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un Buffo de |CFFFF0000Rabiar|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			rabiar_6=false;
            return false;
        }
};

class go_powerup_ancient_1 : public GameObjectScript
{
    public:
        go_powerup_ancient_1() : GameObjectScript("go_powerup_ancient_1") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

			ChatHandler handler(player->GetSession());
            player->CastSpell(player, SPELL_ANCIENT, false);
			player->RemoveArenaSpellCooldowns();
			player->RemoveAllSpellCooldown();
			handler.PSendSysMessage("|CFFFFD700Tiempo de reutilizaci""\xC3\xB3""n de tus habilidades reiniciado.");
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un |CFF800080Caldero Ancestral|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			caldero_1=false;
            return false;
        }
};

class go_powerup_ancient_2 : public GameObjectScript
{
    public:
        go_powerup_ancient_2() : GameObjectScript("go_powerup_ancient_2") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

			ChatHandler handler(player->GetSession());
            player->CastSpell(player, SPELL_ANCIENT, false);
			player->RemoveArenaSpellCooldowns();
			player->RemoveAllSpellCooldown();
			handler.PSendSysMessage("|CFFFFD700Tiempo de reutilizaci""\xC3\xB3""n de tus habilidades reiniciado.");
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un |CFF800080Caldero Ancestral|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			caldero_2=false;
            return false;
        }
};

class go_powerup_ancient_3 : public GameObjectScript
{
    public:
        go_powerup_ancient_3() : GameObjectScript("go_powerup_ancient_3") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			//Creamos los iconos por faccion
			const char* icono_jugador;
			if(player->GetTeamId() == ALLIANCE)
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
			else
				icono_jugador = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";

			//Creamos los colores custom
			//Colores jugador
			const char* color_jugador;
			if(player->getClass() == CLASS_WARRIOR)
				color_jugador = "|CFFC79C6E";
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
				color_jugador = "|CFFC41F3B";
			else if (player->getClass() == CLASS_DRUID)
				color_jugador = "|CFFFF7D0A";
			else if (player->getClass() == CLASS_HUNTER)
				color_jugador = "|CFFABD473";
			else if (player->getClass() == CLASS_MAGE)
				color_jugador = "|CFF69CCF0";
			else if (player->getClass() == CLASS_PALADIN)
				color_jugador = "|CFFF58CBA";
			else if (player->getClass() == CLASS_PRIEST)
				color_jugador = "|CFFFFFFFF";
			else if (player->getClass() == CLASS_ROGUE)
				color_jugador = "|CFFFFF569";
			else if (player->getClass() == CLASS_SHAMAN)
				color_jugador = "|CFF0070DE";
			else if (player->getClass() == CLASS_WARLOCK)
				color_jugador = "|CFF9482C9";

			ChatHandler handler(player->GetSession());
            player->CastSpell(player, SPELL_ANCIENT, false);
			player->RemoveArenaSpellCooldowns();
			player->RemoveAllSpellCooldown();
			handler.PSendSysMessage("|CFFFFD700Tiempo de reutilizaci""\xC3\xB3""n de tus habilidades reiniciado.");
            go->RemoveFromWorld();
			sWorld->SendAreaText(368, "|CFF87CEFA""\xc2\xa1""%s%s%s |CFF87CEFAha utilizado un |CFF800080Caldero Ancestral|R|CFF87CEFA!", icono_jugador, color_jugador, player->GetName());
			caldero_3=false;
            return false;
        }
};

class go_piedra_de_hogar : public GameObjectScript
{
    public:
        go_piedra_de_hogar() : GameObjectScript("go_piedra_de_hogar") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
			player->RemoveSpellCooldown(8690, true);
            player->CastSpell(player, 8690, false);
            //go->RemoveFromWorld();
            return true;
        }
};
 
class npc_teleport_pvp_island : public CreatureScript
{
    public:
        npc_teleport_pvp_island() : CreatureScript("npc_teleport_pvp_island") { }
 
        struct npc_teleport_pvp_islandAI : public ScriptedAI
        {
            npc_teleport_pvp_islandAI(Creature* creature) : ScriptedAI(creature) { }
 
            void UpdateAI(uint32 diff)
            {
                if (Player* player = me->SelectNearestPlayer(1.0f))
                {
                    if (!player || player->IsBeingTeleported() || !player->IsAlive() || player->IsInCombat())
                        return;

					// Prevent Group
					if (Group* group = player->GetGroup())
					{
						if (group->GetMembersCount() > 2)
							ChatHandler(player->GetSession()).PSendSysMessage("Tu grupo no puede tener m""\xC3\xA1""s de 2 miembros en la Zona PvP.");
							group->Disband(true);
					}

					/*player->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_PVP);
					player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);*/

					//player->CastSpell(player, SPELL_SPIRITUAL_IMMUNITY, true);
 
                    int i = urand(0, MAX_PLAYER_SPAWN_POINTS - 1);
                    player->TeleportTo(1, playerSpawnPoint[i].GetPositionX(), playerSpawnPoint[i].GetPositionY(), playerSpawnPoint[i].GetPositionZ(), playerSpawnPoint[i].GetOrientation());
                }
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_teleport_pvp_islandAI(creature);
        }
};
 
void AddSC_isla_pvp()
{
    new pvp_island();
	new npc_teleport_pvp_island();
	//NPCs
    new npc_summon_curar_1();
	new npc_summon_curar_2();
	new npc_summon_curar_3();
	new npc_summon_curar_4();
	new npc_summon_rabiar_1();
	new npc_summon_rabiar_2();
	new npc_summon_rabiar_3();
	new npc_summon_rabiar_4();
	new npc_summon_rabiar_5();
	new npc_summon_rabiar_6();
	new npc_summon_caldero_1();
	new npc_summon_caldero_2();
	new npc_summon_caldero_3();
	//GOBs
	new go_powerup_food_1();
	new go_powerup_food_2();
	new go_powerup_food_3();
	new go_powerup_food_4();
    new go_powerup_berserking_1();
	new go_powerup_berserking_2();
	new go_powerup_berserking_3();
	new go_powerup_berserking_4();
	new go_powerup_berserking_5();
	new go_powerup_berserking_6();
    new go_powerup_ancient_1();
	new go_powerup_ancient_2();
	new go_powerup_ancient_3();
	new go_piedra_de_hogar();
}
