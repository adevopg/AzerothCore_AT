#include "ScriptPCH.h"
using namespace std;
 
class npc_arena_teamTop : public CreatureScript
{
   public:
           npc_arena_teamTop() : CreatureScript("npc_arena_teamTop") { }
 
           bool OnGossipHello(Player * player, Creature * creature)
           {
                  AddGossipItemFor(player,9, "|cff00ff00|TInterface\\icons\\Achievement_arena_2v2_7:26|t|r Ver el Top de Arenas de 2vs2", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                  AddGossipItemFor(player,GOSSIP_ICON_CHAT, "Olvidalo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                  SendGossipMenuFor(player, 1, creature->GetGUID());
                   return true;
           }
 
           bool OnGossipSelect(Player * player, Creature * creature, uint32 sender, uint32 actions)
           {
                   if(sender == GOSSIP_SENDER_MAIN)
                   {
                           switch(actions)
                           {
                               case GOSSIP_ACTION_INFO_DEF+1:
                                   {
                                           QueryResult result = CharacterDatabase.Query("SELECT name,rating FROM arena_team WHERE type='2' ORDER BY rating DESC LIMIT 10");
                                           if(!result)
                                                   return false;
 
                                           Field * fields = NULL;
                                           player->Whisper("|cff4169E1Aqui esta el Top 10 de equipos de 2vs2:|r", LANG_UNIVERSAL, player);
                                           do
                                           {
                                                   fields = result->Fetch();
                                                   string arena_name = fields[0].Get<std::string>();
                                                   uint32 rating = fields[1].Get<uint32>();
                                                   char msg[250];
                                                   snprintf(msg, 250, "Equipo: |cffFFFF00%s|r, Rating: |cffFFFF00%u \n", arena_name.c_str(), rating);
                                                   player->Whisper(msg, LANG_UNIVERSAL, player);
                                           }while(result->NextRow());
                                   }break;
 
                               case GOSSIP_ACTION_INFO_DEF+4:
                                           {
                                                   player->PlayerTalkClass->SendCloseGossip();
                                           }break;
                           }
                   }
 
                   return true;
           }
};
 
void AddSC_npc_arena_teamTop()
{
        new npc_arena_teamTop;
}
