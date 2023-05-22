#include "ScriptPCH.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include <sstream>

class FastArenaCrystal : public GameObjectScript
{
    public:

        FastArenaCrystal()
            : GameObjectScript("FastArenaCrystal")
        {
        }

        bool OnGossipHello(Player* player, GameObject* go)
        {
     		 if (player->IsSpectator())
      		{
                 ChatHandler(player->GetSession()).PSendSysMessage("No puedes hacer eso siendo un espectador.");
                return false;
    	       }

            if (Battleground *pBG = player->GetBattleground())
                if (pBG->isArena())
                    ChatHandler(player->GetSession()).PSendSysMessage("Jugadores que han clickeado: %u", pBG->ClickFastStart(player, go));

            return false;
        }
};

void AddSC_fast_arena_start()
{
    new FastArenaCrystal();
}
