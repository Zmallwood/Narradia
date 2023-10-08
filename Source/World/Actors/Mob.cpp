#include "Mob.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Mob::Pimpl
    /*////////////*/
    {
      public:
        int mob_type_ = 0;
        float distance_above_ground_ = 0.0f;
        int ticks_last_movement_ = 0;
        int movement_speed_ = 1000;
        Point2 destination_ = {-1, -1};
        Point2 previous_coordinate_ = {-1, -1};
        Point2 spawn_location_;
        int health_ = 10;
        int max_health_ = 10;
        int ticks_last_hit_recieved_ = 0;
        bool aggroing_player_ = false;
        int ticks_last_attack_ = 0;
        int attack_speed_ = 700;
    };

    Mob::Mob(int mobType_, int spawnX_, int spawnY_)
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////*/
    {
        p->mob_type_ = mobType_;
        p->spawn_location_ = {spawnX_, spawnY_};
    }

    int
    Mob::GetTicksLastMovement()
    /*///////////////////////*/
    {
        return p->ticks_last_movement_;
    }

    int
    Mob::GetMoveSpeed()
    /*///////////////*/
    {
        return p->movement_speed_;
    }

    void
    Mob::ClearDestination()
    /*///////////////////*/
    {
        p->destination_ = {-1, -1};
    }

    bool
    Mob::HasNoDestination()
    /*///////////////////*/
    {
        return p->destination_ == Point2{-1, -1};
    }

    void
    Mob::SetDestination(Point2 newDestination)
    /*//////////////////////////////////////*/
    {
        p->destination_ = newDestination;
    }

    Point2
    Mob::GetDestination()
    /*/////////////////*/
    {
        return p->destination_;
    }

    Point2
    Mob::GetPreviousCoordinate()
    /*////////////////////////*/
    {
        return p->previous_coordinate_;
    }

    int
    Mob::GetMobType()
    /*/////////////*/
    {
        return p->mob_type_;
    }

    void
    Mob::UpdateTicksLastMovement()
    /*//////////////////////////*/
    {
        p->ticks_last_movement_ = SDL_GetTicks();
    }

    void
    Mob::SetPreviousCoordinate(Point2 newPreviousCoordinate)
    /*////////////////////////////////////////////////////*/
    {
        p->previous_coordinate_ = newPreviousCoordinate;
    }

    float
    Mob::GetDistanceAboveGround()
    /*/////////////////////////*/
    {
        return p->distance_above_ground_;
    }

    void
    Mob::SetDistanceAboveGround(float newDistanceAboveGround)
    /*/////////////////////////////////////////////////////*/
    {
        p->distance_above_ground_ = newDistanceAboveGround;
    }

    void
    Mob::SetMoveSpeed(int newMoveSpeed)
    /*///////////////////////////////*/
    {
        p->movement_speed_ = newMoveSpeed;
    }

    int
    Mob::GetSpawnX()
    /*////////////*/
    {
        return p->spawn_location_.x;
    }

    int
    Mob::GetSpawnY()
    /*/////////////*/
    {
        return p->spawn_location_.y;
    }

    int
    Mob::GetTicksLastHitRecieved()
    /*//////////////////////////*/
    {
        return p->ticks_last_hit_recieved_;
    }

    void
    Mob::StartAggroPlayer()
    /*///////////////////*/
    {
        p->aggroing_player_ = true;
    }

    bool
    Mob::IsAggroingPlayer()
    /*///////////////////*/
    {
        return p->aggroing_player_;
    }

    int
    Mob::GetTicksLastAttack()
    /*/////////////////////*/
    {
        return p->ticks_last_attack_;
    }

    int
    Mob::GetAttackSpeed()
    /*/////////////////*/
    {
        return p->attack_speed_;
    }

    void
    Mob::UpdateTicksLastAttack()
    /*////////////////////////*/
    {
        p->ticks_last_attack_ = SDL_GetTicks();
    }

    void
    Mob::Hit(int damage)
    /*////////////////*/
    {
        p->health_ -= damage;
        p->ticks_last_hit_recieved_ = SDL_GetTicks();
    }
}