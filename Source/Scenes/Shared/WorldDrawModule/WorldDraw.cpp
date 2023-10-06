//////////////////////////////////////////////////////////////////////
#include "WorldDraw.hpp"
#include "Camera.hpp"
#include "Configuration.hpp"
#include "RenderLoop.hpp"
#include "SubDraws.hpp"
#include "World/Mob.hpp"
#include "World/Tile.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
#include "Engine/Core/Graphics/Rendering/RendererTiles.hpp"
#include "Engine/Assets/ModelBank.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    WorldDraw::WorldDraw()
    /*//////////////////*/
    {
        Log();
        Camera::Get().Create();
        Log();
        SubDrawGround::Get().Create();
        Log();
        SubDrawMob::Get().Create();
        Log();
        SubDrawCompanion::Get().Create();
        Log();
        SubDrawPlayer::Get().Create();
        Log();
        SubDrawSky::Get().Create();
        Log();
        auto allModels = ModelBank::Get().GetAllModels();
        for (auto &model : allModels)
            RendererModels::Get().NewModel(model.first);
    }

    void WorldDraw::Update()
    /*////////////////////*/
    {
        Camera::Get().Update();
    }

    void WorldDraw::Render()
    /*////////////////////*/
    {
        Log();
        SubDrawSky::Get().DrawSky();
        RenderLoop::data1.clear();
        RenderLoop::data2.clear();
        RendererTiles::Get().StartBatchDrawing();
        RenderLoop(
            [&]() { SubDrawGround::Get().DrawGround(worldViewMode == WorldDrawModes::Play); })();
        RendererTiles::Get().StopBatchDrawing();
        auto msTime1 = SDL_GetTicks() * 2;
        auto msTime2 = SDL_GetTicks() * 2 + 1000;
        for (auto &d : RenderLoop::data1)
        /*******************************/
        {
            auto modelNameHash = d.first;
            Log();
            RendererModels::Get().DrawModelsMany(
                modelNameHash, msTime1, d.second.positions, d.second.rotations, d.second.scalings,
                d.second.brightnesses, d.second.colorMods);
        }
        for (auto &d : RenderLoop::data2)
        /*******************************/
        {
            auto modelNameHash = d.first;
            Log();
            RendererModels::Get().DrawModelsMany(
                modelNameHash, msTime2, d.second.positions, d.second.rotations, d.second.scalings,
                d.second.brightnesses, d.second.colorMods);
        }
        RenderLoop([]
                   /********************************************/
                   {
                       if (RenderLoop::currTile->GetRoofType())
                       /**************************************/
                       {
                           Log();
                           RendererModels::Get().DrawModel(
                               RenderLoop::currTile->GetRoofType(), 0,
                               {RenderLoop::currVertTile.v0.position.x + kTileSize / 2,
                                RenderLoop::currTileAvgElev * kElevAmount,
                                RenderLoop::currVertTile.v0.position.z + kTileSize / 2},
                               0.0f, 0.6f, 1.0f);
                       }
                       Log();
                       SubDrawMob::Get().DrawMob();
                       Log();
                       SubDrawCompanion::Get().DrawCompanion();
                   })();
        if (worldViewMode == WorldDrawModes::Play)
        /****************************************/
        {
            Log();
            SubDrawPlayer::Get().DrawPlayer();
        }
    }

    void WorldDraw::EnablePlayMode()
    /*////////////////////////////*/
    {
        worldViewMode = WorldDrawModes::Play;
    }

    void WorldDraw::EnableMapEditorMode()
    /*/////////////////////////////////*/
    {
        worldViewMode = WorldDrawModes::Editor;
    }
}
//////////////////////////////////////////////////////////////////////