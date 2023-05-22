#include "ScriptPCH.h"
#include <cstring>
#include <string.h>
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "World.h"
#include "ObjectGuid.h"

typedef std::vector<ArenaTeam*> RankingContainer;

enum enus
{
    ARENA_MAX_RESULTS = 10,
    ARENA_2V2_LADDER = GOSSIP_ACTION_INFO_DEF + 1,
    ARENA_GOODBYE = GOSSIP_ACTION_INFO_DEF + 4,
    ARENA_START_TEAM_LOOKUP = GOSSIP_ACTION_INFO_DEF + 5,
	ARENA_PLAYER_ONLY = GOSSIP_ACTION_INFO_DEF + 7,
    ARENA_HELP = GOSSIP_ACTION_INFO_DEF + 9999,
};

uint32 GetGuidByDb(const char* name)
{
	QueryResult check = CharacterDatabase.Query("SELECT `guid` FROM `characters` WHERE `name` = '%s'", name);

	if(!check)
		return NULL;
	
	Field* fields = check->Fetch();
		return fields[0].Get<uint64>();
}

uint32 GetTeamByDb(uint64 GUID, uint32 type)
{
	QueryResult result = CharacterDatabase.Query("SELECT `arenaTeamId` FROM `arena_team_member` WHERE `guid` = %u", GUID);

	if(!result)
		return NULL;

	std::vector<ArenaTeam*> teamIds;

	do
	{
		Field* fields = result->Fetch();
		teamIds.push_back(sArenaTeamMgr->GetArenaTeamById(fields[0].Get<uint32>()));
	}while(result->NextRow());

	for(RankingContainer::iterator itr = teamIds.begin(); itr != teamIds.end(); ++itr)
	{
		if((*itr)->GetType() == type)
			return (*itr)->GetId();
	}

	return NULL;
}

uint32 GetArenaTeamId(uint64 Guid, uint32 type)
{
    ObjectGuid playerGuid;
    playerGuid.Set(Guid);

	Player* player = ObjectAccessor::FindPlayer(playerGuid);
	
	if(!player || !player->GetSession())
		return GetTeamByDb(Guid, type);
	
	return player->GetArenaTeamId(ArenaTeam::GetSlotByType(type));
}

std::string GetTypeAsString(uint32 type)
{
	if(type == 2)
		return "2v2";
	else if(type == 3)
		return "3v3";
	return "5v5";
}

void toggleStatus(Player* player, bool& currentStatus, const std::string& statusMessage)
{
    if (currentStatus)
    {
        // El estado cambió a desactivado
        currentStatus = false;
        // Aquí puedes realizar cualquier lógica adicional necesaria cuando el estado cambia a desactivado
    }
    else
    {
        // El estado cambió a activado
        currentStatus = true;
        // Aquí puedes realizar cualquier lógica adicional necesaria cuando el estado cambia a activado
    }
}

std::string getPlayerStatus(const ObjectGuid& playerGuid)
{
    Player* player = ObjectAccessor::FindPlayer(playerGuid);
    if (!player)
        return "Offline";
    bool afkStatus = player->isAFK();
    bool dndStatus = player->isDND();

    if (afkStatus || dndStatus)
    {
        std::string status = "Online, ";
        if (afkStatus)
        {
            toggleStatus(player, afkStatus, "<AFK>");
            status += "AFK";
        }
        if (dndStatus)
        {
            toggleStatus(player, dndStatus, "<Ocupado>");
            status += "Ocupado";
        }
        return status;
    }

    return "Online";
}






std::string getWinPercent(uint32 wins, uint32 losses)
{
    uint32 totalGames = wins + losses;
    if (totalGames == 0)
        return "0%";
            
    std::stringstream buf;
    uint32 percentage = (wins * 100) / totalGames;
    buf << percentage << "%";
    return buf.str();
}

std::string raceToString(uint8 race) 
{
    std::string race_s = "Desconocido";
    switch (race)
    {
        case RACE_HUMAN:            
            race_s = "Humano";       
        break;

        case RACE_ORC:              
            race_s = "Orco";         
        break;

        case RACE_DWARF:           
            race_s = "Enano";       
        break;
 
        case RACE_NIGHTELF:         
            race_s = "Elfo de la Noche";   
        break;

        case RACE_UNDEAD_PLAYER:   
            race_s = "No-Muerto";      
        break;
                
        case RACE_TAUREN:          
            race_s = "Tauren";      
        break;

        case RACE_GNOME:            
            race_s = "Gnomo";       
        break;

        case RACE_TROLL:            
            race_s = "Troll";       
        break;

        case RACE_BLOODELF:         
            race_s = "Elfo de Sangre";   
        break;

        case RACE_DRAENEI:          
            race_s = "Draenei";     
        break;
    }
    return race_s;
}
   
std::string classToString(uint8 Class) 
{
	std::string Class_s = "Desconocido";
    switch (Class)
    {
        case CLASS_WARRIOR:         
            Class_s = "|cff4e2a04Guerrero|r";        
        break;
               
		case CLASS_PALADIN:        
            Class_s = "|cfff58cbaPaladin|r";       
        break;
               
        case CLASS_HUNTER:         
            Class_s = "|cffabd473Cazador|r";        
        break;
               
        case CLASS_ROGUE:          
            Class_s = "|cfffff569Picaro|r";          
        break;
               
        case CLASS_PRIEST:
            Class_s = "|cffffffffSacerdote|r";
        break;
               
        case CLASS_DEATH_KNIGHT:
            Class_s = "|cffc41f3bCaballero de la Muerte|r";
        break;
               
        case CLASS_SHAMAN: 
           Class_s = "|cff0070deChaman|r";       
        break;
               
        case CLASS_MAGE: 
            Class_s = "|cff69ccf0Mago|r";          
        break;
               
        case CLASS_WARLOCK: 
            Class_s = "|cff9482c9Brujo|r";       
        break;
               
        case CLASS_DRUID:          
            Class_s = "|cffff7d0aDruida|r";          
        break;
    }
    return Class_s;
}

void SendTeamInfo(Player* player, Creature* creature, std::string code)
{
	ArenaTeam* team = sArenaTeamMgr->GetArenaTeamByName(code);
	if(!team)
	{
		player->GetSession()->SendNotification("Ese equipo de arenas no existe.");
		player->PlayerTalkClass->SendCloseGossip();
		return;
	}
	
	uint32 teamId = team->GetId();
	
	QueryResult result = CharacterDatabase.Query("SELECT name, rating, seasonWins, seasonGames - seasonWins, weekWins, weekGames - weekWins, rank, captainGuid, type FROM arena_team WHERE arenaTeamId = '%u'", teamId);

    if(!result) 
    {
        player->GetSession()->SendNotification("Equipo de arenas no encontrado");
        player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    Field *fields = result->Fetch();
    std::string name = fields[0].Get<std::string>();
    uint32 rating = fields[1].Get<uint32>();
    uint32 seasonWins = fields[2].Get<uint32>();
    uint32 seasonLosses = fields[3].Get<uint32>();
	uint32 weekWins = fields[4].Get<uint32>();
    uint32 weekLosses = fields[5].Get<uint32>();
    uint32 rank = fields[6].Get<uint32>();
    uint32 captainGuid = fields[7].Get<uint32>();
    uint32 type = fields[8].Get<uint32>();

    std::string seasonWinPercentage = getWinPercent(seasonWins, seasonLosses);
    std::string weekWinPercentage = getWinPercent(weekWins, weekLosses);

    std::stringstream buf;
    buf << "Nombre del equipo: " << "|cffff8000" << name << "|r";
    AddGossipItemFor(player,7, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
    buf.str("");

    buf << "Índice: " << "|cffe30000" << rating << "|r" << " (Rango: " << "|cffe30000" << rank << "|r" << ", Tipo: " << "|cffe30000" << type << "c" << type << "|r"")";
    AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
    buf.str("");

    /*buf << "Total de la Semana: " << "|cffe30000" << weekWins << "-" << weekLosses << "|r"" (" << "|cffe30000" << weekWinPercentage << "|r" << "victorias)"; 
    AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
    buf.str("");*/

    buf << "Total de la Temporada: " << "|cffe30000" << seasonWins << "-" << seasonLosses << "|r" << " (" << "|cffe30000" << seasonWinPercentage << "|r" << "victorias)"; 
    AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);

    QueryResult members = CharacterDatabase.Query("SELECT  a.guid, a.personalRating, a.weekWins, a.weekGames - a.weekWins, a.seasonWins, a.seasonGames - a.seasonWins, c.name, c.race, c.class, c.level FROM arena_team_member a LEFT JOIN characters c ON c.guid = a.guid WHERE arenaTeamId = '%u' ORDER BY a.guid = '%u' DESC, a.seasonGames DESC, c.name ASC", teamId, captainGuid);
    if(!members) 
    {
        AddGossipItemFor(player,7, "No se encuentran miembros en el equipo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
    } 
    else 
    {
        uint32 memberPos = 1;
        uint32 memberCount = members->GetRowCount();
        uint32 guid, personalRating, level;
        std::string name, race, Class;

        buf.str("");
        buf << "----- " << memberCount << ((memberCount == 1) ? " miembro" : " miembros") << ((memberCount == 1) ? " encontrado" : " encontrados") << " -----";
        AddGossipItemFor(player,0, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
        do 
        {
            fields = members->Fetch();
            guid = fields[0].Get<uint32>();
            personalRating = fields[1].Get<uint32>();
            weekWins= fields[2].Get<uint32>();
            weekLosses = fields[3].Get<uint32>();
            seasonWins = fields[4].Get<uint32>();
            seasonLosses = fields[5].Get<uint32>();
            name = fields[6].Get<std::string>();
            race = raceToString(fields[7].Get<uint8>());
            Class = classToString(fields[8].Get<uint8>());
            level = fields[9].Get<uint32>();

            seasonWinPercentage = getWinPercent(seasonWins, seasonLosses);
            weekWinPercentage = getWinPercent(weekWins, weekLosses);

            buf.str(""); 
            buf << memberPos << ". "; 
            if (guid == captainGuid) 
                buf <<  "Capitan del Equipo: ";

            buf << name << ", " /*<< getPlayerStatus(guid)*/;
            AddGossipItemFor(player,7, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            buf.str("");

            buf << race << " " << Class << ", " << "|cffe30000" << personalRating << "|r" << " de índice personal";
            AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            buf.str("");

            /*buf << "Semana: " << "|cffe30000" << weekWins << "-" << weekLosses << "|r" << " (" << "|cffe30000" << weekWinPercentage << "|r" << "victorias), " << "|cffe30000" << (weekWins + weekLosses) << "|r" << " jugadas"; 
            AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            buf.str("");*/

            buf << "Temporada: " << "|cffe30000" << seasonWins << "-" << seasonLosses << "|r" << " (" << "|cffe30000" << seasonWinPercentage << "|r" << "victorias), " << "|cffe30000" << (seasonWins + seasonLosses) << "|r" << " jugadas"; 
            AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            memberPos++;
		}while(members->NextRow());
    }
    SendGossipMenuFor(player,1, creature->GetGUID());

	/*std::ostringstream info;
	
	info << "------------ " << "Tipo: " << "|cffe30000" << GetTypeAsString(team->GetType()) << "|R" << " ------------";
	AddGossipItemFor(player,0, info.str().c_str(), GOSSIP_SENDER_MAIN, 0);
	info.str("");
	info.clear();

	uint32 rank = team->GetStats().Rank;
	uint32 rating = team->GetStats().Rating;

	uint32 weekGames = team->GetStats().WeekGames;
	uint32 weekWins = team->GetStats().WeekWins;
	uint32 weekLosses = weekGames - weekWins;

	uint32 seasonGames = team->GetStats().SeasonGames;
	uint32 seasonWins = team->GetStats().SeasonWins;
	uint32 seasonLosses = seasonGames - seasonWins;

	Player* captain = ObjectAccessor::FindPlayer(team->GetCaptain());
	std::string captainName;
	bool captainValid;

	if(!captain || !captain->GetSession())
	{
		QueryResult result = CharacterDatabase.Query("SELECT `name` FROM `characters` WHERE `guid` = %u", team->GetCaptain());
		if(result)
		{
			Field* fields = result->Fetch();
			captainName = fields[0].Get<std::string>();
			captainValid = true;
		}
	}
	else
	{
		captainName = captain->GetName();
		captainValid = true;
	}


	info << "\nCapitán del Equipo: " << captainName << ", " << getPlayerStatus(team->GetCaptain());
    AddGossipItemFor(player,7, info.str(), GOSSIP_SENDER_MAIN, parentOption);
    info.str("");

	info << "\nÍndice: " << "|cffe30000" << (uint32)rating << "|R" << "(Rango: "  << "|cffe30000" << (uint16)rank << "|R)";
	<< "\nMembers : ";

	for(ArenaTeam::MemberList::iterator itr = team->m_membersBegin(); itr != team->m_membersEnd(); ++itr)
	{
	info << itr->Name;
	if(std::distance(team->m_membersBegin(), itr) < team->GetType() -1 )
	info << ", ";
	}
	info << "\nWeek Games : "
	<< (uint32)weekGames << ", Wins : " << (uint32)weekWins << ", Losses : " << (uint32)weekLosses
	<< "\nSeason Games : "
	<< (uint32)seasonGames << ", Wins : " << (uint32)seasonWins << ", Losses : " << (uint32)seasonLosses;

	AddGossipItemFor(player,0, info.str().c_str(), GOSSIP_SENDER_MAIN, 0);

	SendGossipMenuFor(player,1, creature->GetGUID());*/
}

void SendSoloInfo(Player* player, Creature* creature, uint32 teamId, uint64 target, uint32 type)
{
    std::ostringstream info;
    info << "------------ " << "Tipo: " << "|cffe30000" << GetTypeAsString(type) << "|R" << " ------------";
    AddGossipItemFor(player, 0, info.str().c_str(), GOSSIP_SENDER_MAIN, 0);
    info.str("");
    info.clear();
    ArenaTeam* team = sArenaTeamMgr->GetArenaTeamById(teamId);
    info << "Estadísticas: \n";
    for (ArenaTeam::MemberList::iterator itr = team->m_membersBegin(); itr != team->m_membersEnd(); ++itr)
    {
        if (target == itr->Guid.GetRawValue())
        {
            std::ostringstream ss;

            std::string name = itr->Name;
            std::string teamName = team->GetName();
            uint32 MMR = itr->MatchMakerRating;
            uint32 personalRating = itr->PersonalRating;

            uint32 seasonGames = itr->SeasonGames;
            uint32 seasonWins = itr->SeasonWins;
            uint32 seasonLosses = seasonGames - seasonWins;

            uint32 weekGames = itr->WeekGames;
            uint32 weekWins = itr->WeekWins;
            uint32 weekLosses = weekGames - weekWins;

            ss.str("");
            ss << name << ", " << getPlayerStatus(itr->Guid);
            AddGossipItemFor(player, 7, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            ss.str("");

            ss << "MMR Personal: " << "|cffe30000" << MMR << "|R";
            AddGossipItemFor(player, 4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            ss.str("");

            ss << "Nombre del Equipo: " << "|cffe30000" << teamName << "|R";
            AddGossipItemFor(player, 4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            ss.str("");

            ss << "Índice Personal: " << "|cffe30000" << personalRating << "|R";
            AddGossipItemFor(player, 4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            ss.str("");

            ss << "Semana: " << "|cffe30000" << weekWins << "|R" << "-" << "|cffe30000" << weekLosses << "|R" << "," << "|cffe30000 " << weekGames << "|R" << " jugadas en total";
            AddGossipItemFor(player, 4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            ss.str("");

            ss << "Temporada: " << "|cffe30000" << seasonWins << "|R" << "-" << "|cffe30000" << seasonLosses << "|R" << "," << "|cffe30000 " << seasonGames << "|R" << " jugadas en total";
            AddGossipItemFor(player, 4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
            ss.str("");
        }
    }

    SendGossipMenuFor(player, 907, creature->GetGUID());
}

void SendOptions(Player* player, Creature* creature, const char* code)
{
    uint64 Guid;
    Player* target = ObjectAccessor::FindPlayerByName(code);

    if (!target || !target->GetSession())
        Guid = GetGuidByDb(code);
    else
        Guid = target->GetGUID().GetRawValue();

    if (!Guid)
    {
        player->GetSession()->SendNotification("El jugador no existe.");
        player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    if (!GetArenaTeamId(Guid, 2) && !GetArenaTeamId(Guid, 3) && !GetArenaTeamId(Guid, 5))
    {
        player->GetSession()->SendNotification("El jugador no está en un Equipo de Arenas.");
        player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    for (int i = 2; i < 6; ++i)
    {
        if (i == 4)
            continue;

        uint16 action = (i != 5 ? i * 2 - (i == 3 ? 1 : 0) : i + 1);

        if (GetArenaTeamId(Guid, i))
            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, GetTypeAsString(i), Guid, action);
    }
    SendGossipMenuFor(player, 1, creature->GetGUID());
}



class arena_top_teams : public CreatureScript
{
private:
    uint32 optionToTeamType(uint32 option)
    {
        uint32 teamType;
        switch (option)
        {
        case ARENA_2V2_LADDER:
            teamType = 2;
            break;
        default:
            // Asignar un valor predeterminado si no se encuentra un caso coincidente
            teamType = 0; // O cualquier otro valor predeterminado que desees
            break;
        }
        return teamType;
    }
        
    uint32 teamTypeToOption(uint32 teamType)
    {
        uint32 option;
        switch (teamType)
        {
        case 2:
            option = ARENA_2V2_LADDER;
            break;
        default:
            // Asignar un valor predeterminado si no se encuentra un caso coincidente
            option = 0; // O cualquier otro valor predeterminado que desees
            break;
        }
        return option;
    }
        
    std::string raceToString(uint8 race) 
    {
        std::string race_s = "Desconocido";
        switch (race)
        {
            case RACE_HUMAN:            
                race_s = "Humano";       
            break;
                
            case RACE_ORC:              
                race_s = "Orco";         
            break;
                
            case RACE_DWARF:           
                race_s = "Enano";       
            break;
                
            case RACE_NIGHTELF:         
                race_s = "Elfo de la Noche";   
            break;
                
            case RACE_UNDEAD_PLAYER:   
                race_s = "No-Muerto";      
            break;
                
            case RACE_TAUREN:          
                race_s = "Tauren";      
            break;
                
            case RACE_GNOME:            
                race_s = "Gnomo";       
            break;
                
            case RACE_TROLL:            
                race_s = "Troll";       
            break;
                
            case RACE_BLOODELF:         
                race_s = "Elfo de Sangre";   
            break;
                
            case RACE_DRAENEI:          
                race_s = "Draenei";     
            break;
        }
        return race_s;
    }
        
    std::string classToString(uint8 Class) 
    {
        std::string Class_s = "Desconocido";
        switch (Class)
        {
            case CLASS_WARRIOR:         
                Class_s = "|cff4e2a04Guerrero|r";        
            break;
                
            case CLASS_PALADIN:        
                Class_s = "|cfff58cbaPaladin|r";       
            break;
                
            case CLASS_HUNTER:         
                Class_s = "|cffabd473Cazador|r";        
            break;
                
            case CLASS_ROGUE:          
                Class_s = "|cfffff569Picaro|r";          
            break;
                
            case CLASS_PRIEST:
                Class_s = "|cffffffffSacerdote|r";
            break;
                
            case CLASS_DEATH_KNIGHT:
                Class_s = "|cffc41f3bCaballero de la Muerte|r";
            break;
                
            case CLASS_SHAMAN: 
                Class_s = "|cff0070deChaman|r";       
            break;
                
            case CLASS_MAGE: 
                Class_s = "|cff69ccf0Mago|r";          
            break;
                
            case CLASS_WARLOCK: 
                Class_s = "|cff9482c9Brujo|r";       
            break;
                
            case CLASS_DRUID:          
                Class_s = "|cffff7d0aDruida|r";          
            break;
        }
        return Class_s;
    }


    std::string getPlayerStatus(const ObjectGuid& playerGuid)
    {
        Player* player = ObjectAccessor::FindPlayer(playerGuid);
        if (!player)
            return "Offline";
        bool afkStatus = player->isAFK();
        bool dndStatus = player->isDND();

        if (afkStatus || dndStatus)
        {
            std::string status = "Online, ";
            if (afkStatus)
            {
                toggleStatus(player, afkStatus, "<AFK>");
                status += "AFK";
            }
            if (dndStatus)
            {
                toggleStatus(player, dndStatus, "<Ocupado>");
                status += "Ocupado";
            }
            return status;
        }

        return "Online";
    }
        
    std::string getWinPercent(uint32 wins, uint32 losses)
    {
        uint32 totalGames = wins + losses;
        if (totalGames == 0)
            return "0%";
            
        std::stringstream buf;
        uint32 percentage = (wins * 100) / totalGames;
        buf << percentage << "%";
        return buf.str();
    }

    public:
        arena_top_teams() : CreatureScript("arena_top_teams"){}
        
    bool OnGossipHello(Player *player, Creature *creature)
    {
		AddGossipItemFor(player,4,"|cff00ff00|TInterface\\icons\\Achievement_Arena_2v2_7:26:26:-15:0|t|rTop 10: Arenas 2c2", GOSSIP_SENDER_MAIN, ARENA_2V2_LADDER);
		//player->ADD_GOSSIP_ITEM_EXTENDED(4, "|cff00ff00|TInterface\\icons\\Achievement_Arena_2v2_7:26:26:-15:0|t|rEstadísticas de un Jugador", GOSSIP_SENDER_MAIN, 20, "Inserta el nombre del jugador y pulsa el botón ""Aceptar""", 0, true);
		//player->ADD_GOSSIP_ITEM_EXTENDED(0, "|cff00ff00|TInterface\\icons\\Achievement_Arena_2v2_7:26:26:-15:0|t|rEstadísticas de un Equipo de Arenas", GOSSIP_SENDER_MAIN, 3, "Inserta el nombre del equipo y pulsa el botón ""Aceptar""", 0, true);
		AddGossipItemFor(player,4,"|cff00ff00|TInterface\\icons\\spell_chargenegative:26:26:-15:0|t|rOlvídalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
        SendGossipMenuFor(player,1, creature->GetGUID());
        return true;
    }
	
	bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code)
	{		
		player->PlayerTalkClass->ClearMenus();
		switch(action)
		{
			case 20:
			{
				SendOptions(player, creature, code);
				return true;
			} break;

			case 3:
			{
				SendTeamInfo(player, creature, code);
				return true;
			} break;
		}
		return true;
	}
        
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 uiAction) 
    {
        player->PlayerTalkClass->ClearMenus();
        switch(uiAction) 
        {
			case 4: 
				SendSoloInfo(player, creature, GetArenaTeamId(sender, 2), sender, 2);
			break;

			case 5:
				SendSoloInfo(player, creature, GetArenaTeamId(sender, 3), sender, 3);
			break;

			case 6:
				SendSoloInfo(player, creature, GetArenaTeamId(sender, 5), sender, 5);
			break;

            case ARENA_GOODBYE:
            {
                player->PlayerTalkClass->SendCloseGossip();
                break;
            }

            case ARENA_HELP:
            {
                ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Informacion]:|r Esta criatura muestra el Top 10 de equipos de arenas. Solamente necesitas hacer click en un nombre de un equipo para ver sus detalles.");
                player->PlayerTalkClass->SendCloseGossip();
				break;
            }

            case ARENA_2V2_LADDER:
            {
                uint32 teamType = optionToTeamType(uiAction);
                QueryResult result = CharacterDatabase.Query("SELECT arenaTeamId, name, rating FROM arena_team WHERE type = '%u' ORDER BY rating DESC LIMIT %u;", teamType, ARENA_MAX_RESULTS);
                    
                if(!result) 
                {
                    AddGossipItemFor(player, 7, "Olvidalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
                    SendGossipMenuFor(player, 1, creature->GetGUID());
                } 
                else
                {
                    std::string name;
                    uint32 teamId, rating, rank = 1;
                    AddGossipItemFor(player,0,"Lista del Top de Arenas:", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
                    do 
                    {
                        Field *fields = result->Fetch();
                        teamId = fields[0].Get<uint32>();
                        name = fields[1].Get<std::string>();
                        rating = fields[2].Get<uint32>();
                            
                        std::stringstream buffer;
                        buffer << rank << ". " << name;
                        buffer << ": " << "|cffe30000" << rating << "|r" << " de índice";
                        AddGossipItemFor(player,4, buffer.str(), GOSSIP_SENDER_MAIN, ARENA_START_TEAM_LOOKUP + teamId);
                            
                        rank++;
                    } 
                    while(result->NextRow());
                    AddGossipItemFor(player,7, "Olvidalo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
                    SendGossipMenuFor(player,1, creature->GetGUID());
                }
                break;
            }
            
            default:
            {
                if (uiAction > ARENA_START_TEAM_LOOKUP) 
                {
                    uint32 teamId = uiAction - ARENA_START_TEAM_LOOKUP;
                    QueryResult result = CharacterDatabase.Query("SELECT name, rating, seasonWins, seasonGames - seasonWins, weekWins, weekGames - weekWins, rank, captainGuid, type FROM arena_team WHERE arenaTeamId = '%u'", teamId);
                        
                    if(!result) 
                    {
                        player->GetSession()->SendNotification("Equipo de arenas no encontrado");
                        player->PlayerTalkClass->SendCloseGossip();
                        return true;
                    }
                        
                    Field *fields = result->Fetch();
                    std::string name = fields[0].Get<std::string>();
                    uint32 rating = fields[1].Get<uint32>();
                    uint32 seasonWins = fields[2].Get<uint32>();
                    uint32 seasonLosses = fields[3].Get<uint32>();
                    uint32 weekWins = fields[4].Get<uint32>();
                    uint32 weekLosses = fields[5].Get<uint32>();
                    uint32 rank = fields[6].Get<uint32>();
                    uint32 captainGuid = fields[7].Get<uint32>();
                    uint32 type = fields[8].Get<uint32>();
                    uint32 parentOption = teamTypeToOption(type);
                        
                    std::string seasonWinPercentage = getWinPercent(seasonWins, seasonLosses);
                    std::string weekWinPercentage = getWinPercent(weekWins, weekLosses);
                        
                    std::stringstream buf;
                    buf << "Nombre del equipo: " << "|cffff8000" << name << "|r";
                    AddGossipItemFor(player,7, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                    buf.str("");
                    
                    buf << "Índice: " << "|cffe30000" << rating << "|r" << " (Rango: " << "|cffe30000" << rank << "|r" << ", Tipo: " << "|cffe30000" << type << "c" << type << "|r"")";
                    AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                    buf.str("");
                    
                    /*buf << "Total de la Semana: " << "|cffe30000" << weekWins << "-" << weekLosses << "|r"" (" << "|cffe30000" << weekWinPercentage << "|r" << "victorias)"; 
                    AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                    buf.str("");*/
                    
                    buf << "Total de la Temporada: " << "|cffe30000" << seasonWins << "-" << seasonLosses << "|r" << " (" << "|cffe30000" << seasonWinPercentage << "|r" << "victorias)"; 
                    AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                        
                    QueryResult members = CharacterDatabase.Query("SELECT  a.guid, a.personalRating, a.weekWins, a.weekGames - a.weekWins, a.seasonWins, a.seasonGames - a.seasonWins, c.name, c.race, c.class, c.level FROM arena_team_member a LEFT JOIN characters c ON c.guid = a.guid WHERE arenaTeamId = '%u' ORDER BY a.guid = '%u' DESC, a.seasonGames DESC, c.name ASC", teamId, captainGuid);
                    if(!members) 
                    {
                        AddGossipItemFor(player,7, "No se encuentran miembros en el equipo", GOSSIP_SENDER_MAIN, parentOption);
                    } 
                    else 
                    {
                        uint32 memberPos = 1;
                        uint32 memberCount = members->GetRowCount();
                        uint32 guid, personalRating, level;
                        std::string name, race, Class;
                            
                        buf.str("");
                        buf << "----- " << memberCount << ((memberCount == 1) ? " miembro" : " miembros") << ((memberCount == 1) ? " encontrado" : " encontrados") << " -----";
                        AddGossipItemFor(player,0, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                        do 
                        {
                            fields = members->Fetch();
                            guid = fields[0].Get<uint32>();
                            personalRating = fields[1].Get<uint32>();
                            weekWins= fields[2].Get<uint32>();
                            weekLosses = fields[3].Get<uint32>();
                            seasonWins = fields[4].Get<uint32>();
                            seasonLosses = fields[5].Get<uint32>();
                            name = fields[6].Get<std::string>();
                            race = raceToString(fields[7].Get<uint8>());
                            Class = classToString(fields[8].Get<uint8>());
                            level = fields[9].Get<uint32>();
                                
                            seasonWinPercentage = getWinPercent(seasonWins, seasonLosses);
                            weekWinPercentage = getWinPercent(weekWins, weekLosses);
                                
                            buf.str(""); 
                            buf << memberPos << ". "; 
                            if (guid == captainGuid) 
                                buf <<  "Capitan del Equipo: ";
                                
                            buf << name << ", " /*<< getPlayerStatus(guid)*/;
                            AddGossipItemFor(player,7, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                            buf.str("");
                            
                            buf << race << " " << Class << ", " << "|cffe30000" << personalRating << "|r" << " de índice personal";
                            AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                            buf.str("");
                            
                            /*buf << "Semana: " << "|cffe30000" << weekWins << "-" << weekLosses << "|r" << " (" << "|cffe30000" << weekWinPercentage << "|r" << "victorias), " << "|cffe30000" << (weekWins + weekLosses) << "|r" << " jugadas"; 
                            AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                            buf.str("");*/
                            
                            buf << "Temporada: " << "|cffe30000" << seasonWins << "-" << seasonLosses << "|r" << " (" << "|cffe30000" << seasonWinPercentage << "|r" << "victorias), " << "|cffe30000" << (seasonWins + seasonLosses) << "|r" << " jugadas"; 
                            AddGossipItemFor(player,4, buf.str(), GOSSIP_SENDER_MAIN, parentOption);
                            memberPos++;
                        } 
                        while(members->NextRow());
                    }
                    SendGossipMenuFor(player, 1, creature->GetGUID());
                }
            }
        }
        return true;
    }
};

void AddSC_arena_top_teams()
{
    new arena_top_teams();
}
