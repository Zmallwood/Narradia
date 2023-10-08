#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawerPlayer : public Singleton<SubDrawerPlayer> {
      public:
        SubDrawerPlayer();
        void Create();
        void DrawPlayer();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}