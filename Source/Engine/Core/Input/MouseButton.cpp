//////////////////////////////////////////////////////////////////////
#include "MouseButton.hpp"
#include "MouseActionManager.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseButton::Pimpl
    /*////////////////////*/
    {
      public:
        static constexpr int defaultClickSpeed = 200;
        bool isPressed = false;
        Uint64 ticksButtonDown = 0;
        int clickDuration = 0;
        bool hasBeenFired = false;
        bool hasBeenReleased = false;
        MouseActionManager actionMngr;
    };

    MouseButton::MouseButton()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    MouseButton::Reset()
    /*////////////////*/
    {
        p->isPressed = false;
    }

    void
    MouseButton::PressDown()
    /*////////////////////*/
    {
        p->isPressed = true;
        p->hasBeenFired = true;
        p->hasBeenReleased = false;
        p->clickDuration = 0;
        p->ticksButtonDown = SDL_GetTicks();
    }

    void
    MouseButton::Release()
    /*//////////////////*/
    {
        p->isPressed = false;
        p->hasBeenReleased = true;
        p->clickDuration = SDL_GetTicks() - p->ticksButtonDown;
    }

    int
    MouseButton::GetPressDuration() const
    /*/////////////////////////////////*/
    {
        return SDL_GetTicks() - p->ticksButtonDown;
    }

    int
    MouseButton::ClickDurationPickResult()
    /*//////////////////////////////////*/
    {
        auto result = p->clickDuration;
        p->clickDuration = Pimpl::defaultClickSpeed;
        return result;
    }

    int
    MouseButton::ClickDurationPeekResult() const
    /*////////////////////////////////////////*/
    {
        return p->clickDuration;
    }

    void
    MouseButton::PerformMouseAction()
    /*/////////////////////////////*/
    {
        Log();
        if (p->hasBeenFired)
            p->actionMngr.PerformFiredActions(p->ticksButtonDown, p->isPressed, p->hasBeenFired);
        else
            p->actionMngr.ClearFiredActions();
        Log();
        if (p->hasBeenReleased)
            p->actionMngr.PerformReleasedActions(p->ticksButtonDown);
        else
            p->actionMngr.ClearReleasedActions();
    }

    void
    MouseButton::AddFiredAction(
        std::string_view uniqueName, std::function<void()> action, float priority, int delay,
        bool ensureIsExec)
    /*/////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->hasBeenFired == true)
            return;
        auto id = Hash(uniqueName);
        if (p->actionMngr.FiredActionsContains(id))
            return;
        p->actionMngr.AddFiredAction(
            id, {action, priority, static_cast<Uint64>(delay), ensureIsExec});
    }

    void
    MouseButton::AddReleasedAction(
        std::string_view uniqueName, std::function<void()> action, float priority, int delay,
        bool ensureIsExec)
    /*/////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->hasBeenReleased == true)
            return;
        auto id = Hash(uniqueName);
        if (p->actionMngr.ReleasedActionsContains(id))
            return;
        p->actionMngr.AddReleasedAction(
            id, {action, priority, static_cast<Uint64>(delay), ensureIsExec});
    }

    void
    MouseButton::ResetActions()
    /*///////////////////////*/
    {
        p->actionMngr.ClearFiredActions();
        p->hasBeenFired = false;
        p->actionMngr.ClearReleasedActions();
        p->hasBeenReleased = false;
    }

    const int
    MouseButton::GetDefaultClickSpeed()
    /*///////////////////////////////*/
    {
        return Pimpl::defaultClickSpeed;
    }
}