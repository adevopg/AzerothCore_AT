#include "ScriptPCH.h"

struct PlayerAbuse
{
    uint64 victimGUID;
    uint32 whenKilled;
};

std::map<uint64, PlayerAbuse> abuseList;

class kill_player_abuse : PlayerScript
{
public:
    kill_player_abuse() : PlayerScript("kill_player_abuse") { }

    void OnPVPKill(Player* killer, Player* victim)
    {
        if (killer->GetGUID() == victim->GetGUID())
            return;
			
		if (killer->GetAreaId() == 368 || killer->InBattleground() || killer->InArena() || killer->GetAreaId() == 2177)
			return;

        if (!abuseList.empty())
        {
            for (std::map<uint64, PlayerAbuse>::const_iterator itr = abuseList.begin(); itr != abuseList.end(); ++itr)
            {
                if (itr->first == killer->GetGUID().GetRawValue() && itr->second.victimGUID == victim->GetGUID().GetRawValue())
                {
                    if (GetMSTimeDiffToNow(itr->second.whenKilled) < 120000) // 2 min
                    {   // El jugador no podra matar al mismo jugador durante 2 minutos
                        ChatHandler(killer->GetSession()).PSendSysMessage("No puedes matar a ese jugador hasta dentro de %u minuto(s).", CalculateTimeInMinutes(GetMSTimeDiffToNow(itr->second.whenKilled)));
                        return;
                    }
                    else
                        abuseList.erase(killer->GetGUID().GetRawValue());
                }
            }
        }
        abuseList[killer->GetGUID().GetRawValue()].victimGUID = victim->GetGUID().GetRawValue();
        abuseList[killer->GetGUID().GetRawValue()].whenKilled = getMSTime();
    }

    uint32 CalculateTimeInMinutes(uint32 m_time)
    {
        uint32 howManyMinutes;
        if (m_time >= 120000) // 120000 = 2 min
            howManyMinutes = 2;
        else if (m_time < 60000) //60000 = 1 min
            howManyMinutes = 1;
        return howManyMinutes;
    }
};

void AddSC_player_abuse()
{
    new kill_player_abuse();
}
