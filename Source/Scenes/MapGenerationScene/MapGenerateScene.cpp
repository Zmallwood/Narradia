//////////////////////////////////////////////////////////////////////
#include "MapGenerateScene.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/WorldDraw.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    MapGenerateScene::MapGenerateScene()
    /*////////////////////////////////*/
    {
        CreateGui();
        glIdBackgroundImage = Renderer2DImages::Get().NewImage();
        glIdGenerationText = TextRenderer::Get().NewString();
    }

    void
    MapGenerateScene::Enter()
    /*/////////////////////*/
    {
        currGenerationStep = GenerateSteps::CreateBlank;
    }

    void
    MapGenerateScene::UpdateDerived()
    /*/////////////////////////////*/
    {
        Log();
        if (currGenerationStep == GenerateSteps::CalcNormals)
            World::Get().CalculateNormals();
        else
            worldMapGenerator.DoGenerationStep(currGenerationStep);
        currGenerationStep = (GenerateSteps)((int)currGenerationStep + 1);
    }

    void
    MapGenerateScene::RenderDerived()
    /*/////////////////////////////*/
    {
        Log();
        auto rectBack = RectangleF{0.0f, 0.0f, 1.0f, 1.0f};
        Renderer2DImages::Get().DrawImage("DefaultSceneBackground", glIdBackgroundImage, rectBack);
        TextRenderer::Get().DrawString(
            glIdGenerationText, "Generating terrain...", {0.5f, 0.5f}, Colors::wheat, true);
    }

    void
    MapGenerateScene::Finalize()
    /*////////////////////////*/
    {
        Log();
        if (currGenerationStep == GenerateSteps::Completed)
            SceneManager::Get().ChangeScene(SceneNames::Play);
    }
}
//////////////////////////////////////////////////////////////////////