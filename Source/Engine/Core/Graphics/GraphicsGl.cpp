#include "GraphicsGl.hpp"
#include "Engine/Assets/ImageBank.hpp"
#include "Engine/Assets/ModelBank.hpp"
#include "Rendering/Renderer2DImages.hpp"
#include "Rendering/Renderer2DSolidColors.hpp"
#include "Rendering/RendererBillboardImages.hpp"
#include "Rendering/RendererModels.hpp"
#include "Rendering/RendererTiles.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GraphicsGl::Pimpl {
      public:
        static constexpr bool kCullFace = false;
        const Color kFogColorGround = Colors::mildBlue;
        const Color kFogColorObjects = Colors::mildBlue;
        const Color kClearColor = Colors::black;
    };

    GraphicsGl::GraphicsGl()
        : p(std::make_shared<Pimpl>()) {
        glewExperimental = GL_TRUE;
        auto glew_error = glewInit();
        if (glew_error != GLEW_OK)
            printf("Error initializing GLEW! %s\n", glewGetErrorString(glew_error));
        if (SDL_GL_SetSwapInterval(0) < 0)
            printf("Warning: Unable to disable VSync! SDL Error: %s\n", SDL_GetError());
        Renderer2DSolidColors::Create();
        Renderer2DImages::Create();
        RendererTiles::Create();
        RendererModels::Create();
        RendererBillboardImages::Create();
        glClearColor(p->kClearColor.r, p->kClearColor.g, p->kClearColor.b, p->kClearColor.a);
        if (Pimpl::kCullFace) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ImageBank::Get()->LoadImages();
        ModelBank::Get()->LoadModels();
    }

    void GraphicsGl::Cleanup() {
        Renderer2DSolidColors::Get()->Cleanup();
        Renderer2DImages::Get()->Cleanup();
        RendererTiles::Get()->Cleanup();
        RendererModels::Get()->Cleanup();
        RendererBillboardImages::Get()->Cleanup();
        ImageBank::Get()->Cleanup();
    }

    Color GraphicsGl::GetFogColorGround() {
        return p->kFogColorGround;
    }

    Color GraphicsGl::GetFogColorObjects() {
        return p->kFogColorObjects;
    }
}