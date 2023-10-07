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
}