#pragma once
#include "Engine/Core/SceneBase.hpp"
#include "MapGenerator.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapGenerationScene : public SceneBase {
      public:
        MapGenerationScene();
        void Finalize() override;

      private:
        void Enter() override;
        void UpdateDerived() override;
        void RenderDerived() override;
        RenderId rendid_background_image_;
        RenderId rendid_generation_text_;
        GenerationSteps current_generation_step_ = GenerationSteps::CreateBlank;
        WorldMapGenerator world_map_generator_;
    };
}