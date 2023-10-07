//////////////////////////////////////////////////////////////////////
#pragma once
#include "Engine/Core/RotationDirections.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;

    class Companion
    /*///////////*/
    {
      public:
        Companion(Point2);
        void GiveObject(std::shared_ptr<Object>);
        int GetStamina();
        int GetMaxStamina();
        int GetTicksLastMovement();
        int GetMoveSpeed();
        float GetAngle();
        int GetRadius();
        RotationDirections GetDirection();
        void IncreaseRadius();
        Point2 GetSpawnLocation();
        void SetAngle(float);
        void UpdateTicksLastMovement();
        void ConsumeStamina(int);
        bool OutOfStamina();
        void ReverseDirection();
        void DecreaseRadius();
        void SetPreviousCoordinate(Point2);
        Point2 GetPreviousCoordinate();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}