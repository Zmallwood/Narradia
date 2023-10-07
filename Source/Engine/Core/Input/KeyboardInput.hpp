#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class KeyboardInput : public Singleton<KeyboardInput>
    /*/////////////////////////////////////////////////*/
    {
      public:
        KeyboardInput();
        void KeyDown(SDL_Keycode);
        void KeyUp(SDL_Keycode);
        bool AnyKeyHasBeenFired() const;
        bool KeyIsPressed(SDL_Keycode) const;
        bool KeyHasBeenFiredPickResult(SDL_Keycode);
        std::string_view PickTextInput();
        void ResetTextInput();
        void AppendTextInput(std::string_view);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}