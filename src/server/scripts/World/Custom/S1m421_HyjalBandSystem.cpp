#include "ScriptPCH.h"

/*Group HyjalA = new Group;
Group HyjalH = new Group;*/

class Hyjal_band : public PlayerScript
{
public:
   Hyjal_band() : PlayerScript("Hyjal_band") {}
 
	/*void OnUpdateZone(Player* player)
	{
		if(Player->isInZone(616)&& getfaction== alliance)
		{
			uint32 playerGuid = player->GetGUID();
			if(player->GetTeamId()==TEAM_ALLIANCE)
			{
				if(HyjalA->IsMember(playerGuid))
				{
					uint8 subgroup = HyjalA->GetMemberGroup(playerGuid);
					player->SetBattlegroundRaid(HyjalA, subgroup);
				}
				else
				{
					HyjalA->Create(player);
					HyjalA->AddMember(player);
					if (Group* originalGroup = player->GetOriginalGroup())
						if (originalGroup->IsLeader(playerGuid))
						{
							originalGroup->ChangeLeader(playerGuid);
							originalGroup->SendUpdate();
						}
			}
		}
	}*/
};

void AddSC_Hyjal_band()
{
    new Hyjal_band();
}