#include "GameEngine.hpp"
#include "Audio.hpp"
#include "GameLoop.hpp"
#include "Graphics/Graphics.hpp"
#include "TextOutBox.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GameEngine::Pimpl {
      public:
        bool running_ = true;
    };

    GameEngine::GameEngine()
        : p(std::make_shared<Pimpl>()) {
    }

    void GameEngine::Run() const {
        Logger::Create();
        srand(static_cast<unsigned int>(time(nullptr)));
        Log();
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            std::cout << "SDL could not initialize! SDL Error: " << std::string(SDL_GetError())
                      << std::endl;
            return;
        }
        Log();
        Graphics::Create();
        Log();
        Audio::Create();
        Log();
        TextOutBox::Create();
        TextOutBox::Get()->Print("Game started.");
        Log();
        GameLoop().Run();
        SDL_Quit();
    }

    void GameEngine::StopGame() {
        p->running_ = false;
    }

    const bool GameEngine::IsRunning() const {
        return p->running_;
    }
}