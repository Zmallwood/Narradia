#pragma once
#include "Engine/Core/SceneBase.hpp"
#include "MapGenerator.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapGenerationScene : public SceneBase
    /*///////////////////////////////////*/
    {
      public:
        MapGenerationScene();
        void Finalize() override;

      private:
        void Enter() override;
        void UpdateDerived() override;
        void RenderDerived() override;
        RenderId glIdBackgroundImage;
        RenderId glIdGenerationText;
        GenerateSteps currGenerationStep = GenerateSteps::CreateBlank;
        WorldMapGenerate worldMapGenerator;
    };
}