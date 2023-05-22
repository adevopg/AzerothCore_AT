/****************
* Créditos: Ray *
*****************/
#include "ScriptPCH.h"

//////////////////////////////////////////////////Shops///////////////////////////////////////////////////////////////

class go_shop_horda : public GameObjectScript
{
public:
    go_shop_horda() : GameObjectScript("go_shop_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
	 if (player->IsInCombat())
        {
	     player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
            return true;
        }

	if (player->GetTeamId() == HORDE || player->IsGameMaster())
	{
		player->TeleportTo(534, 5529.474609f, -2981.116699f, 1538.368164f, 5.997351f);
		/*if(!player->IsGameMaster())
			player->SetVisible(false);*/
	}
	else
	{
		player->GetSession()->SendNotification("|cff87CEFALo siento, no aceptamos Alianzas.|r");
	}
	return true;
    }
};

class go_shop_alianza : public GameObjectScript
{
public:
    go_shop_alianza() : GameObjectScript("go_shop_alianza") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
	 if (player->IsInCombat())
        {
	     player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
            return true;
        }
		if (player->GetTeamId() == ALLIANCE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5102.932617f, -1787.017944f, 1322.637573f, 0.119992f);
			/*if(!player->IsGameMaster())
				player->SetVisible(false);*/
		}
		else
		{
			player->GetSession()->SendNotification("|cff87CEFALo siento, no aceptamos Hordas.");
		}
	return true;
    }
};

//////////////////////////////////////////////////Trasmog///////////////////////////////////////////////////////////////

class go_transmog_horda : public GameObjectScript
{
public:
    go_transmog_horda() : GameObjectScript("go_transmog_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
	 if (player->IsInCombat())
        {
	     player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
            return true;
        }
		if (player->GetTeamId() == HORDE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5281.762207f, -2911.235107f, 1529.304321f, 2.857106f);
			/*if(!player->IsGameMaster())
				player->SetVisible(false);*/
		}
		else
		{
			player->GetSession()->SendNotification("|cff87CEFALo siento, no aceptamos Alianzas.");
		}
	return true;
    }
};

class go_transmog_alianza : public GameObjectScript
{
public:
    go_transmog_alianza() : GameObjectScript("go_transmog_alianza") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
	 if (player->IsInCombat())
        {
	     player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
            return true;
        }
		if (player->GetTeamId() == ALLIANCE || player->IsGameMaster())
		{
			player->TeleportTo(534, 4936.925781f, -1890.909180f, 1326.586304f, 3.310985f);
			/*if(!player->IsGameMaster())
				player->SetVisible(false);*/
		}
		else
		{
			player->GetSession()->SendNotification("|cff87CEFALo siento, no aceptamos Hordas.");
		}
	return true;
    }
};

//////////////////////////////////////////////////Profesiones///////////////////////////////////////////////////////////////

class go_profesiones_horda : public GameObjectScript
{
public:
    go_profesiones_horda() : GameObjectScript("go_profesiones_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
	 if (player->IsInCombat())
        {
	     player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
            return true;
        }
		if (player->GetTeamId() == HORDE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5175.881348f, -3033.416992f, 1570.757324f, 3.768412f);
			/*if(!player->IsGameMaster())
				player->SetVisible(false);*/
		}
		else
		{
			player->GetSession()->SendNotification("|cff87CEFALo siento, no aceptamos Alianzas.");
		}
	return true;
    }
};

class go_profesiones_alianza : public GameObjectScript
{
public:
    go_profesiones_alianza() : GameObjectScript("go_profesiones_alianza") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
	 if (player->IsInCombat())
        {
	     player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
            return true;
        }
		if (player->GetTeamId() == ALLIANCE || player->IsGameMaster())
		{
			player->TeleportTo(534, 4987.796875f, -1774.041260f, 1331.176758f, 2.242979f);
			/*if(!player->IsGameMaster())
				player->SetVisible(false);*/			
		}
		else
		{
			player->GetSession()->SendNotification("|cff87CEFALo siento, no aceptamos Hordas.");
		}
	return true;
    }
};

//////////////////////////////////////////////////Bosses (solo horda, ali=outdoor)///////////////////////////////////////////////////////////////

class go_boss_horda : public GameObjectScript
{
public:
    go_boss_horda() : GameObjectScript("go_boss_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
		player->TeleportTo(534, 5419.139160f, -2710.270020f, 1494.084595f, 2.765537f);
	return true;
    }
};

//////////////////////////////////////////////////Tabernero Horda (ali=shopali)///////////////////////////////////////////////////////////////

class go_tabernero_horda : public GameObjectScript
{
public:
    go_tabernero_horda() : GameObjectScript("go_tabernero_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
	 if (player->IsInCombat())
        {
	     player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
            return true;
        }
		if (player->GetTeamId() == HORDE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5341.537598f, -2990.847900f, 1538.732178f, 5.026989f);
			/*if(!player->IsGameMaster())
				player->SetVisible(false);*/
		}
		else
		{
			player->GetSession()->SendNotification("|cff87CEFALo siento, no aceptamos Alianzas.");
		}
	return true;
    }
};

//////////////////////////////////////////////////Shop Extended Horda///////////////////////////////////////////////////////////////

class go_extended_horda : public GameObjectScript
{
public:
    go_extended_horda() : GameObjectScript("go_extended_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
	 if (player->IsInCombat())
        {
	     player->GetSession()->SendNotification("Est""\xC3\xA1""s en combate.");
            return true;
        }
		if (player->GetTeamId() == HORDE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5527.606934f, -2816.124268f, 1499.963013f, 5.439089f);
			/*if(!player->IsGameMaster())
				player->SetVisible(false);*/
		}
		else
		{
			player->GetSession()->SendNotification("|cff87CEFALo siento, no aceptamos Alianzas.");
		}
	return true;
    }
};

//////////////////////////////////////////////////SALIDAS DE LAS CASAS (QUITAN INVIS)///////////////////////////////////////////////////////////////

class go_salida_shop_ali : public GameObjectScript
{
public:
    go_salida_shop_ali() : GameObjectScript("go_salida_shop_ali") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
		if (player->GetTeamId() == ALLIANCE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5073.0f, -1785.949951f, 1321.141968f, 3.26007f);
			/*if(!player->IsGameMaster())
				player->SetVisible(true);*/
		}
	return true;
    }
};

class go_salida_profesiones_ali : public GameObjectScript
{
public:
    go_salida_profesiones_ali() : GameObjectScript("go_salida_profesiones_ali") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
		if (player->GetTeamId() == ALLIANCE || player->IsGameMaster())
		{
			player->TeleportTo(534, 4987.569824f, -1750.209961f, 1331.280029f, 1.681590f);
			/*if(!player->IsGameMaster())
				player->SetVisible(true);*/
		}
	return true;
    }
};

class go_salida_transmog_ali : public GameObjectScript
{
public:
    go_salida_transmog_ali() : GameObjectScript("go_salida_transmog_ali") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
		if (player->GetTeamId() == ALLIANCE || player->IsGameMaster())
		{
			player->TeleportTo(534, 4954.765137f, -1891.054932f, 1326.059814f, 0.149133f);
			/*if(!player->IsGameMaster())
				player->SetVisible(true);*/
		}
	return true;
    }
};

class go_salida_shop_horda : public GameObjectScript
{
public:
    go_salida_shop_horda() : GameObjectScript("go_salida_shop_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
		if (player->GetTeamId() == HORDE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5506.825195f, -2967.955811f, 1537.681152f, 3.089043f);
			/*if(!player->IsGameMaster())
				player->SetVisible(true);*/
		}
	return true;
    }
};

class go_salida_tabernero_horda : public GameObjectScript
{
public:
    go_salida_tabernero_horda() : GameObjectScript("go_salida_tabernero_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
		if (player->GetTeamId() == HORDE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5327.930176f, -2970.100098f, 1537.969971f, 1.865080f);
			/*if(!player->IsGameMaster())
				player->SetVisible(true);*/
		}
	return true;
    }
};

class go_salida_profesiones_horda : public GameObjectScript
{
public:
    go_salida_profesiones_horda() : GameObjectScript("go_salida_profesiones_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
		if (player->GetTeamId() == HORDE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5198.867676f, -3023.852539f, 1568.884888f, 0.594297f);
			/*if(!player->IsGameMaster())
				player->SetVisible(true);*/
		}
	return true;
    }
};

class go_salida_transmog_horda : public GameObjectScript
{
public:
    go_salida_transmog_horda() : GameObjectScript("go_salida_transmog_horda") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
		if (player->GetTeamId() == HORDE || player->IsGameMaster())
		{
			player->TeleportTo(534, 5301.479980f, -2909.065430f, 1528.307251f, 5.984542f);
			/*if(!player->IsGameMaster())
				player->SetVisible(true);*/
		}
	return true;
    }
};

void AddSC_gameobjects_shop()
{
    new go_shop_horda();
	new go_shop_alianza();
	new go_transmog_horda();
	new go_transmog_alianza();
	new go_profesiones_horda();
	new go_profesiones_alianza();
	new go_boss_horda();
	new go_tabernero_horda();
	new go_extended_horda();
	new go_salida_shop_ali();
	new go_salida_profesiones_ali();
	new go_salida_transmog_ali();
	new go_salida_shop_horda();
	new go_salida_tabernero_horda();
	new go_salida_profesiones_horda();
	new go_salida_transmog_horda();
}
