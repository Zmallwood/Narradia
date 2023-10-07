//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Mob;

    class MobTargeting : public Singleton<MobTargeting>
    /*///////////////////////////////////////////////*/
    {
      public:
        MobTargeting();
        void Update();
        Mob *GetTargetedMob();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}