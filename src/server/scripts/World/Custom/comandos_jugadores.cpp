#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"
#include "Common.h"
#include "ArenaTeamMgr.h"
#include "ArenaTeam.h"
#include "ObjectGuid.h"
	
    class comandos_jugadores : public CommandScript
    {
    public:
        comandos_jugadores() : CommandScript("comandos_jugadores") {}

        std::vector<ChatCommand> GetCommands() const override
        {
            static std::vector<ChatCommand> utilityCommandTable =
            {
                {"race", SEC_PLAYER, false, &HandleChangeRaceCommand, nullptr},
                {"faction", SEC_PLAYER, true, &HandleChangeFactionCommand, nullptr},
                 {"customize", SEC_PLAYER, true, &HandleCustomizeCommand, nullptr},
                  {"mmr", SEC_PLAYER, true, &HandleMMRCommand, nullptr},
            };

            static std::vector<ChatCommand> commandTable =
            {
                {"utility", SEC_PLAYER, true, nullptr, "", utilityCommandTable},
            };

            return commandTable;
        }

	static bool HandleutilityCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();

            me->Say("comando utilidad?", LANG_UNIVERSAL);
            return true;
    }

	static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		handler->PSendSysMessage("Reloguea para cambiar la raza de tu personaje.");
        return true;
    }

	static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		handler->PSendSysMessage("Reloguea para cambiar la faccion de tu personaje.");
        return true;
    }

	static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		handler->PSendSysMessage("Reloguea para customizar personaje.");
        return true;
    }

	static bool HandleMMRCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();

        // Revision de MMR de 2c2
        uint16 mmr1 = 0;
        {
            if (ArenaTeam* getmmr = sArenaTeamMgr->GetArenaTeamById(me->GetArenaTeamId(0)))
                mmr1 = getmmr->GetMember(me->GetGUID())->MatchMakerRating;
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MATCH_MAKER_RATING);
            stmt->SetData(0, me->GetGUID().GetCounter());
            stmt->SetData(1, 0);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if(result)
				mmr1 = (*result)[0].Get<uint32>();
		    else if (!mmr1)
			    mmr1 = sWorld->getIntConfig(CONFIG_ARENA_START_MATCHMAKER_RATING);
		}
		
		uint16 mmr2 = 0;
		{
			if(ArenaTeam* getmmr = sArenaTeamMgr->GetArenaTeamById(me->GetArenaTeamId(1)))
			    mmr2 = getmmr->GetMember(me->GetGUID())->MatchMakerRating;
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MATCH_MAKER_RATING);
			stmt->SetData(0, me->GetGUID().GetCounter());
			stmt->SetData(1, 1);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if(result)
				mmr2 = (*result)[0].Get<uint32>();
		    else if (!mmr2)
			    mmr2 = sWorld->getIntConfig(CONFIG_ARENA_START_MATCHMAKER_RATING);
		}
		
        uint16 mmr3 = 0; // FIX Warning, Inicializar mmr3 con un valor predeterminado, Podia provocar una incializacion indefinida aqui queda resuelto

        {
            CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MATCH_MAKER_RATING);
            stmt->SetData(0, me->GetGUID().GetCounter());
            stmt->SetData(1, 2);
            PreparedQueryResult result = CharacterDatabase.Query(stmt);

            if (result)
                mmr3 = (*result)[0].Get<uint32>();
            else if (!mmr3)
                mmr3 = sWorld->getIntConfig(CONFIG_ARENA_START_MATCHMAKER_RATING);
        }


		handler->PSendSysMessage("|CFFFFD700Tu MMR en 2c2 es de: |CFFFF0000%u", mmr1);
		handler->PSendSysMessage("|CFFFFD700Tu MMR en 3c3 es de: |CFFFF0000%u", mmr2);
		handler->PSendSysMessage("|CFFFFD700Tu MMR en 5c5 es de: |CFFFF0000%u", mmr3);
	    return true;
    }

    /* static bool HandleMMRCommand(ChatHandler* handler, const char* args)
    {
        Player* player = handler->GetSession()->GetPlayer();
        uint64 playerGuid = player->GetGUID().GetRawValue();

        uint16* MMR = new uint16[2];

        CharacterDatabasePreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MATCH_MAKER_RATING);
        stmt->SetData(0, playerGuid);

        for (int i = 0; i < 2; i++)
        {
            stmt->SetData(1, i);
            PreparedQueryResult result = CharacterDatabase.Query(stmt);

            if (result)
                MMR[i] = (*result)[0].Get<uint32>();
            else
                MMR[i] = sWorld->getIntConfig(CONFIG_ARENA_START_MATCHMAKER_RATING);
        }

        if (MMR[0] != 0)
            handler->PSendSysMessage("|CFFFFD700Tu MMR en 2c2 es de: |CFFFF0000%u", MMR[0]);
        else
            handler->PSendSysMessage("Error interno.");

        if (MMR[1] != 0)
            handler->PSendSysMessage("|CFFFFD700Tu MMR en 3c3 es de: |CFFFF0000%u", MMR[1]);
        else
            handler->PSendSysMessage("Error interno.");

        return true;
    }*/

};

void AddSC_comandos_jugadores()
{
    new comandos_jugadores();
}
