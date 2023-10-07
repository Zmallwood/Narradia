//////////////////////////////////////////////////////////////////////
#include "KeyboardInput.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class KeyboardInput::Pimpl
    /*//////////////////////*/
    {
      public:
        std::string textInput;
        std::set<SDL_Keycode> pressedKeys;
        std::set<SDL_Keycode> firedKeys;
    };

    KeyboardInput::KeyboardInput()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    KeyboardInput::KeyDown(SDL_Keycode key)
    /*///////////////////////////////////*/
    {
        if (p->pressedKeys.count(key) == 0)
            p->firedKeys.insert(key);
        p->pressedKeys.insert(key);
    }

    void
    KeyboardInput::KeyUp(SDL_Keycode key)
    /*/////////////////////////////////*/
    {
        p->pressedKeys.erase(key);
    }

    bool
    KeyboardInput::AnyKeyHasBeenFired() const
    /*/////////////////////////////////////*/
    {
        return p->firedKeys.size() > 0;
    }

    bool
    KeyboardInput::KeyIsPressed(SDL_Keycode key) const
    /*//////////////////////////////////////////////*/
    {
        return p->pressedKeys.count(key) > 0;
    }

    bool
    KeyboardInput::KeyHasBeenFiredPickResult(SDL_Keycode key)
    /*/////////////////////////////////////////////////////*/
    {
        auto result = p->firedKeys.count(key) > 0;
        p->firedKeys.erase(key);
        return result;
    }

    std::string_view
    KeyboardInput::PickTextInput()
    /*//////////////////////////*/
    {
        auto result = p->textInput;
        p->textInput = "";
        return result;
    }

    void
    KeyboardInput::ResetTextInput()
    /*///////////////////////////*/
    {
        p->textInput = "";
    }

    void
    KeyboardInput::AppendTextInput(std::string_view toAppend)
    /*/////////////////////////////////////////////////////*/
    {
        p->textInput.append(toAppend);
    }
}