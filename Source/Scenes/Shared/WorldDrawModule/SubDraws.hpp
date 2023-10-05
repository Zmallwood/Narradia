//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawCompanion : public Singleton<SubDrawCompanion>
    /*///////////////////////////////////////////////////////*/ {
      public:
        SubDrawCompanion();
        void Create();
        void DrawCompanion();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class SubDrawGround : public Singleton<SubDrawGround>
    /*/////////////////////////////////////////////////*/ {
      public:
        SubDrawGround();
        void Create();
        void DrawGround(bool = true);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class SubDrawMob : public Singleton<SubDrawMob>
    /*///////////////////////////////////////////*/ {
      public:
        SubDrawMob();
        void Create();
        void DrawMob();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class SubDrawPlayer : public Singleton<SubDrawPlayer>
    /*/////////////////////////////////////////////////*/ {
      public:
        SubDrawPlayer();
        void Create();
        void DrawPlayer();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
    
    class SubDrawSky : public Singleton<SubDrawSky>
    /*///////////////////////////////////////////*/ {
      public:
        void DrawSky();
    };
}
//////////////////////////////////////////////////////////////////////