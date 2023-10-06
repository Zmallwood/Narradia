//////////////////////////////////////////////////////////////////////
#include "GraphicsGl.hpp"
#include "Engine/Assets.hpp"
#include "Rendering/Renderer2DImages.hpp"
#include "Rendering/Renderer2DSolidColors.hpp"
#include "Rendering/RendererBillboardImages.hpp"
#include "Rendering/RendererModels.hpp"
#include "Rendering/RendererTiles.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GraphicsGl::Pimpl
    /*********************/
    {
      public:
        static constexpr bool kCullFace = false;
        const Color fogColorGround = Colors::mildBlue;
        const Color fogColorObjects = Colors::mildBlue;
        const Color clearColor = Colors::black;
    };

    GraphicsGl::GraphicsGl()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        glewExperimental = GL_TRUE;
        auto glewError = glewInit();
        if (glewError != GLEW_OK)
            printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
        if (SDL_GL_SetSwapInterval(0) < 0)
            printf("Warning: Unable to disable VSync! SDL Error: %s\n", SDL_GetError());
        Renderer2DSolidColors::Create();
        Renderer2DImages::Create();
        RendererTiles::Create();
        RendererModels::Create();
        RendererBillboardImages::Create();
        glClearColor(p->clearColor.r, p->clearColor.g, p->clearColor.b, p->clearColor.a);
        if (Pimpl::kCullFace)
        /*******************/
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ImageBank::Get().LoadImages();
        ModelBank::Get().LoadModels();
    }

    void GraphicsGl::Cleanup()
    /*//////////////////////*/
    {
        Renderer2DSolidColors::Get().Cleanup();
        Renderer2DImages::Get().Cleanup();
        RendererTiles::Get().Cleanup();
        RendererModels::Get().Cleanup();
        RendererBillboardImages::Get().Cleanup();
        ImageBank::Get().Cleanup();
    }

    Color GraphicsGl::GetFogColorGround()
    /*/////////////////////////////////*/
    {
        return p->fogColorGround;
    }

    Color GraphicsGl::GetFogColorObjects()
    /*//////////////////////////////////*/
    {
        return p->fogColorObjects;
    }
}
//////////////////////////////////////////////////////////////////////