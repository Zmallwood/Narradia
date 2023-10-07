#include "KeyboardInput.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class KeyboardInput::Pimpl
    /*//////////////////////*/
    {
      public:
        std::string text_input_;
        std::set<SDL_Keycode> pressed_keys_;
        std::set<SDL_Keycode> fired_keys_;
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
        if (p->pressed_keys_.count(key) == 0)
            p->fired_keys_.insert(key);
        p->pressed_keys_.insert(key);
    }

    void
    KeyboardInput::KeyUp(SDL_Keycode key)
    /*/////////////////////////////////*/
    {
        p->pressed_keys_.erase(key);
    }

    bool
    KeyboardInput::AnyKeyHasBeenFired() const
    /*/////////////////////////////////////*/
    {
        return p->fired_keys_.size() > 0;
    }

    bool
    KeyboardInput::KeyIsPressed(SDL_Keycode key) const
    /*//////////////////////////////////////////////*/
    {
        return p->pressed_keys_.count(key) > 0;
    }

    bool
    KeyboardInput::KeyHasBeenFiredPickResult(SDL_Keycode key)
    /*/////////////////////////////////////////////////////*/
    {
        auto result = p->fired_keys_.count(key) > 0;
        p->fired_keys_.erase(key);
        return result;
    }

    std::string_view
    KeyboardInput::PickTextInput()
    /*//////////////////////////*/
    {
        auto result = p->text_input_;
        p->text_input_ = "";
        return result;
    }

    void
    KeyboardInput::ResetTextInput()
    /*///////////////////////////*/
    {
        p->text_input_ = "";
    }

    void
    KeyboardInput::AppendTextInput(std::string_view toAppend)
    /*/////////////////////////////////////////////////////*/
    {
        p->text_input_.append(toAppend);
    }
}