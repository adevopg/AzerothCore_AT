#include "ScriptPCH.h"

class Anuncio_boss : public PlayerScript
{
public:
Anuncio_boss() : PlayerScript ("Anuncio_boss") {} 

void OnCreatureKill(Player* player, Creature* boss)
{
    if (boss->GetEntry() == 100000 || boss->GetEntry() == 100005 || boss->GetEntry() == 4949 || boss->GetEntry() == 29611)
    {
        char msg[250];

        if (player->GetGroup())
        {
            const std::string playerName = player->GetName();
            const std::string bossName = boss->GetName();

            strcpy_s(msg, sizeof(msg), "|CFF7BBEF7[Anuncio de Boss]|r:|cffff0000 ");
            strcat_s(msg, sizeof(msg), playerName.c_str());
            strcat_s(msg, sizeof(msg), " y su grupo han matado a |CFF18BE00[");
            strcat_s(msg, sizeof(msg), bossName.c_str());
            strcat_s(msg, sizeof(msg), "]|r !!!");
        }
        else
        {
            const std::string playerName = player->GetName();
            const std::string bossName = boss->GetName();

            strcpy_s(msg, sizeof(msg), "|CFF7BBEF7[Anuncio de Boss]|r:|cffff0000 ");
            strcat_s(msg, sizeof(msg), playerName.c_str());
            strcat_s(msg, sizeof(msg), " ha matado a |CFF18BE00[");
            strcat_s(msg, sizeof(msg), bossName.c_str());
            strcat_s(msg, sizeof(msg), "]|r !!!");
        }
        sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
    }
}

};

void AddSC_Anuncio_boss()
{
	new Anuncio_boss();
}
