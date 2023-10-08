#include "Companion.hpp"
#include "Engine/Core/TextOutBox.hpp"
#include "World/Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Companion::Pimpl
    /*//////////////////*/
    {
      public:
        Point2 spawn_location_;
        int stamina_ = 50;
        int max_stamina_ = 50;
        int ticks_last_movement_ = 0;
        int movement_speed_ = 200;
        int radius_ = 1;
        RotationDirections direction_ = RotationDirections::Clockwise;
        float angle_ = 0.0f;
        Point2 previous_coordinate_ = {-1, -1};
    };

    Companion::Companion(Point2 spawnLocation_)
        : p(std::make_shared<Pimpl>())
    /*///////////////////////////////////////*/
    {
        p->spawn_location_ = spawnLocation_;
    }

    void
    Companion::GiveObject(std::shared_ptr<Object> object)
    /*/////////////////////////////////////////////////*/
    {
        if (object->GetObjectType() == Hash("ObjectRedApple") ||
            object->GetObjectType() == Hash("ObjectCookedFish"))
        /******************************************************/
        {
            TextOutBox::Get()->Print("Companion: Thanks for the food, I feel more energized.");
            p->stamina_ = std::min(p->max_stamina_, p->stamina_ + 30);
        }
        else
        /**/
        {
            TextOutBox::Get()->Print("Companion: Thanks for the gift.");
        }
    }

    int
    Companion::GetStamina()
    /*///////////////////*/
    {
        return p->stamina_;
    }

    int
    Companion::GetMaxStamina()
    /*//////////////////////*/
    {
        return p->max_stamina_;
    }

    int
    Companion::GetTicksLastMovement()
    /*/////////////////////////////*/
    {
        return p->ticks_last_movement_;
    }

    int
    Companion::GetMoveSpeed()
    /*/////////////////////*/
    {
        return p->movement_speed_;
    }

    float
    Companion::GetAngle()
    /*/////////////////*/
    {
        return p->angle_;
    }

    int
    Companion::GetRadius()
    /*//////////////////*/
    {
        return p->radius_;
    }

    RotationDirections
    Companion::GetDirection()
    /*/////////////////////*/
    {
        return p->direction_;
    }

    void
    Companion::IncreaseRadius()
    /*///////////////////////*/
    {
        p->radius_++;
    }

    Point2
    Companion::GetSpawnLocation()
    /*/////////////////////////*/
    {
        return p->spawn_location_;
    }

    void
    Companion::SetAngle(float newAngle)
    /*///////////////////////////////*/
    {
        p->angle_ = newAngle;
    }

    void
    Companion::UpdateTicksLastMovement()
    /*////////////////////////////////*/
    {
        p->ticks_last_movement_ = SDL_GetTicks();
    }

    void
    Companion::ConsumeStamina(int amountStamina)
    /*////////////////////////////////////////*/
    {
        p->stamina_ -= amountStamina;
    }

    bool
    Companion::OutOfStamina()
    /*/////////////////////*/
    {
        return p->stamina_ <= 0;
    }

    void
    Companion::ReverseDirection()
    /*/////////////////////////*/
    {
        if (p->direction_ == RotationDirections::Clockwise)
            p->direction_ = RotationDirections::CounterClockwise;
        else
            p->direction_ = RotationDirections::Clockwise;
    }

    void
    Companion::DecreaseRadius()
    /*///////////////////////*/
    {
        p->radius_--;
    }

    void
    Companion::SetPreviousCoordinate(Point2 newPreviousCoordinate)
    /*//////////////////////////////////////////////////////////*/
    {
        p->previous_coordinate_ = newPreviousCoordinate;
    }

    Point2
    Companion::GetPreviousCoordinate()
    /*//////////////////////////////*/
    {
        return p->previous_coordinate_;
    }
}