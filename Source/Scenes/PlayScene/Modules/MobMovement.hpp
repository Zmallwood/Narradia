#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MobMovement : public Singleton<MobMovement> {
      public:
        void Update();
    };
}