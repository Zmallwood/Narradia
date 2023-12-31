#pragma once
#include "Matter/Color.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GraphicsGl : public Singleton<GraphicsGl> {
      public:
        GraphicsGl();
        void Cleanup();
        Color GetFogColorGround();
        Color GetFogColorObjects();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}