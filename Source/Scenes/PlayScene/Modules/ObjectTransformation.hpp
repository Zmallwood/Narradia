#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectTransformation : public Singleton<ObjectTransformation>
    /*///////////////////////////////////////////////////////////////*/
    {
      private:
        void Update();

        static constexpr int kUpdateSpeed = 2000;
        int ticks_last_update_ = 0;
        
        friend class PlayScene;
    };
}