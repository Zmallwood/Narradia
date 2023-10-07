#pragma once
#include "MouseButton.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseInput : public Singleton<MouseInput>
    /*///////////////////////////////////////////*/
    {
      public:
        MouseInput();
        void Reset();
        void MouseDown(Uint8);
        void MouseUp(Uint8);
        void MouseWheel(int);
        void MouseMove(Point2);
        int MouseWheelDeltaPickResult();
        void PerformMouseActions();
        void ResetMouseActions();
        Point2 GetMotionDeltaPickResults();
        MouseButton &GetLeftButton();
        MouseButton &GetMiddleButton();
        MouseButton &GetRightButton();

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}