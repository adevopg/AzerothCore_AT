/******************
Creado por: Darthye
******************/

#include "ScriptPCH.h"

class npc_waypoint : public CreatureScript
{
    public:
        npc_waypoint() : CreatureScript("npc_waypoint") { }
        
        struct npc_waypointAI : public ScriptedAI
        {
            npc_waypointAI(Creature * c) : ScriptedAI(c) { }
        
            int CountMovimiento;
            
            void Reset()
            {
                CountMovimiento = 0;
                me->SetSpeed(MOVE_RUN, 0.63f, true);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;
                CountMovimiento++;
            }
                        
            void UpdateAI(uint32 diff)
            {
                switch(CountMovimiento)
                {                    
                                        case 0:
                        me->GetMotionMaster()->MovePoint(1, -13181.589844f, 317.612366f, 33.241516f);
                        break;
                        
                                        case 1:
                        me->GetMotionMaster()->MovePoint(2, -13166.786133f, 306.249664f, 33.241516f);
                        break;
                        
                                        case 2:
                        me->GetMotionMaster()->MovePoint(3, -13158.435547f, 293.052094f, 33.241520f);
                        break;
                        
                                        case 3:
                        me->GetMotionMaster()->MovePoint(4, -13154.561523f, 275.506836f, 33.233864f);
                        break;
                        
                                        case 4:
                        me->GetMotionMaster()->MovePoint(5, -13155.096680f, 267.170471f, 33.232922f);
                        break;
						
					case 5:
                        me->GetMotionMaster()->MovePoint(6, -13158.646484f, 252.055786f, 33.240597f);
                        break;
					
					case 6:
                        me->GetMotionMaster()->MovePoint(7, -13167.969727f, 238.082458f, 33.241215f);
                        break;
						
					case 7:
                        me->GetMotionMaster()->MovePoint(8, -13183.056641f, 227.075531f, 33.241215f);
                        break;
						
					case 8:
                        me->GetMotionMaster()->MovePoint(9, -13204.732422f, 221.843506f, 33.219696f);
                        break;
						
					case 9:
                        me->GetMotionMaster()->MovePoint(10, -13220.266602f, 223.607681f, 33.240986f);
                        break;
						
					case 10:
                        me->GetMotionMaster()->MovePoint(11, -13234.599609f, 239.267319f, 33.351437f);
                        break;
					
					case 11:
                        me->GetMotionMaster()->MovePoint(12, -13245.307617f, 259.498108f, 21.858171f);
                        break;
						
					case 12:
                        me->GetMotionMaster()->MovePoint(13, -13239.383789f, 269.705292f, 21.858171f);
                        break;
						
					case 13:
                        me->GetMotionMaster()->MovePoint(14, -13226.435547f, 275.875061f, 21.858171f);
                        break;						
                        
                                        case 14:
                        me->GetMotionMaster()->MovePoint(15, -13206.138672f, 278.040771f, 21.858171f);
                        break;      	

                       			case 15:
                        me->GetMotionMaster()->MovePoint(16, -13205.928711f, 278.072113f, 21.858335f);
                        Player* player = ObjectAccessor::GetPlayer(*me, me->GetGUID());
                        sWorld->SendPlayerText(player, """\xc2\xa1""Que comience la masacre!");
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 353);
                        CountMovimiento = 16;
                        break;
                }
            }
        };
        
        CreatureAI * GetAI(Creature * pCreature) const
        {
            return new npc_waypointAI(pCreature);
        }
};


void AddSC_npc_waypoint()
{
    new npc_waypoint();
}
