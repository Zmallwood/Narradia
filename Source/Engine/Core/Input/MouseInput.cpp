//////////////////////////////////////////////////////////////////////
#include "MouseInput.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseInput::Pimpl
    /*///////////////////*/
    {
      public:
        Point2 motionDelta = {0, 0};
        int mouseWheelDelta = 0;
        MouseButton leftButton;
        MouseButton middleButton;
        MouseButton rightButton;
    };

    MouseInput::MouseInput()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    MouseInput::Reset()
    /*///////////////*/
    {
        p->leftButton.Reset();
        p->middleButton.Reset();
        p->rightButton.Reset();
        p->mouseWheelDelta = 0;
        p->motionDelta = {0, 0};
        ResetMouseActions();
    }

    void
    MouseInput::MouseDown(Uint8 mouseButton)
    /*////////////////////////////////////*/
    {
        switch (mouseButton)
        /******************/
        {
        case SDL_BUTTON_LEFT:
            /***************/
            p->leftButton.PressDown();
            break;
        case SDL_BUTTON_MIDDLE:
            /*****************/
            p->middleButton.PressDown();
            break;
        case SDL_BUTTON_RIGHT:
            /****************/
            p->rightButton.PressDown();
            break;
        }
    }

    void
    MouseInput::MouseUp(Uint8 mouseButton)
    /*//////////////////////////////////*/
    {
        switch (mouseButton)
        /******************/
        {
        case SDL_BUTTON_LEFT:
            /***************/
            p->leftButton.Release();
            break;
        case SDL_BUTTON_MIDDLE:
            /*****************/
            p->middleButton.Release();
            break;
        case SDL_BUTTON_RIGHT:
            /****************/
            p->rightButton.Release();
            break;
        }
    }

    void
    MouseInput::MouseWheel(int deltaScroll)
    /*///////////////////////////////////*/
    {
        p->mouseWheelDelta += deltaScroll;
    }

    void
    MouseInput::MouseMove(Point2 motionDelta_)
    /*//////////////////////////////////////*/
    {
        p->motionDelta = motionDelta_;
    }

    int
    MouseInput::MouseWheelDeltaPickResult()
    /*///////////////////////////////////*/
    {
        auto resultMouseWheelDelta = p->mouseWheelDelta;
        p->mouseWheelDelta = 0;
        return resultMouseWheelDelta;
    }

    void
    MouseInput::PerformMouseActions()
    /*/////////////////////////////*/
    {
        Log();
        p->leftButton.PerformMouseAction();
        Log();
        p->middleButton.PerformMouseAction();
        Log();
        p->rightButton.PerformMouseAction();
    }

    void
    MouseInput::ResetMouseActions()
    /*///////////////////////////*/
    {
        p->leftButton.ResetActions();
        p->middleButton.ResetActions();
        p->rightButton.ResetActions();
    }

    Point2
    MouseInput::GetMotionDeltaPickResults()
    /*///////////////////////////////////*/
    {
        auto result = p->motionDelta;
        p->motionDelta = {0, 0};
        return result;
    }

    MouseButton &
    MouseInput::GetLeftButton()
    /*///////////////////////*/
    {
        return p->leftButton;
    }

    MouseButton &
    MouseInput::GetMiddleButton()
    /*/////////////////////////*/
    {
        return p->middleButton;
    }

    MouseButton &
    MouseInput::GetRightButton()
    /*////////////////////////*/
    {
        return p->rightButton;
    }
}
//////////////////////////////////////////////////////////////////////