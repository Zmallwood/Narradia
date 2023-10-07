#include "ActionRepeat.hpp"
#include "Engine/Core/Input/KeyboardInput.hpp"
#include "Engine/Core/TextOutBox.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ActionRepeat::Pimpl
    /*/////////////////////*/
    {
      public:
        std::function<void()> repeating_action_ = nullptr;
    };

    ActionRepeat::ActionRepeat()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    ActionRepeat::Update()
    /*//////////////////*/
    {
        if (KeyboardInput::Get()->KeyHasBeenFiredPickResult(SDLK_r))
            RepeatLastAction();
    }

    void
    ActionRepeat::RepeatLastAction()
    /*////////////////////////////*/
    {
        if (p->repeating_action_ == nullptr)
        /********************************/
        {
            TextOutBox::Get()->Print("There is no previous action to repeat.");
        }
        else
        /**/
        {
            TextOutBox::Get()->Print("Repeating action.");
            Log();
            p->repeating_action_();
        }
    }

    void
    ActionRepeat::SetRepeatingAction(std::function<void()> action)
    /*//////////////////////////////////////////////////////////*/
    {
        p->repeating_action_ = action;
    }
}