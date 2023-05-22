#include "ScriptPCH.h"

class go_cofre_gurubashi : public GameObjectScript
{
public:
    go_cofre_gurubashi() : GameObjectScript("go_cofre_gurubashi") { }

	int32 GetValor()
	{
		QueryResult valor = WorldDatabase.Query("SELECT start FROM evento_gurubashi_reliquias");

		if (!valor)
			return 0;
		else
		{
			Field *fields = valor->Fetch();
				return fields[0].Get<uint32>();
		}
	}
	
	int32 GetTextoUnico()
	{
		QueryResult valor2 = WorldDatabase.Query("SELECT texto_unico FROM evento_gurubashi_reliquias");

		if (!valor2)
			return 0;
		else
		{
			Field *fields = valor2->Fetch();
				return fields[0].Get<uint32>();
		}
	}
	
	uint8 tiempo;

	void OnGameObjectRemove(GameObject* go)
    {
		//if (GetValor() != 0)
		//{
			WorldDatabase.Execute("UPDATE evento_gurubashi_reliquias SET start = 0");
		//	if(GetTextoUnico() == 1)
		//	{
				sWorld->SendGlobalText("El evento de Gurubashi ha llegado a su fin, porque alguien ha looteado el Cofre de Gurubashi. Ma""\xC3\xB1""ana comenzar""\xC3\xA1"" otro evento. ""\xc2\xa1""Estad atentos!.", NULL);	
				WorldDatabase.Execute("UPDATE evento_gurubashi_reliquias SET texto_unico = 0");
		//	}
		//}
	}
};

void AddSC_cofre_gurubashi()
{
    new go_cofre_gurubashi();
}
