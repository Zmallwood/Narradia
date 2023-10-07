#include "WorldDraw.hpp"
#include "Camera.hpp"
#include "ConfigurationWorldDraw.hpp"
#include "Engine/Assets/ModelBank.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
#include "Engine/Core/Graphics/Rendering/RendererTiles.hpp"
#include "RenderLoop.hpp"
#include "World/Actors/Mob.hpp"
#include "World/Tile.hpp"
#include "SubDrawers/SubDrawerCompanion.hpp"
#include "SubDrawers/SubDrawerGround.hpp"
#include "SubDrawers/SubDrawerMob.hpp"
#include "SubDrawers/SubDrawerPlayer.hpp"
#include "SubDrawers/SubDrawerSky.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    WorldDraw::WorldDraw()
    /*//////////////////*/
    {
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
        auto allModels = ModelBank::Get()->GetAllModels();
        for (auto &model : allModels)
            RendererModels::Get()->NewModel(model.first);
    }

    void
    WorldDraw::Update()
    /*///////////////*/
    {
        Camera::Get()->Update();
    }

    void
    WorldDraw::Render()
    /*///////////////*/
    {
        Log();
        SubDrawerSky::Get()->DrawSky();
        RenderLoop::data1.clear();
        RenderLoop::data2.clear();
        RendererTiles::Get()->StartBatchDrawing();
        RenderLoop(
            [&]() { SubDrawerGround::Get()->DrawGround(worldViewMode == WorldDrawModes::Play); })();
        RendererTiles::Get()->StopBatchDrawing();
        auto msTime1 = SDL_GetTicks() * 2;
        auto msTime2 = SDL_GetTicks() * 2 + 1000;
        for (auto &d : RenderLoop::data1)
        /*******************************/
        {
            auto modelNameHash = d.first;
            Log();
            RendererModels::Get()->DrawModelsMany(
                modelNameHash, msTime1, d.second.positions, d.second.rotations, d.second.scalings,
                d.second.brightnesses, d.second.colorMods);
        }
        for (auto &d : RenderLoop::data2)
        /*******************************/
        {
            auto modelNameHash = d.first;
            Log();
            RendererModels::Get()->DrawModelsMany(
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
                           RendererModels::Get()->DrawModel(
                               RenderLoop::currTile->GetRoofType(), 0,
                               {RenderLoop::currVertTile.v0.position.x + kTileSize / 2,
                                RenderLoop::currTileAvgElev * kElevAmount,
                                RenderLoop::currVertTile.v0.position.z + kTileSize / 2},
                               0.0f, 0.6f, 1.0f);
                       }
                       Log();
                       SubDrawerMob::Get()->DrawMob();
                       Log();
                       SubDrawerCompanion::Get()->DrawCompanion();
                   })();
        if (worldViewMode == WorldDrawModes::Play)
        /****************************************/
        {
            Log();
            SubDrawerPlayer::Get()->DrawPlayer();
        }
    }

    void
    WorldDraw::EnablePlayMode()
    /*///////////////////////*/
    {
        worldViewMode = WorldDrawModes::Play;
    }

    void
    WorldDraw::EnableMapEditorMode()
    /*////////////////////////////*/
    {
        worldViewMode = WorldDrawModes::Editor;
    }
}