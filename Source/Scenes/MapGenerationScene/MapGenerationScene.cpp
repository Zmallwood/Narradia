#include "MapGenerationScene.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/WorldDraw.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    MapGenerationScene::MapGenerationScene()
    /*////////////////////////////////*/
    {
        CreateGui();
        rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        rendid_generation_text_ = TextRenderer::Get()->NewString();
    }

    void
    MapGenerationScene::Enter()
    /*/////////////////////*/
    {
        current_generation_step_ = GenerationSteps::CreateBlank;
    }

    void
    MapGenerationScene::UpdateDerived()
    /*/////////////////////////////*/
    {
        Log();
        if (current_generation_step_ == GenerationSteps::CalcNormals)
            World::Get()->CalculateNormals();
        else
            world_map_generator_.DoGenerationStep(current_generation_step_);
        current_generation_step_ = (GenerationSteps)((int)current_generation_step_ + 1);
    }

    void
    MapGenerationScene::RenderDerived()
    /*/////////////////////////////*/
    {
        Log();
        auto rect_background_image = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get()->DrawImage("DefaultSceneBackground", rendid_background_image_, rect_background_image);
        TextRenderer::Get()->DrawString(
            rendid_generation_text_, "Generating terrain...", {0.5f, 0.5f}, Colors::wheat, true);
    }

    void
    MapGenerationScene::Finalize()
    /*////////////////////////*/
    {
        Log();
        if (current_generation_step_ == GenerationSteps::Completed)
            SceneManager::Get()->ChangeScene(SceneNames::Play);
    }
}