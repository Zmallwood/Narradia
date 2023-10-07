#include "Graphics.hpp"
#include "Engine/SdlDeleter.hpp"
#include "GraphicsGl.hpp"
#include "Rendering/Text/TextRenderer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Graphics::Pimpl
    /*/////////////////*/
    {
      public:
        const std::string_view kWindowTitle = "Narradia";
        std::shared_ptr<SDL_Window> window_;
        const Size kConfigResolution = {1600, 900};
#if defined(_DEBUG) || !defined(NDEBUG)
        const Uint32 kWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
#else
        const Uint32 kWindowFlags =
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
    };

    Graphics::Graphics()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        p->window_ = std::shared_ptr<SDL_Window>(
            SDL_CreateWindow(
                p->kWindowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                p->kConfigResolution.width, p->kConfigResolution.height, p->kWindowFlags),
            SdlDeleter());
        if (p->window_ == nullptr)
        /***********************/
        {
            std::cout << "Window could not be created! SDL Error: " << std::string(SDL_GetError())
                      << std::endl;
            return;
        }
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        if (SDL_GL_CreateContext(p->window_.get()) == nullptr)
        /***************************************************/
        {
            std::cout << "OpenGL context could not be created! SDL Error: "
                      << std::string(SDL_GetError()) << std::endl;
            return;
        }
        GraphicsGl::Get();
        TextRenderer::Create();
    }

    SDL_Window *
    Graphics::GetWindow() const
    /*///////////////////////*/
    {
        return p->window_.get();
    }

    void
    Graphics::ClearCanvas() const
    /*/////////////////////////*/
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void
    Graphics::PresentCanvas() const
    /*///////////////////////////*/
    {
        SDL_GL_SwapWindow(p->window_.get());
    }

    void
    Graphics::Cleanup()
    /*//////////////**/
    {
        GraphicsGl::Get()->Cleanup();
    }
}