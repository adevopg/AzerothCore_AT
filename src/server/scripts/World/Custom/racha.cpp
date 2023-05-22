/*******************************************************************************************
* Créditos: Khanx "El sabio" y Ray                                                            *
********************************************************************************************/
/*
CREATE TABLE `racha` (
  `guid` int(10) unsigned NOT NULL,
  `muertes` int(5) unsigned NOT NULL DEFAULT '0',
  `maxmuertes` int(5) unsigned DEFAULT '0',
  PRIMARY KEY (`guid`)
);
CREATE TABLE `racha_sanacion` (
  `guid` int(10) unsigned NOT NULL,
  `sanacion` int(10) unsigned NOT NULL DEFAULT '0',
  `sanacion2` int(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
);
*/
/*
En /src/server/game/Server/Protocol/Handlers/CharacterHandler.cpp:
1- Buscar void WorldSession::HandlePlayerLogin(LoginQueryHolder * holder)
2- Añadir:
	CharacterDatabase.PExecute("DELETE FROM racha WHERE guid='%u'", pCurrChar->GetGUID());
	CharacterDatabase.PExecute("DELETE FROM racha_sanacion WHERE guid='%u'", pCurrChar->GetGUID());
*/
/*
Nota: Para acabar la racha de muertes al salir de BG:

1- Abrir: /src/server/game/Maps/Map.cpp
2- Buscar: BattlegroundMap::Remove
3- Añadir: 
	QueryResult limpiar = CharacterDatabase.Query("DELETE FROM racha  WHERE guid='%u'", player->GetGUID());
	QueryResult limpiar = CharacterDatabase.Query("DELETE FROM racha_sanacion  WHERE guid='%u'", player->GetGUID());
*/
#include "ScriptPCH.h"
#include "Language.h"
#include "Unit.h"
#include "World.h"

int Getmuertes(Player * player)
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

int Getsanacion(Player * player)
{
	QueryResult cuenta2 = CharacterDatabase.Query("SELECT sanacion FROM racha_sanacion WHERE guid = '%u'", player->GetGUID().GetRawValue());

	if (!cuenta2)
		return 0;
	else
	{
		Field *fields = cuenta2->Fetch();
			return fields[0].Get<uint32>();
	}
}

int Getsanacion2(Player * player)
{
	QueryResult cuenta3 = CharacterDatabase.Query("SELECT sanacion2 FROM racha_sanacion WHERE guid = '%u'", player->GetGUID().GetRawValue());

	if (!cuenta3)
		return 1;
	else
	{
		Field *fields = cuenta3->Fetch();
			return fields[0].Get<uint32>();
	}
}

class Racha_de_Muertes : public PlayerScript
{

    public:
		 Racha_de_Muertes()
			 : PlayerScript("Racha_de_Muertes"){}

		void OnPVPHeal(Player* healer, Unit* healed, uint32 healAmount)
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
 
			if (healed->IsPet() || healed->GetTypeId() == TYPEID_PLAYER)
			{
				// Solo en BG y Zona PVP
				Battleground* bg = healer->GetBattleground();
				if (!bg || bg->Is1vs1())
				{
					//QueryResult limpiar = CharacterDatabase.Query("DELETE FROM racha WHERE guid='%u'", healer->GetGUID());
					return;
				}
			
				int32 sanacion = Getsanacion(healer);
				int32 sanacion2 = Getsanacion2(healer);
				if (sanacion == 0) //Si el healer lleva 0 de sanacion -> Se hace un replace (como insert into)
					QueryResult inicio = CharacterDatabase.Query("REPLACE INTO racha_sanacion(guid, sanacion, sanacion2) VALUES ('%u', '%u', 1)", healer->GetGUID(), healAmount);
				else	//Si el healer lleva algo de sanacion, se hace un update sumando la cantidad
				{
					QueryResult inicio = CharacterDatabase.Query("UPDATE racha_sanacion SET sanacion = sanacion+'%u' WHERE guid='%u'", healAmount, healer->GetGUID());
						sanacion++;
				}

				//Creamos los iconos por faccion
				const char* icono_healer;
				if(healer->GetTeamId() == ALLIANCE)
					icono_healer = "|TInterface\\PVPFrame\\PVP-Currency-Alliance:18:18|t";
				else
					icono_healer = "|TInterface\\PVPFrame\\PVP-Currency-Horde:18:18|t";
				
				//Creamos los colores custom
				//Colores healer
				const char* color_healer;
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
					ChatHandler(healer->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA2500|r puntos de Honor EXTRA por tu racha de sanacion de %u.", sanacion);
					bg->SendWarningToAll(LANG_RACHA_SANACION, icono_healer, color_healer, healer->GetName(), sanacion);
					QueryResult inicio = CharacterDatabase.Query("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
				}

				if(sanacion>=250000 && sanacion2==2)
				{
					healer->ModifyHonorPoints(+4000);
					ChatHandler(healer->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA4000|r puntos de Honor EXTRA por tu racha de sanacion de %u.", sanacion);
					bg->SendWarningToAll(LANG_RACHA_SANACION, icono_healer, color_healer, healer->GetName(), sanacion);
					QueryResult inicio = CharacterDatabase.Query("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
				}

				if(sanacion>=400000 && sanacion2==3)
				{
					healer->ModifyHonorPoints(+6500);
					ChatHandler(healer->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA6500|r puntos de Honor EXTRA por tu racha de sanacion de %u.", sanacion);
					bg->SendWarningToAll(LANG_RACHA_SANACION, icono_healer, color_healer, healer->GetName(), sanacion);
					QueryResult inicio = CharacterDatabase.Query("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
				}

				if(sanacion>=570000 && sanacion2==4)
				{
					healer->ModifyHonorPoints(+9000);
					ChatHandler(healer->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA9000|r puntos de Honor EXTRA por tu racha de sanacion de %u.", sanacion);
					bg->SendWarningToAll(LANG_RACHA_SANACION, icono_healer, color_healer, healer->GetName(), sanacion);
					QueryResult inicio = CharacterDatabase.Query("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
				}
				
				if(sanacion>=760000 && sanacion2==5)
				{
					healer->ModifyHonorPoints(+12000);
					ChatHandler(healer->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA12000|r puntos de Honor EXTRA por tu racha de sanacion de %u.", sanacion);
					bg->SendWarningToAll(LANG_RACHA_SANACION, icono_healer, color_healer, healer->GetName(), sanacion);
					QueryResult inicio = CharacterDatabase.Query("UPDATE racha_sanacion SET sanacion2 = sanacion2+1 WHERE guid='%u'", healer->GetGUID());
				}
			}
        }

		void OnPVPKill(Player * asesino, Player * muerto)
		{
			// Solo en BG y zona PVP
			Battleground* bg = asesino->GetBattleground();
			if (!bg || bg->Is1vs1())
			{
				//QueryResult limpiar = CharacterDatabase.Query("DELETE FROM racha WHERE guid='%u'", muerto->GetGUID());
				return;
			}

			if (asesino == muerto) //Suicidarse no aumenta la cantidad de asesinatos
			{
				QueryResult limpiar = CharacterDatabase.Query("DELETE FROM racha WHERE guid='%u'", muerto->GetGUID());
				QueryResult limpiar2 = CharacterDatabase.Query("DELETE FROM racha_sanacion WHERE guid='%u'", muerto->GetGUID());
				return;
			}

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
			int32 sanacion = Getsanacion(muerto);	//Calcula la sanacion que llevaba el que acaba de morir
			int32 sanacion2 = Getsanacion2(muerto);	//Calculamos el numero sanacion2 para mayor comprobacion al matar a alguien en racha

			if (muertes == 0) //Si el asesino lleva 0 muertes -> Se hace un replace (como insert into)
				QueryResult inicio = CharacterDatabase.Query("REPLACE INTO racha(guid, muertes) VALUES ('%u', 1)", asesino->GetGUID());
			else	//Si el asesino lleva alguna muerte, se hace un update sumando una muerte más
			{
				QueryResult inicio = CharacterDatabase.Query("UPDATE racha SET muertes = muertes+1 WHERE guid='%u'", asesino->GetGUID());
					muertes++;
			}

			if (muertes2>0 || sanacion>0)	//Si el que es asesinado tiene alguna muerte se le borran
			{
				QueryResult limpiar = CharacterDatabase.Query("DELETE FROM racha WHERE guid='%u'", muerto->GetGUID());
				QueryResult limpiar2 = CharacterDatabase.Query("DELETE FROM racha_sanacion WHERE guid='%u'", muerto->GetGUID());
			}
	
			if (muertes2>=5 && muertes2<=9)	//Si el que es asesinado está en una racha de muertes, se premia al asesino	
			{
				asesino->ModifyHonorPoints(+600);
				ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado con la racha de asesinatos de: %s%s%s|r. Has obtenido |CFF87CEFA600|r puntos de Honor EXTRA por matarle.", icono_muerto, color_muerto, muerto->GetName());
				bg->SendWarningToAll(LANG_RACHA_PARADA, icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
			}

			if (muertes2>=10 && muertes2<=24)	//Si el que es asesinado está en una racha de muertes, se premia al asesino	
			{
				asesino->ModifyHonorPoints(+1200);
				ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado con la racha de asesinatos de: %s%s%s|r. Has obtenido |CFF87CEFA1200|r puntos de Honor EXTRA por matarle.", icono_muerto, color_muerto, muerto->GetName());
				bg->SendWarningToAll(LANG_RACHA_PARADA, icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
			}

			if (muertes2>=25)	//Si el que es asesinado está en una racha de muertes, se premia al asesino	
			{
				asesino->ModifyHonorPoints(+2200);
				ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado con la racha de asesinatos de: %s%s%s|r. Has obtenido |CFF87CEFA2200|r puntos de Honor EXTRA por matarle.", icono_muerto, color_muerto, muerto->GetName());
				bg->SendWarningToAll(LANG_RACHA_PARADA, icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
			}

			if(muertes>=5)	//Si el asesino mata a más de 5 personas inicia la racha 100p de Honor por muerte
			{
				asesino->ModifyHonorPoints(+150);
				asesino->GetSession()->SendAreaTriggerMessage("Llevas una racha de %u asesinatos", muertes);
				bg->SendWarningToAll(LANG_RACHA_ASESINO, icono_asesino, color_asesino, asesino->GetName(), muertes);
			}

			if (sanacion>=30000 && sanacion2>=2)	//Si el que es asesinado está en una racha de sanacion, se premia al asesino	
			{
				asesino->ModifyHonorPoints(+300);
				ChatHandler(asesino->GetSession()).PSendSysMessage("Has acabado la racha de sanacion de: %s%s%s|r. Has obtenido |CFF87CEFA300|r puntos de Honor EXTRA por matarle.", icono_muerto, color_muerto, muerto->GetName());
				bg->SendWarningToAll(LANG_RACHA_PARADA_SANACION, icono_asesino, color_asesino, asesino->GetName(), icono_muerto, color_muerto, muerto->GetName());
			}

			switch(muertes)		//El asesino recibe Honor e Items por muertes
			{
				case 5:
					asesino->ModifyHonorPoints(+500);
					ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA500|r puntos de Honor EXTRA por tu racha de 5.");
				break;
			
				case 10:
					asesino->ModifyHonorPoints(+2000);
					ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA2000|r puntos de Honor EXTRA por tu racha de 10.");
				break;
			
				case 15:
					asesino->ModifyHonorPoints(+2500);
					ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA2500|r puntos de Honor EXTRA por tu racha de 15.");
				break;

				case 20:
					asesino->ModifyHonorPoints(+6000);
					ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA6000|r puntos de Honor EXTRA por tu racha de 20.");
				break;

				case 25:
					//asesino->AddItem(31337, 1);
					asesino->ModifyHonorPoints(+8000);
					ChatHandler(asesino->GetSession()).PSendSysMessage("Has obtenido |CFF87CEFA8000|r puntos de Honor EXTRA por tu racha de 25.");
				break;
			}
		}
		void OnPlayerLogin(Player* player)
		{
			CharacterDatabase.Execute("DELETE FROM racha WHERE guid='%u'", player->GetGUID());
		}
		void OnPlayerLogout(Player* player)
		{
			CharacterDatabase.Execute("DELETE FROM racha WHERE guid='%u'", player->GetGUID());
		}
		 
};

void AddSC_Racha_de_Muertes()
{
   new Racha_de_Muertes;
}
