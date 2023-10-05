//////////////////////////////////////////////////////////////////////
#include "Companion.hpp"
#include "Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Companion::Pimpl
    /*//////////////////*/ {
      public:
        Point2 spawnLocation;
        int stamina = 50;
        int maxStamina = 50;
        int ticksLastMovement = 0;
        int moveSpeed = 200;
        int radius = 1;
        Directions direction = Directions::Clockwise;
        float angle = 0.0f;
        Point2 previousCoordinate = {-1, -1};
    };

    Companion::Companion(Point2 spawnLocation_)
        : p(std::make_shared<Pimpl>())
    /*///////////////////////////////////////*/ {
        p->spawnLocation = spawnLocation_;
    }

    void Companion::GiveObject(std::shared_ptr<Object> object)
    /*//////////////////////////////////////////////////////*/ {
        if (object->GetObjectType() == Hash("ObjectRedApple") ||
            object->GetObjectType() == Hash("ObjectCookedFish"))
        /******************************************************/
        {
            TextOutBox::Get().Print("Companion: Thanks for the food, I feel more energized.");
            p->stamina = std::min(p->maxStamina, p->stamina + 30);
        } else
        /****/ {
            TextOutBox::Get().Print("Companion: Thanks for the gift.");
        }
    }

    int Companion::GetStamina()
    /*///////////////////////*/ {
        return p->stamina;
    }

    int Companion::GetMaxStamina()
    /*//////////////////////////*/ {
        return p->maxStamina;
    }

    int Companion::GetTicksLastMovement()
    /*/////////////////////////////////*/ {
        return p->ticksLastMovement;
    }

    int Companion::GetMoveSpeed()
    /*/////////////////////////*/ {
        return p->moveSpeed;
    }

    float Companion::GetAngle()
    /*///////////////////////*/ {
        return p->angle;
    }

    int Companion::GetRadius()
    /*//////////////////////*/ {
        return p->radius;
    }

    Directions Companion::GetDirection()
    /*////////////////////////////////*/ {
        return p->direction;
    }

    void Companion::IncreaseRadius()
    /*////////////////////////////*/ {
        p->radius++;
    }

    Point2 Companion::GetSpawnLocation()
    /*////////////////////////////////*/ {
        return p->spawnLocation;
    }

    void Companion::SetAngle(float newAngle)
    /*////////////////////////////////////*/ {
        p->angle = newAngle;
    }

    void Companion::UpdateTicksLastMovement()
    /*/////////////////////////////////////*/ {
        p->ticksLastMovement = SDL_GetTicks();
    }

    void Companion::ConsumeStamina(int amountStamina)
    /*/////////////////////////////////////////////*/ {
        p->stamina -= amountStamina;
    }

    bool Companion::OutOfStamina()
    /*//////////////////////////*/ {
        return p->stamina <= 0;
    }

    void Companion::ReverseDirection()
    /*//////////////////////////////*/ {
        if (p->direction == Directions::Clockwise)
            p->direction = Directions::CounterClockwise;
        else
            p->direction = Directions::Clockwise;
    }

    void Companion::DecreaseRadius()
    /*////////////////////////////*/ {
        p->radius--;
    }

    void Companion::SetPreviousCoordinate(Point2 newPreviousCoordinate)
    /*///////////////////////////////////////////////////////////////*/ {
        p->previousCoordinate = newPreviousCoordinate;
    }

    Point2 Companion::GetPreviousCoordinate()
    /*/////////////////////////////////////*/ {
        return p->previousCoordinate;
    }
}
//////////////////////////////////////////////////////////////////////