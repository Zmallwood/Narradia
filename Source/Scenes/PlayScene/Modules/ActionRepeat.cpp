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
        std::function<void()> repeatingAction = nullptr;
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
        if (p->repeatingAction == nullptr)
        /********************************/
        {
            TextOutBox::Get()->Print("There is no previous action to repeat.");
        }
        else
        /**/
        {
            TextOutBox::Get()->Print("Repeating action.");
            Log();
            p->repeatingAction();
        }
    }

    void
    ActionRepeat::SetRepeatingAction(std::function<void()> action)
    /*//////////////////////////////////////////////////////////*/
    {
        p->repeatingAction = action;
    }
}