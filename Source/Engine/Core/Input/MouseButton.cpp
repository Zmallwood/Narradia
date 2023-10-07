#include "MouseButton.hpp"
#include "MouseActionManager.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MouseButton::Pimpl
    /*////////////////////*/
    {
      public:
        static constexpr int kDefaultClickSpeed = 200;
        bool is_pressed_ = false;
        Uint64 ticks_button_down_ = 0;
        int click_duration_ = 0;
        bool has_been_fired_ = false;
        bool has_been_released_ = false;
        MouseActionManager mouse_action_manager_;
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
        p->is_pressed_ = false;
    }

    void
    MouseButton::PressDown()
    /*////////////////////*/
    {
        p->is_pressed_ = true;
        p->has_been_fired_ = true;
        p->has_been_released_ = false;
        p->click_duration_ = 0;
        p->ticks_button_down_ = SDL_GetTicks();
    }

    void
    MouseButton::Release()
    /*//////////////////*/
    {
        p->is_pressed_ = false;
        p->has_been_released_ = true;
        p->click_duration_ = SDL_GetTicks() - p->ticks_button_down_;
    }

    int
    MouseButton::GetPressDuration() const
    /*/////////////////////////////////*/
    {
        return SDL_GetTicks() - p->ticks_button_down_;
    }

    int
    MouseButton::ClickDurationPickResult()
    /*//////////////////////////////////*/
    {
        auto result = p->click_duration_;
        p->click_duration_ = Pimpl::kDefaultClickSpeed;
        return result;
    }

    int
    MouseButton::ClickDurationPeekResult() const
    /*////////////////////////////////////////*/
    {
        return p->click_duration_;
    }

    void
    MouseButton::PerformMouseAction()
    /*/////////////////////////////*/
    {
        Log();
        if (p->has_been_fired_)
            p->mouse_action_manager_.PerformFiredActions(p->ticks_button_down_, p->is_pressed_, p->has_been_fired_);
        else
            p->mouse_action_manager_.ClearFiredActions();
        Log();
        if (p->has_been_released_)
            p->mouse_action_manager_.PerformReleasedActions(p->ticks_button_down_);
        else
            p->mouse_action_manager_.ClearReleasedActions();
    }

    void
    MouseButton::AddFiredAction(
        std::string_view uniqueName, std::function<void()> action, float priority, int delay,
        bool ensureIsExec)
    /*/////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->has_been_fired_ == true)
            return;
        auto id = Hash(uniqueName);
        if (p->mouse_action_manager_.FiredActionsContains(id))
            return;
        p->mouse_action_manager_.AddFiredAction(
            id, {action, priority, static_cast<Uint64>(delay), ensureIsExec});
    }

    void
    MouseButton::AddReleasedAction(
        std::string_view uniqueName, std::function<void()> action, float priority, int delay,
        bool ensureIsExec)
    /*/////////////////////////////////////////////////////////////////////////////////////*/
    {
        if (p->has_been_released_ == true)
            return;
        auto id = Hash(uniqueName);
        if (p->mouse_action_manager_.ReleasedActionsContains(id))
            return;
        p->mouse_action_manager_.AddReleasedAction(
            id, {action, priority, static_cast<Uint64>(delay), ensureIsExec});
    }

    void
    MouseButton::ResetActions()
    /*///////////////////////*/
    {
        p->mouse_action_manager_.ClearFiredActions();
        p->has_been_fired_ = false;
        p->mouse_action_manager_.ClearReleasedActions();
        p->has_been_released_ = false;
    }

    const int
    MouseButton::GetDefaultClickSpeed()
    /*///////////////////////////////*/
    {
        return Pimpl::kDefaultClickSpeed;
    }
}