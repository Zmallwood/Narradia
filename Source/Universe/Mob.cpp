//////////////////////////////////////////////////////////////////////
#include "Mob.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Mob::Pimpl
    /*////////////*/ {
      public:
        int mobType = 0;
        float distanceAboveGround = 0.0f;
        int ticksLastMovement = 0;
        int moveSpeed = 1000;
        Point2 destination = {-1, -1};
        Point2 previousCoordinate = {-1, -1};
        Point2 spawnLocation;
        int hp = 10;
        int maxHp = 10;
        int ticksLastHitRecieved = 0;
        bool aggroingPlayer = false;
        int ticksLastAttack = 0;
        int attackSpeed = 700;
    }; // Class

    Mob::Mob(int mobType_, int spawnX_, int spawnY_)
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////*/ {
        p->mobType = mobType_;
        p->spawnLocation = {spawnX_, spawnY_};
    } // Function

    int Mob::GetTicksLastMovement()
    /*///////////////////////////*/ { return p->ticksLastMovement; } // Function

    int Mob::GetMoveSpeed()
    /*///////////////////*/ { return p->moveSpeed; } // Function

    void Mob::ClearDestination()
    /*////////////////////////*/ { p->destination = {-1, -1}; } // Function

    bool Mob::HasNoDestination()
    /*////////////////////////*/ { return p->destination == Point2{-1, -1}; } // Function

    void Mob::SetDestination(Point2 newDestination)
    /*///////////////////////////////////////////*/ { p->destination = newDestination; } // Function

    Point2 Mob::GetDestination()
    /*////////////////////////*/ { return p->destination; } // Function

    Point2 Mob::GetPreviousCoordinate()
    /*///////////////////////////////*/ { return p->previousCoordinate; } // Function

    int Mob::GetMobType()
    /*/////////////////*/ { return p->mobType; } // Function

    void Mob::UpdateTicksLastMovement()
    /*///////////////////////////////*/ { p->ticksLastMovement = SDL_GetTicks(); } // Function

    void Mob::SetPreviousCoordinate(Point2 newPreviousCoordinate)
    /*/////////////////////////////////////////////////////////*/ {
        p->previousCoordinate = newPreviousCoordinate;
    } // Function

    float Mob::GetDistanceAboveGround()
    /*///////////////////////////////*/ { return p->distanceAboveGround; } // Function

    void Mob::SetDistanceAboveGround(float newDistanceAboveGround)
    /*//////////////////////////////////////////////////////////*/ {
        p->distanceAboveGround = newDistanceAboveGround;
    } // Function

    void Mob::SetMoveSpeed(int newMoveSpeed)
    /*////////////////////////////////////*/ { p->moveSpeed = newMoveSpeed; } // Function

    int Mob::GetSpawnX()
    /*////////////////*/ { return p->spawnLocation.x; } // Function

    int Mob::GetSpawnY()
    /*////////////////*/ { return p->spawnLocation.y; } // Function

    int Mob::GetTicksLastHitRecieved()
    /*//////////////////////////////*/ { return p->ticksLastHitRecieved; } // Function

    void Mob::StartAggroPlayer()
    /*////////////////////////*/ { p->aggroingPlayer = true; } // Function

    bool Mob::IsAggroingPlayer()
    /*////////////////////////*/ { return p->aggroingPlayer; } // Function

    int Mob::GetTicksLastAttack()
    /*/////////////////////////*/ { return p->ticksLastAttack; } // Function

    int Mob::GetAttackSpeed()
    /*/////////////////////*/ { return p->attackSpeed; } // Function

    void Mob::UpdateTicksLastAttack()
    /*/////////////////////////////*/ { p->ticksLastAttack = SDL_GetTicks(); } // Function
}
//////////////////////////////////////////////////////////////////////