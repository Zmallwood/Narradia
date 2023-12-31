#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawerMob : public Singleton<SubDrawerMob> {
      public:
        SubDrawerMob();
        void Create();
        void DrawMob();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}