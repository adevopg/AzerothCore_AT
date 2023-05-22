#include "ScriptPCH.h"
#include "../../modules/mod-transmog/src/Transmogrification.h"
#define sT  sTransmogrification

class npc_enchant : public CreatureScript
{
public:
    npc_enchant() : CreatureScript("npc_enchant") { }

    bool isEquiped(Player* player, uint8 slot)
    {
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if (!item)
            return false;

        return true;
    }

    bool validItem(uint8 type, Item* item)
    {
        ItemTemplate const* pProto = item->GetTemplate();

        // Una Mano
        if (type == 1 && (pProto->Class == 2 && (pProto->SubClass == 0 || pProto->SubClass == 1 || pProto->SubClass == 4 || pProto->SubClass == 5 || pProto->SubClass == 6 || pProto->SubClass == 7 || pProto->SubClass == 8 || pProto->SubClass == 10 || pProto->SubClass == 13 || pProto->SubClass == 15)))
            return true;
        // Dos Manos
        if (type == 2 && (pProto->Class == 2 && (pProto->SubClass == 1 || pProto->SubClass == 5 || pProto->SubClass == 6 || pProto->SubClass == 8 || pProto->SubClass == 10)))
            return true;
        // Baston
        if (type == 3 && (pProto->Class == 2 && pProto->SubClass == 10))
            return true;
        // Escudo
        if (type == 4 && pProto->InventoryType == 14)
            return true;

        return false;
    }

    void Enchant(Player* player, Creature* creature, uint8 slot, uint32 enchant, uint8 type = 0)
    {
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

        if (!item || (type != 0 && !validItem(type, item)))
        {
           CloseGossipMenuFor(player);
            player->GetSession()->SendNotification("|cFF00FFFF""\xc2\xbf""Seguro que tienes ese objeto equipado?|r");
            return;
        }

        SpellItemEnchantmentEntry const* enchantid = sSpellItemEnchantmentStore.LookupEntry(enchant);
        if (!enchantid)
            return;

        player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, false);
        item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchant, 0, 0);
        player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, true);
		//player->GetSession()->SendNotification("|cffFF0000""\xc2\xa1""|cFFFFD700%s |cffFF0000encantado correctamente!", item->GetTemplate()->Name1.c_str());
        std::string color = "|cff";
        switch (item->GetTemplate()->Quality)
        {
			case 0: color += "9d9d9d"; break;
			case 1: color += "ffffff"; break;
            case 2: color += "1eff00"; break;
            case 3: color += "0070dd"; break;
            case 4: color += "a335ee"; break;
            case 5: color += "ff8000"; break;
        }
        ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000""\xc2\xa1""%s[%s] |cffFF0000encantado correctamente!", color.c_str(), item->GetTemplate()->Name1.c_str());
		
        //creature->CastSpell(player, 45209, true);
        OnGossipHello(player, creature);
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		bool count = false;

		if(isEquiped(player, EQUIPMENT_SLOT_HEAD))
		{
			Item* newItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD);
			uint32 entry = sT->GetFakeEntry(newItem->GetGUID());
			std::string icon = entry ? sT->GetItemIcon(entry, 27, 27, -14, 0) : sT->GetItemIcon(newItem->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, icon+"Encantar Casco", EQUIPMENT_SLOT_HEAD, GOSSIP_ACTION_INFO_DEF+2);
			count = true;
		}
		
		if(isEquiped(player, EQUIPMENT_SLOT_SHOULDERS))
		{
			Item* newItem2 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS);
			uint32 entry2 = sT->GetFakeEntry(newItem2->GetGUID());
			std::string icon2 = entry2 ? sT->GetItemIcon(entry2, 27, 27, -14, 0) : sT->GetItemIcon(newItem2->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon2+"Encantar Hombreras", EQUIPMENT_SLOT_SHOULDERS, GOSSIP_ACTION_INFO_DEF+3);
			count = true;
		}
		
		if(isEquiped(player, EQUIPMENT_SLOT_BACK))
		{
			Item* newItem3 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK);
			uint32 entry3 = sT->GetFakeEntry(newItem3->GetGUID());
			std::string icon3 = entry3 ? sT->GetItemIcon(entry3, 27, 27, -14, 0) : sT->GetItemIcon(newItem3->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon3+"Encantar Espalda", EQUIPMENT_SLOT_BACK, GOSSIP_ACTION_INFO_DEF+4);
			count = true;
		}
		
		if(isEquiped(player, EQUIPMENT_SLOT_CHEST))
		{
			Item* newItem4 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST);
			uint32 entry4 = sT->GetFakeEntry(newItem4->GetGUID());
			std::string icon4 = entry4 ? sT->GetItemIcon(entry4, 27, 27, -14, 0) : sT->GetItemIcon(newItem4->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon4+"Encantar Pechera", EQUIPMENT_SLOT_CHEST, GOSSIP_ACTION_INFO_DEF+5);
			count = true;
		}
		
		if(isEquiped(player, EQUIPMENT_SLOT_WRISTS))
		{
			Item* newItem5 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS);
			uint32 entry5 = sT->GetFakeEntry(newItem5->GetGUID());
			std::string icon5 = entry5 ? sT->GetItemIcon(entry5, 27, 27, -14, 0) : sT->GetItemIcon(newItem5->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon5+"Encantar Mu""\xC3\xB1""ecas", EQUIPMENT_SLOT_WRISTS, GOSSIP_ACTION_INFO_DEF+6);
			count = true;
		}
		
		if(isEquiped(player, EQUIPMENT_SLOT_HANDS))
		{
			Item* newItem6 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS);
			uint32 entry6 = sT->GetFakeEntry(newItem6->GetGUID());
			std::string icon6 = entry6 ? sT->GetItemIcon(entry6, 27, 27, -14, 0) : sT->GetItemIcon(newItem6->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon6+"Encantar Guantes", EQUIPMENT_SLOT_HANDS, GOSSIP_ACTION_INFO_DEF+7);
			count = true;
		}
		
		if(isEquiped(player, EQUIPMENT_SLOT_LEGS))
		{
			Item* newItem7 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS);
			uint32 entry7 = sT->GetFakeEntry(newItem7->GetGUID());
			std::string icon7 = entry7 ? sT->GetItemIcon(entry7, 27, 27, -14, 0) : sT->GetItemIcon(newItem7->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon7+"Encantar Piernas", EQUIPMENT_SLOT_LEGS, GOSSIP_ACTION_INFO_DEF+8);
			count = true;
		}

		if(isEquiped(player, EQUIPMENT_SLOT_FEET))
		{
			Item* newItem8 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET);
			uint32 entry8 = sT->GetFakeEntry(newItem8->GetGUID());
			std::string icon8 = entry8 ? sT->GetItemIcon(entry8, 27, 27, -14, 0) : sT->GetItemIcon(newItem8->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon8+"Encantar Botas", EQUIPMENT_SLOT_FEET, GOSSIP_ACTION_INFO_DEF+9);
			count = true;
		}
		
        if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
		{
			if(isEquiped(player, EQUIPMENT_SLOT_FINGER1))
			{
				Item* newItem9 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1);
				uint32 entry9 = sT->GetFakeEntry(newItem9->GetGUID());
				std::string icon9 = entry9 ? sT->GetItemIcon(entry9, 27, 27, -14, 0) : sT->GetItemIcon(newItem9->GetEntry(), 27, 27, -14, 0);
				AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon9+"Encantar Anillo 1", EQUIPMENT_SLOT_FINGER1, GOSSIP_ACTION_INFO_DEF+10);
				count = true;
			}

			if(isEquiped(player, EQUIPMENT_SLOT_FINGER2))
			{
				Item* newItem10 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2);
				uint32 entry10 = sT->GetFakeEntry(newItem10->GetGUID());
				std::string icon10 = entry10 ? sT->GetItemIcon(entry10, 27, 27, -14, 0) : sT->GetItemIcon(newItem10->GetEntry(), 27, 27, -14, 0);
				AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon10+"Encantar Anillo 2", EQUIPMENT_SLOT_FINGER2, GOSSIP_ACTION_INFO_DEF+11);
				count = true;
			}
		}
		
		if(isEquiped(player, EQUIPMENT_SLOT_MAINHAND))
		{
			Item* newItem11 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
			uint32 entry11 = sT->GetFakeEntry(newItem11->GetGUID());
			std::string icon11 = entry11 ? sT->GetItemIcon(entry11, 27, 27, -14, 0) : sT->GetItemIcon(newItem11->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon11+"Encantar Mano Principal", EQUIPMENT_SLOT_MAINHAND, GOSSIP_ACTION_INFO_DEF+12);
			count = true;
		}
		
		if(isEquiped(player, EQUIPMENT_SLOT_OFFHAND))		
		{
			Item* newItem12 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			uint32 entry12 = sT->GetFakeEntry(newItem12->GetGUID());
			std::string icon12 = entry12 ? sT->GetItemIcon(entry12, 27, 27, -14, 0) : sT->GetItemIcon(newItem12->GetEntry(), 27, 27, -14, 0);
			AddGossipItemFor(player, GOSSIP_ICON_TRAINER, icon12+"Encantar Mano Secundaria", EQUIPMENT_SLOT_OFFHAND, GOSSIP_ACTION_INFO_DEF+13);
			count = true;
		}
		
		AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "|TInterface/ICONS/Ability_Spy:27:27:-14:0|tOlv""\xC3\xAD""dalo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+200);
		SendGossipMenuFor(player, 68, creature->GetGUID());

		if(!count)
            ChatHandler(player->GetSession()).PSendSysMessage("|cfffcedbbNo tienes objetos equipados que se puedan encantar.|r");

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
			case GOSSIP_ACTION_INFO_DEF+200:
               CloseGossipMenuFor(player);
                break;
            case GOSSIP_ACTION_INFO_DEF+1:
                OnGossipHello(player, creature);
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+50 AP y 20 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+21);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+50 AP y 20 Temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+16);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+30 SP y 10 MP5", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+19);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+30 SP y 20 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+20);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+29 SP y 20 Temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+17);
                /*AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Arcanum de la Luna Eclipsada", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+23);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Arcanum del alma de la llama", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+24);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Arcanum de la sombra huida", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+25);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Arcanum del alma helada", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+26);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Arcanum de amparo toxico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+27);*/
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+30 Aguante y 25 Temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+18);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+37 Aguante y 20 Defensa", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+22);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+16:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3795);
                break;
            case GOSSIP_ACTION_INFO_DEF+17:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3797);
                break;
            case GOSSIP_ACTION_INFO_DEF+18:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3842);
                break;
            case GOSSIP_ACTION_INFO_DEF+19:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3819);
                break;
            case GOSSIP_ACTION_INFO_DEF+20:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3820);
                break;
            case GOSSIP_ACTION_INFO_DEF+21:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3817);
                break;
            case GOSSIP_ACTION_INFO_DEF+22:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3818);
                break;
            case GOSSIP_ACTION_INFO_DEF+23:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3815);
                break;
            case GOSSIP_ACTION_INFO_DEF+24:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3816);
                break;
            case GOSSIP_ACTION_INFO_DEF+25:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3814);
                break;
            case GOSSIP_ACTION_INFO_DEF+26:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3812);
                break;
            case GOSSIP_ACTION_INFO_DEF+27:
                Enchant(player, creature, EQUIPMENT_SLOT_HEAD, 3813);
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
				if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) == 450)
                {
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+120 AP y 15 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+31);
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+70 SP y 8 MP5", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+32);
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+70 SP y 15 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+34);
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+70 Esquivar y 15 Defensa", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+33);
				}
				AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+30 Aguante y 15 Temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+28);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+40 AP y 15 Temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+29);
				AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+40 AP y 15 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+195);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+23 SP y 15 Temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+30);
				AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+24 SP y 15 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+197);
				AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+24 SP y 8 MP5", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+196);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+28:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3852);
                break;
            case GOSSIP_ACTION_INFO_DEF+29:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3793);
                break;
            case GOSSIP_ACTION_INFO_DEF+30:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3794);
                break;
            case GOSSIP_ACTION_INFO_DEF+31:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3835);
                break;
            case GOSSIP_ACTION_INFO_DEF+32:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3836);
                break;
            case GOSSIP_ACTION_INFO_DEF+33:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3837);
                break;
            case GOSSIP_ACTION_INFO_DEF+34:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3838);
                break;
            case GOSSIP_ACTION_INFO_DEF+195:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3808);
                break;
            case GOSSIP_ACTION_INFO_DEF+196:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3809);
                break;
            case GOSSIP_ACTION_INFO_DEF+197:
                Enchant(player, creature, EQUIPMENT_SLOT_SHOULDERS, 3810);
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+10 Agilidad y Sigilo aumentado", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+35);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+22 Agilidad", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+40);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+23 Indice de Celeridad", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+38);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+35 Penetracion de hechizos", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+41);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+10 Espiritu y -2% Amenaza", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+36);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+225 Armadura", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+39);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+16 Indice de Defensa", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+37);
				if (player->HasSkill(SKILL_TAILORING) && player->GetSkillValue(SKILL_TAILORING) == 450)
                {
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Bordado de resplandor oscuro", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+42);
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Bordado de tejido de luz", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+43);
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Bordado de guardia de espada", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+44);
				}
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+35:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 3256);
                break;
            case GOSSIP_ACTION_INFO_DEF+36:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 3296);
                break;
            case GOSSIP_ACTION_INFO_DEF+37:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 1951);
                break;
            case GOSSIP_ACTION_INFO_DEF+38:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 3831);
                break;
            case GOSSIP_ACTION_INFO_DEF+39:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 3294);
                break;
            case GOSSIP_ACTION_INFO_DEF+40:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 1099);
                break;
            case GOSSIP_ACTION_INFO_DEF+41:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 3243);
                break;
            case GOSSIP_ACTION_INFO_DEF+42:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 3728);
                break;
            case GOSSIP_ACTION_INFO_DEF+43:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 3722);
                break;
            case GOSSIP_ACTION_INFO_DEF+44:
                Enchant(player, creature, EQUIPMENT_SLOT_BACK, 3730);
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+10 Todas las estadisticas", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+47);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+275 Puntos de salud", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+48);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+10 MP5", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+49);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+20 Temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+50);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+20 Indice de Defensa", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+51);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+47:
                Enchant(player, creature, EQUIPMENT_SLOT_CHEST, 3832);
                break;
            case GOSSIP_ACTION_INFO_DEF+48:
                Enchant(player, creature, EQUIPMENT_SLOT_CHEST, 3297);
                break;
            case GOSSIP_ACTION_INFO_DEF+49:
                Enchant(player, creature, EQUIPMENT_SLOT_CHEST, 2381);
                break;
            case GOSSIP_ACTION_INFO_DEF+50:
                Enchant(player, creature, EQUIPMENT_SLOT_CHEST, 3245);
                break;
            case GOSSIP_ACTION_INFO_DEF+51:
                Enchant(player, creature, EQUIPMENT_SLOT_CHEST, 1953);
                break;
            case GOSSIP_ACTION_INFO_DEF+6:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
				if (player->HasSkill(SKILL_LEATHERWORKING) && player->GetSkillValue(SKILL_LEATHERWORKING) == 450)
                {
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Forro de pelaje de Aguante", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+52);
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Forro de pelaje de Poder con Hechizos", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+53);
					AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Forro de pelaje de Poder de Ataque", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+54);
				}
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+18 Espiritu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+55);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+15 Pericia", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+56);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+6 Todas las estadisticas", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+57);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+16 Intelecto", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+58);
				AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+30 Poder con hechizos", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+122);
				AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+50 Poder de ataque", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+123);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+52:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 3757);
                break;
            case GOSSIP_ACTION_INFO_DEF+53:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 3758);
                break;
            case GOSSIP_ACTION_INFO_DEF+54:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 3756);
                break;
            case GOSSIP_ACTION_INFO_DEF+55:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 1147);
                break;
            case GOSSIP_ACTION_INFO_DEF+56:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 3231);
                break;
            case GOSSIP_ACTION_INFO_DEF+57:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 2661);
                break;
            case GOSSIP_ACTION_INFO_DEF+58:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 1119);
                break;
            case GOSSIP_ACTION_INFO_DEF+122:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 2332);
                break;
            case GOSSIP_ACTION_INFO_DEF+123:
                Enchant(player, creature, EQUIPMENT_SLOT_WRISTS, 3845);
                break;
            case GOSSIP_ACTION_INFO_DEF+7:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+16 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+59);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+10 Indice de Parada y 2% Amenaza", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+60);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+44 Poder de ataque", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+61);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+28 Poder con hechizos", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+120);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+20 Agilidad", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+62);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+20 Indice de Golpe", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+63);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+15 Indice de Pericia", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+64);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+59:
                Enchant(player, creature, EQUIPMENT_SLOT_HANDS, 3249);
                break;
            case GOSSIP_ACTION_INFO_DEF+60:
                Enchant(player, creature, EQUIPMENT_SLOT_HANDS, 3253);
                break;
            case GOSSIP_ACTION_INFO_DEF+61:
                Enchant(player, creature, EQUIPMENT_SLOT_HANDS, 1603);
                break;
            case GOSSIP_ACTION_INFO_DEF+62:
                Enchant(player, creature, EQUIPMENT_SLOT_HANDS, 3222);
                break;
            case GOSSIP_ACTION_INFO_DEF+63:
                Enchant(player, creature, EQUIPMENT_SLOT_HANDS, 3234);
                break;
            case GOSSIP_ACTION_INFO_DEF+64:
                Enchant(player, creature, EQUIPMENT_SLOT_HANDS, 3231);
                break;
            case GOSSIP_ACTION_INFO_DEF+120:
                Enchant(player, creature, EQUIPMENT_SLOT_HANDS, 3246);
                break;
            case GOSSIP_ACTION_INFO_DEF+121:
                Enchant(player, creature, EQUIPMENT_SLOT_HANDS, 3603);
                break;
            case GOSSIP_ACTION_INFO_DEF+8:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+40 Temple y 28 Aguante", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+65);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+55 Aguante y 22 Agilidad", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+66);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+75 AP y 22 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+67);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+50 SP y 20 Espiritu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+68);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+50 SP y 30 Aguante", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+69);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+65:
                Enchant(player, creature, EQUIPMENT_SLOT_LEGS, 3853);
                break;
            case GOSSIP_ACTION_INFO_DEF+66:
                Enchant(player, creature, EQUIPMENT_SLOT_LEGS, 3822);
                break;
            case GOSSIP_ACTION_INFO_DEF+67:
                Enchant(player, creature, EQUIPMENT_SLOT_LEGS, 3823);
                break;
            case GOSSIP_ACTION_INFO_DEF+68:
                Enchant(player, creature, EQUIPMENT_SLOT_LEGS, 3719);
                break;
            case GOSSIP_ACTION_INFO_DEF+69:
                Enchant(player, creature, EQUIPMENT_SLOT_LEGS, 3721);
                break;
            case GOSSIP_ACTION_INFO_DEF+9:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+32 Poder de ataque", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+70);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+15 Aguante y Velocidad aumentada", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+71);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+16 Agilidad", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+72);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+18 Espiritu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+73);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+7 Salud y Mana cada 5s", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+74);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+12 Indice de golpe y 12 Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+75);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+22 Aguante", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+76);
                //AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Propulsiones de nitro", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+77);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+70:
                Enchant(player, creature, EQUIPMENT_SLOT_FEET, 1597);
                break;
            case GOSSIP_ACTION_INFO_DEF+71:
                Enchant(player, creature, EQUIPMENT_SLOT_FEET, 3232);
                break;
            case GOSSIP_ACTION_INFO_DEF+72:
                Enchant(player, creature, EQUIPMENT_SLOT_FEET, 983);
                break;
            case GOSSIP_ACTION_INFO_DEF+73:
                Enchant(player, creature, EQUIPMENT_SLOT_FEET, 1147);
                break;
            case GOSSIP_ACTION_INFO_DEF+74:
                Enchant(player, creature, EQUIPMENT_SLOT_FEET, 3244);
                break;
            case GOSSIP_ACTION_INFO_DEF+75:
                Enchant(player, creature, EQUIPMENT_SLOT_FEET, 3826);
                break;
            case GOSSIP_ACTION_INFO_DEF+76:
                Enchant(player, creature, EQUIPMENT_SLOT_FEET, 1075);
                break;
            case GOSSIP_ACTION_INFO_DEF+77:
                Enchant(player, creature, EQUIPMENT_SLOT_FEET, 3606);
                break;
            case GOSSIP_ACTION_INFO_DEF+10:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Asalto", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+78);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Poder con hechizos superior", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+79);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Aguante", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+80);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+78:
                Enchant(player, creature, EQUIPMENT_SLOT_FINGER1, 3839);
                break;
            case GOSSIP_ACTION_INFO_DEF+79:
                Enchant(player, creature, EQUIPMENT_SLOT_FINGER1, 3840);
                break;
            case GOSSIP_ACTION_INFO_DEF+80:
                Enchant(player, creature, EQUIPMENT_SLOT_FINGER1, 3791);
                break;
            case GOSSIP_ACTION_INFO_DEF+11:
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Asalto", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+81);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Poder con hechizos superior", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+82);
                AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Aguante", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+83);
                SendGossipMenuFor(player, 68, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+81:
                Enchant(player, creature, EQUIPMENT_SLOT_FINGER2, 3839);
                break;
            case GOSSIP_ACTION_INFO_DEF+82:
                Enchant(player, creature, EQUIPMENT_SLOT_FINGER2, 3840);
                break;
            case GOSSIP_ACTION_INFO_DEF+83:
                Enchant(player, creature, EQUIPMENT_SLOT_FINGER2, 3791);
                break;
            case GOSSIP_ACTION_INFO_DEF+12:
            {
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

                Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                if (!item)
                    return true;

                if (validItem(2, item))
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Masacre", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+98);
                if (validItem(3, item))
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Poder con Hechizos superior", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+99);
                if (validItem(1, item))
                {
                    if (!validItem(2, item))
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Potencia Superior", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+89);
                    if (!validItem(3, item))
                        AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Poder con Hechizos Poderoso", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+88);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Precision", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+85);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Agilidad Excepcional", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+94);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Espiritu Excepcional", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+95);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Guardia Titan", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+84);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Rabiar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+86);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Magia Negra", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+87);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Verdugo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+96);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Mangosta", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+97);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Rompehielo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+90);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Guardian de vida", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+91);
					//AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Cadena de titanio", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+199);
					if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
                    {
						AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Drenador de sangre", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+92);
						AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Amparo de hojas", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+93);
					}
                }

                SendGossipMenuFor(player, 68, creature->GetGUID());
            }   break;
            case GOSSIP_ACTION_INFO_DEF+84:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3851, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+85:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3788, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+86:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3789, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+87:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3790, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+88:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3834, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+89:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3833, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+90:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3239, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+91:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3241, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+92:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3870, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+93:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3869, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+94:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 1103, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+95:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3844, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+96:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3225, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+97:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 2673, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+98:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3827, 2);
                break;
            case GOSSIP_ACTION_INFO_DEF+99:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3854, 3);
                break;
			/*case GOSSIP_ACTION_INFO_DEF+199:
                Enchant(player, creature, EQUIPMENT_SLOT_MAINHAND, 3731, 1);
                break;*/
            case GOSSIP_ACTION_INFO_DEF+13:
            {
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "<- Volver al Men""\xC3\xBA"" principal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

                Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (!item)
                    return true;

                if (validItem(1, item))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Potencia Superior", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+111);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Poder con Hechizos Poderoso", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+110);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "+25 Indice de Golpe y Golpe Critico", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+107);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Agilidad Excepcional", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+116);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Espiritu Excepcional", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+117);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Guardia Titan", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+106);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Rabiar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+108);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Magia Negra", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+109);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Verdugo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+118);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Mangosta", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+119);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Rompehielo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+112);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Guardian de vida", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+113);
					//AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Cadena de titanio", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+198);
					
					if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
                    {
						AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Drenador de sangre", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+92);
						AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Amparo de hojas", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+93);
					}
                }
                if (validItem(4, item))
                {
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Encantar Escudo: +25 Intelecto", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+101);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Encantar Escudo: +15 Bloqueo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+102);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Encantar Escudo: +36 Valor de Bloqueo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+105);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Encantar Escudo: +20 Defensa", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+100);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Encantar Escudo: +18 Aguante", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+104);
                    AddGossipItemFor(player, GOSSIP_ICON_TRAINER, "Encantar Escudo: +12 Temple", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+103);
                }

                SendGossipMenuFor(player, 68, creature->GetGUID());
            }   break;
            case GOSSIP_ACTION_INFO_DEF+100:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 1952, 4);
                break;
            case GOSSIP_ACTION_INFO_DEF+101:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 1128, 4);
                break;
            case GOSSIP_ACTION_INFO_DEF+102:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 2655, 4);
                break;
            case GOSSIP_ACTION_INFO_DEF+103:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3229, 4);
                break;
            case GOSSIP_ACTION_INFO_DEF+104:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 1071, 4);
                break;
            case GOSSIP_ACTION_INFO_DEF+105:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 2653, 4);
                break;
            case GOSSIP_ACTION_INFO_DEF+106:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3851, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+107:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3788, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+108:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3789, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+109:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3790, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+110:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3834, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+111:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3833, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+112:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3239, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+113:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3241, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+114:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3870, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+115:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3869, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+116:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 1103, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+117:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3844, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+118:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3225, 1);
                break;
            case GOSSIP_ACTION_INFO_DEF+119:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 2673, 1);
                break;
			/*case GOSSIP_ACTION_INFO_DEF+198:
                Enchant(player, creature, EQUIPMENT_SLOT_OFFHAND, 3731, 1);
                break;*/
        }
        return true;
    }
};

void AddSC_npc_enchant()
{
    new npc_enchant;
}
