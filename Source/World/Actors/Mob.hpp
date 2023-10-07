//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Mob
    /*/////*/
    {
      public:
        Mob(int, int, int);
        int GetTicksLastMovement();
        int GetMoveSpeed();
        void ClearDestination();
        bool HasNoDestination();
        void SetDestination(Point2);
        Point2 GetDestination();
        Point2 GetPreviousCoordinate();
        int GetMobType();
        void UpdateTicksLastMovement();
        void SetPreviousCoordinate(Point2);
        float GetDistanceAboveGround();
        void SetDistanceAboveGround(float);
        void SetMoveSpeed(int);
        int GetSpawnX();
        int GetSpawnY();
        int GetTicksLastHitRecieved();
        void StartAggroPlayer();
        bool IsAggroingPlayer();
        int GetTicksLastAttack();
        int GetAttackSpeed();
        void UpdateTicksLastAttack();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}