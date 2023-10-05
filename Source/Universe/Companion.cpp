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
    }; // Class

    Companion::Companion(Point2 spawnLocation_)
        : p(std::make_shared<Pimpl>())
    /*///////////////////////////////////////*/ { p->spawnLocation = spawnLocation_; } // Function

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
    } // Function

    int Companion::GetStamina()
    /*///////////////////////*/ { return p->stamina; } // Function

    int Companion::GetMaxStamina()
    /*//////////////////////////*/ { return p->maxStamina; } // Function

    int Companion::GetTicksLastMovement()
    /*/////////////////////////////////*/ { return p->ticksLastMovement; } // Function

    int Companion::GetMoveSpeed()
    /*/////////////////////////*/ { return p->moveSpeed; } // Function

    float Companion::GetAngle()
    /*///////////////////////*/ { return p->angle; } // Function

    int Companion::GetRadius()
    /*//////////////////////*/ { return p->radius; } // Function

    Directions Companion::GetDirection()
    /*////////////////////////////////*/ { return p->direction; } // Function

    void Companion::IncreaseRadius()
    /*////////////////////////////*/ { p->radius++; } // Function

    Point2 Companion::GetSpawnLocation()
    /*////////////////////////////////*/ { return p->spawnLocation; } // Function

    void Companion::SetAngle(float newAngle)
    /*////////////////////////////////////*/ { p->angle = newAngle; } // Function

    void Companion::UpdateTicksLastMovement()
    /*/////////////////////////////////////*/ { p->ticksLastMovement = SDL_GetTicks(); } // Function

    void Companion::ConsumeStamina(int amountStamina)
    /*/////////////////////////////////////////////*/ { p->stamina -= amountStamina; } // Function

    bool Companion::OutOfStamina()
    /*//////////////////////////*/ { return p->stamina <= 0; } // Function

    void Companion::ReverseDirection()
    /*//////////////////////////////*/ {
        if (p->direction == Directions::Clockwise)
            p->direction = Directions::CounterClockwise;
        else
            p->direction = Directions::Clockwise;
    } // Function

    void Companion::DecreaseRadius()
    /*////////////////////////////*/ { p->radius--; } // Function

    void Companion::SetPreviousCoordinate(Point2 newPreviousCoordinate)
    /*///////////////////////////////////////////////////////////////*/ {
        p->previousCoordinate = newPreviousCoordinate;
    } // Function

    Point2 Companion::GetPreviousCoordinate()
    /*/////////////////////////////////////*/ { return p->previousCoordinate; } // Function
}
//////////////////////////////////////////////////////////////////////