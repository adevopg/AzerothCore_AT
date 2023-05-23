#include "ScriptPCH.h"

/*Group HyjalA = new Group;
Group HyjalH = new Group;*/

class Hyjal_band : public PlayerScript
{
public:
   Hyjal_band() : PlayerScript("Hyjal_band") {}
 
	/*void OnUpdateZone(Player* player)
	{
		if(player->IsInMap()  == 616 && player->GetTeamId() == ALLIANCE)
		{
			uint32 playerGuid = player->GetGUID().GetRawValue();
			if(player->GetTeamId()==TEAM_ALLIANCE)
			{
				if(player->IsInSameGroupWith(playerGuid))
				{
					uint8 subgroup = player->GetMemberGroup(playerGuid);
					player->SetBattlegroundRaid(HyjalA, subgroup);
				}
				else
				{
					player->Create(player);
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
