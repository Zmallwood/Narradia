//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawerGround : public Singleton<SubDrawerGround>
    /*/////////////////////////////////////////////////*/
    {
      public:
        SubDrawerGround();
        void Create();
        void DrawGround(bool = true);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class SubDrawerMob : public Singleton<SubDrawerMob>
    /*///////////////////////////////////////////*/
    {
      public:
        SubDrawerMob();
        void Create();
        void DrawMob();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };

    class SubDrawerPlayer : public Singleton<SubDrawerPlayer>
    /*/////////////////////////////////////////////////*/
    {
      public:
        SubDrawerPlayer();
        void Create();
        void DrawPlayer();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
    
    class SubDrawerSky : public Singleton<SubDrawerSky>
    /*///////////////////////////////////////////*/
    {
      public:
        void DrawSky();
    };
}