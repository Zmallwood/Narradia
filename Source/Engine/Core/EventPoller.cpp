//////////////////////////////////////////////////////////////////////
#include "EventPoller.hpp"
#include "GameEngine.hpp"
#include "Input/KeyboardInput.hpp"
#include "Input/MouseInput.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void EventPoller::PollEvents() const
    /*////////////////////////////////*/
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        /********************************/
        {
            switch (event.type)
            /*****************/
            {
            case SDL_QUIT:
                /********/
                GameEngine::Get().StopGame();
                break;
            case SDL_KEYDOWN:
                /***********/
                KeyboardInput::Get().KeyDown(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                /*********/
                KeyboardInput::Get().KeyUp(event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                /*******************/
                MouseInput::Get().MouseDown(event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                /*****************/
                MouseInput::Get().MouseUp(event.button.button);
                break;
            case SDL_MOUSEMOTION:
                /***************/
                MouseInput::Get().MouseMove({event.motion.xrel, event.motion.yrel});
                break;
            case SDL_TEXTINPUT:
                /*************/
                KeyboardInput::Get().AppendTextInput(event.text.text);
                break;
            case SDL_MOUSEWHEEL:
                /**************/
                MouseInput::Get().MouseWheel(event.wheel.y);
                break;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////