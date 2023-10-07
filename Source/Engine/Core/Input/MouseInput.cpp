#include "MouseInput.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseInput::Pimpl
    /*///////////////////*/
    {
      public:
        Point2 motion_delta_ = {0, 0};
        int mouse_wheel_delta_ = 0;
        MouseButton left_mouse_button_;
        MouseButton middle_mouse_button_;
        MouseButton right_mouse_button_;
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
        p->left_mouse_button_.Reset();
        p->middle_mouse_button_.Reset();
        p->right_mouse_button_.Reset();
        p->mouse_wheel_delta_ = 0;
        p->motion_delta_ = {0, 0};
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
            p->left_mouse_button_.PressDown();
            break;
        case SDL_BUTTON_MIDDLE:
            /*****************/
            p->middle_mouse_button_.PressDown();
            break;
        case SDL_BUTTON_RIGHT:
            /****************/
            p->right_mouse_button_.PressDown();
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
            p->left_mouse_button_.Release();
            break;
        case SDL_BUTTON_MIDDLE:
            /*****************/
            p->middle_mouse_button_.Release();
            break;
        case SDL_BUTTON_RIGHT:
            /****************/
            p->right_mouse_button_.Release();
            break;
        }
    }

    void
    MouseInput::MouseWheel(int deltaScroll)
    /*///////////////////////////////////*/
    {
        p->mouse_wheel_delta_ += deltaScroll;
    }

    void
    MouseInput::MouseMove(Point2 motionDelta_)
    /*//////////////////////////////////////*/
    {
        p->motion_delta_ = motionDelta_;
    }

    int
    MouseInput::MouseWheelDeltaPickResult()
    /*///////////////////////////////////*/
    {
        auto result_mouse_wheel_delta = p->mouse_wheel_delta_;
        p->mouse_wheel_delta_ = 0;
        return result_mouse_wheel_delta;
    }

    void
    MouseInput::PerformMouseActions()
    /*/////////////////////////////*/
    {
        Log();
        p->left_mouse_button_.PerformMouseAction();
        Log();
        p->middle_mouse_button_.PerformMouseAction();
        Log();
        p->right_mouse_button_.PerformMouseAction();
    }

    void
    MouseInput::ResetMouseActions()
    /*///////////////////////////*/
    {
        p->left_mouse_button_.ResetActions();
        p->middle_mouse_button_.ResetActions();
        p->right_mouse_button_.ResetActions();
    }

    Point2
    MouseInput::GetMotionDeltaPickResults()
    /*///////////////////////////////////*/
    {
        auto result = p->motion_delta_;
        p->motion_delta_ = {0, 0};
        return result;
    }

    MouseButton &
    MouseInput::GetLeftButton()
    /*///////////////////////*/
    {
        return p->left_mouse_button_;
    }

    MouseButton &
    MouseInput::GetMiddleButton()
    /*/////////////////////////*/
    {
        return p->middle_mouse_button_;
    }

    MouseButton &
    MouseInput::GetRightButton()
    /*////////////////////////*/
    {
        return p->right_mouse_button_;
    }
}