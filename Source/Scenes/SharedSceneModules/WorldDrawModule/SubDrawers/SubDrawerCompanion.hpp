#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawerCompanion : public Singleton<SubDrawerCompanion> {
      public:
        SubDrawerCompanion();
        void Create();
        void DrawCompanion();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}