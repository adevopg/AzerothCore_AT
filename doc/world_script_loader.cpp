/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// This is where scripts' loading functions should be declared:
void AddSC_areatrigger_scripts();
void AddSC_emerald_dragons();
void AddSC_generic_creature();
void AddSC_go_scripts();
void AddSC_guards();
void AddSC_item_scripts();
void AddSC_npc_professions();
void AddSC_npc_innkeeper();
void AddSC_npcs_special();
void AddSC_npc_taxi();
void AddSC_achievement_scripts();
void AddSC_chat_log(); // location: scripts\World\chat_log.cpp
void AddSC_action_ip_logger(); // location: scripts\World\action_ip_logger.cpp
void AddSC_player_scripts();
void AddSC_npc_stave_of_ancients();
void AddSC_server_mail();
//Add Custom Scripts
void AddSC_Anuncio_boss();
void AddSC_gm_login();
void AddSC_arena_top_teams();
void AddSC_fast_arena_start();
void AddSC_boss_malfurion();
void AddSC_boss_sylvanas();
void AddSC_channel_factions();
void AddSC_cofre_gurubashi();
void AddSC_comandos_jugadores();
void AddSC_fake_commandscript();
void AddSC_defensor_shop();
void AddSC_duelreset();
void AddSC_gameobjects_shop();
void AddSC_GuildHouse();
void AddSC_GMItems();
void AddSC_isla_pvp();
void AddSC_libro_comandos();
void AddSC_Npc_ah();
void AddSC_npc_1v1arena();
void AddSC_Npc_Beastmaster();
void AddSC_npc_buff();
void AddSC_npc_blood_money();
void AddSC_npc_enchant();
void AddSC_npc_enchantment();
void AddSC_Npc_skill();
void AddSC_Npc_vendor();
void AddSC_npc_waypoint();
void AddSC_player_abuse();
void AddSC_probador_monturas_custom();
void AddSC_PvPTitles();
void AddSC_Racha_de_Muertes();
void AddSC_skill_npc();
void AddSC_teleport_array();
void AddSC_npc_arena_teamTop();
void AddSC_NPC_PvPIsland_Vendor();
void AddSC_vipcommands();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
//Add Custom Scripts
void AddCustomScripts()
{
    AddSC_Anuncio_boss();
    AddSC_gm_login();
    AddSC_arena_top_teams();
    AddSC_fast_arena_start();
    AddSC_boss_malfurion();
    AddSC_boss_sylvanas();
    AddSC_channel_factions();
    AddSC_cofre_gurubashi();
    AddSC_comandos_jugadores();
    AddSC_fake_commandscript();
    AddSC_defensor_shop();
    AddSC_gameobjects_shop();
    AddSC_GuildHouse();
    AddSC_GMItems();
    AddSC_isla_pvp();
    AddSC_libro_comandos();
    AddSC_Npc_ah();
    AddSC_npc_1v1arena();
    AddSC_Npc_Beastmaster();
    AddSC_npc_buff();
    AddSC_npc_blood_money();
    AddSC_npc_enchant();
    AddSC_npc_enchantment();
    AddSC_Npc_skill();
    AddSC_Npc_vendor();
    AddSC_npc_waypoint();
    AddSC_player_abuse();
    AddSC_probador_monturas_custom();
    AddSC_PvPTitles();
    AddSC_Racha_de_Muertes();
    AddSC_skill_npc();
    AddSC_teleport_array();
    AddSC_npc_arena_teamTop();
    AddSC_NPC_PvPIsland_Vendor();
    AddSC_vipcommands();
}
void AddWorldScripts()
{
    AddSC_areatrigger_scripts();
    AddSC_emerald_dragons();
    AddSC_generic_creature();
    AddSC_go_scripts();
    AddSC_guards();
    AddSC_item_scripts();
    AddSC_npc_professions();
    AddSC_npc_innkeeper();
    AddSC_npcs_special();
    AddSC_npc_taxi();
    AddSC_achievement_scripts();
    AddSC_chat_log(); // location: scripts\World\chat_log.cpp
    AddSC_action_ip_logger(); // location: scripts\World\action_ip_logger.cpp
    AddSC_player_scripts();
    AddSC_npc_stave_of_ancients();
    AddSC_server_mail();
}
