#include "WorldDraw.hpp"
#include "Camera.hpp"
#include "ConfigWorldDraw.hpp"
#include "Engine/Assets/ModelBank.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
#include "Engine/Core/Graphics/Rendering/RendererTiles.hpp"
#include "RenderLoop.hpp"
#include "SubDrawers/SubDrawerCompanion.hpp"
#include "SubDrawers/SubDrawerGround.hpp"
#include "SubDrawers/SubDrawerMob.hpp"
#include "SubDrawers/SubDrawerPlayer.hpp"
#include "SubDrawers/SubDrawerSky.hpp"
#include "World/Actors/Mob.hpp"
#include "World/Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    WorldDraw::WorldDraw() {
        Log();
        Camera::Get()->Create();
        Log();
        SubDrawerGround::Get()->Create();
        Log();
        SubDrawerMob::Get()->Create();
        Log();
        SubDrawerCompanion::Get()->Create();
        Log();
        SubDrawerPlayer::Get()->Create();
        Log();
        SubDrawerSky::Get()->Create();
        Log();
        auto all_models = ModelBank::Get()->GetAllModels();
        for (auto &model : all_models)
            RendererModels::Get()->NewModel(model.first);
    }

    void WorldDraw::Update() {
        Camera::Get()->Update();
    }

    void WorldDraw::Render() {
        Log();
        SubDrawerSky::Get()->DrawSky();
        RenderLoop::data1_.clear();
        RenderLoop::data2_.clear();
        RendererTiles::Get()->StartBatchDrawing();
        RenderLoop([&]() {
            SubDrawerGround::Get()->DrawGround(current_world_draw_mode_ == WorldDrawModes::Play);
        })();
        RendererTiles::Get()->StopBatchDrawing();
        auto ms_time1 = SDL_GetTicks() * 2;
        auto ms_time2 = SDL_GetTicks() * 2 + 1000;
        for (auto &d : RenderLoop::data1_) {
            auto model_name_hash = d.first;
            Log();
            RendererModels::Get()->DrawModelsMany(
                model_name_hash, ms_time1, d.second.positions, d.second.rotations,
                d.second.scalings, d.second.brightnesses, d.second.colorMods);
        }
        for (auto &d : RenderLoop::data2_) {
            auto model_name_hash = d.first;
            Log();
            RendererModels::Get()->DrawModelsMany(
                model_name_hash, ms_time2, d.second.positions, d.second.rotations,
                d.second.scalings, d.second.brightnesses, d.second.colorMods);
        }
        RenderLoop([] {
            if (RenderLoop::current_tile_->GetRoofType()) {
                Log();
                RendererModels::Get()->DrawModel(
                    RenderLoop::current_tile_->GetRoofType(), 0,
                    {RenderLoop::current_vertex_tile_.v0.position.x + kTileSize / 2,
                     RenderLoop::current_tile_avg_elev_ * kElevAmount,
                     RenderLoop::current_vertex_tile_.v0.position.z + kTileSize / 2},
                    0.0f, 0.6f, 1.0f);
            }
            Log();
            SubDrawerMob::Get()->DrawMob();
            Log();
            SubDrawerCompanion::Get()->DrawCompanion();
        })();
        if (current_world_draw_mode_ == WorldDrawModes::Play) {
            Log();
            SubDrawerPlayer::Get()->DrawPlayer();
        }
    }

    void WorldDraw::EnablePlayMode() {
        current_world_draw_mode_ = WorldDrawModes::Play;
    }

    void WorldDraw::EnableMapEditorMode() {
        current_world_draw_mode_ = WorldDrawModes::Editor;
    }
}