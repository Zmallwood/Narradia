#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectTransformation : public Singleton<ObjectTransformation>
    /*///////////////////////////////////////////////////////////////*/
    {
      private:
        void Update();
        static constexpr int k_update_speed = 2000;
        int ticks_last_update = 0;
        friend class PlayScene;
    };
}