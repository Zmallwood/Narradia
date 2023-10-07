//////////////////////////////////////////////////////////////////////
#include "Mob.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Mob::Pimpl
    /*////////////*/
    {
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
    };

    Mob::Mob(int mobType_, int spawnX_, int spawnY_)
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////*/
    {
        p->mobType = mobType_;
        p->spawnLocation = {spawnX_, spawnY_};
    }

    int
    Mob::GetTicksLastMovement()
    /*///////////////////////*/
    {
        return p->ticksLastMovement;
    }

    int
    Mob::GetMoveSpeed()
    /*///////////////*/
    {
        return p->moveSpeed;
    }

    void
    Mob::ClearDestination()
    /*///////////////////*/
    {
        p->destination = {-1, -1};
    }

    bool
    Mob::HasNoDestination()
    /*///////////////////*/
    {
        return p->destination == Point2{-1, -1};
    }

    void
    Mob::SetDestination(Point2 newDestination)
    /*//////////////////////////////////////*/
    {
        p->destination = newDestination;
    }

    Point2
    Mob::GetDestination()
    /*/////////////////*/
    {
        return p->destination;
    }

    Point2
    Mob::GetPreviousCoordinate()
    /*////////////////////////*/
    {
        return p->previousCoordinate;
    }

    int
    Mob::GetMobType()
    /*/////////////*/
    {
        return p->mobType;
    }

    void
    Mob::UpdateTicksLastMovement()
    /*//////////////////////////*/
    {
        p->ticksLastMovement = SDL_GetTicks();
    }

    void
    Mob::SetPreviousCoordinate(Point2 newPreviousCoordinate)
    /*////////////////////////////////////////////////////*/
    {
        p->previousCoordinate = newPreviousCoordinate;
    }

    float
    Mob::GetDistanceAboveGround()
    /*/////////////////////////*/
    {
        return p->distanceAboveGround;
    }

    void
    Mob::SetDistanceAboveGround(float newDistanceAboveGround)
    /*/////////////////////////////////////////////////////*/
    {
        p->distanceAboveGround = newDistanceAboveGround;
    }

    void
    Mob::SetMoveSpeed(int newMoveSpeed)
    /*///////////////////////////////*/
    {
        p->moveSpeed = newMoveSpeed;
    }

    int
    Mob::GetSpawnX()
    /*////////////*/
    {
        return p->spawnLocation.x;
    }

    int
    Mob::GetSpawnY()
    /*/////////////*/
    {
        return p->spawnLocation.y;
    }

    int
    Mob::GetTicksLastHitRecieved()
    /*//////////////////////////*/
    {
        return p->ticksLastHitRecieved;
    }

    void
    Mob::StartAggroPlayer()
    /*///////////////////*/
    {
        p->aggroingPlayer = true;
    }

    bool
    Mob::IsAggroingPlayer()
    /*///////////////////*/
    {
        return p->aggroingPlayer;
    }

    int
    Mob::GetTicksLastAttack()
    /*/////////////////////*/
    {
        return p->ticksLastAttack;
    }

    int
    Mob::GetAttackSpeed()
    /*/////////////////*/
    {
        return p->attackSpeed;
    }

    void
    Mob::UpdateTicksLastAttack()
    /*////////////////////////*/
    {
        p->ticksLastAttack = SDL_GetTicks();
    }
}
//////////////////////////////////////////////////////////////////////