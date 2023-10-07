#include "SubDrawerGround.hpp"
#include "World/MapArea.hpp"
#include "Engine/Core/Graphics/Rendering/RendererTiles.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/RenderLoop.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "World/Tile.hpp"
#include "Scenes/PlayScene/Modules/MobTargeting.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawerGround::Pimpl
    /*//////////////////////*/
    {
      public:
        std::map<int, std::map<int, RenderId>> idsTileLayers;
    };

    SubDrawerGround::SubDrawerGround()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    SubDrawerGround::Create()
    /*///////////////////*/
    {
        for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                p->idsTileLayers[x][y] = RendererTiles::Get()->NewTile();
    }

    void
    SubDrawerGround::DrawGround(bool doDrawTerritoryBorders)
    /*//////////////////////////////////////////////////*/
    {
        auto tile = RenderLoop::currTile;
        auto tileCoord = RenderLoop::currTileCoord;
        auto thisTileClaimedByPlayer = RenderLoop::currThisTileClaimedByPlayer;
        auto eastTileClaimedByPlayer = RenderLoop::currEastTileClaimedByPlayer;
        auto southTileClaimedByPlayer = RenderLoop::currSouthTileClaimedByPlayer;
        auto v0 = RenderLoop::currVertTile.v0;
        auto v1 = RenderLoop::currVertTile.v1;
        auto v2 = RenderLoop::currVertTile.v2;
        auto v3 = RenderLoop::currVertTile.v3;
        auto normal00 = RenderLoop::currVertTile.v0.normal;
        auto normal10 = RenderLoop::currVertTile.v1.normal;
        auto normal11 = RenderLoop::currVertTile.v2.normal;
        auto normal01 = RenderLoop::currVertTile.v3.normal;
        auto groundType = tile->GetGroundType();
        auto tileVariation = (tileCoord.x * tileCoord.y) % 3;
        if (groundType == Hash("GroundWater"))
        /************************************/
        {
            auto waterAnimIndex =
                ((SDL_GetTicks() * 2 + tileCoord.x * tileCoord.y * 6) % 2700) / 900;
            if (waterAnimIndex > 0)
                groundType = Hash("GroundWater_" + std::to_string(waterAnimIndex));
            else
                groundType = Hash("GroundWater");
        }
        else if (groundType == Hash("GroundRiver"))
        /*******************************************/
        {
            auto riverAnimIndex =
                ((SDL_GetTicks() * 3 + tileCoord.x * tileCoord.y * 6) % 2700) / 900;
            if (riverAnimIndex > 0)
                groundType = Hash("GroundRiver_" + std::to_string(riverAnimIndex));
            else
                groundType = Hash("GroundRiver");
        }
        else if (groundType == Hash("GroundGrass"))
        /*******************************************/
        {
            groundType = Hash("GroundGrass_" + std::to_string(tileVariation));
        }
        else if (groundType == Hash("GroundRock"))
        /******************************************/
        {
            groundType = Hash("GroundRock_" + std::to_string(tileVariation));
        }
        RendererTiles::Get()->DrawTile(groundType, tile->GetTileRendId());
        auto groundLayerType = tile->GetGroundLayerType();
        if (groundLayerType)
        /******************/
        {
            v0.position.y += 0.03f;
            v1.position.y += 0.03f;
            v2.position.y += 0.03f;
            v3.position.y += 0.03f;
            RendererTiles::Get()->UpdateDrawTile(
                groundLayerType, p->idsTileLayers[tileCoord.x][tileCoord.y], v0, v1, v2, v3,
                normal00, normal10, normal11, normal01);
        }
        auto hoveredTile = TileHovering::Get()->hoveredTile;
        if (hoveredTile.x == tileCoord.x && hoveredTile.y == tileCoord.y)
        /***************************************************************/
        {
            v0.position.y += 0.03f;
            v1.position.y += 0.03f;
            v2.position.y += 0.03f;
            v3.position.y += 0.03f;
            RendererTiles::Get()->UpdateDrawTile(
                Hash("TileHovered"), p->idsTileLayers[tileCoord.x][tileCoord.y], v0, v1, v2, v3,
                normal00, normal10, normal11, normal01);
        }
        auto mob = tile->GetMob().get();
        if (MobTargeting::Get()->GetTargetedMob() == mob && nullptr != mob)
        /****************************************************************/
        {
            v0.position.y += 0.03f;
            v1.position.y += 0.03f;
            v2.position.y += 0.03f;
            v3.position.y += 0.03f;
            RendererTiles::Get()->UpdateDrawTile(
                Hash("TileTargetedMob"), p->idsTileLayers[tileCoord.x][tileCoord.y], v0, v1, v2, v3,
                normal00, normal10, normal11, normal01);
        }
        v0.position.y += 0.03f;
        v1.position.y += 0.03f;
        v2.position.y += 0.03f;
        v3.position.y += 0.03f;
        if (doDrawTerritoryBorders)
        /*************************/
        {
            if (thisTileClaimedByPlayer != eastTileClaimedByPlayer)
            /*****************************************************/
            {
                RendererTiles::Get()->UpdateDrawTile(
                    Hash("PlayerClaimedTileBorderE"), p->idsTileLayers[tileCoord.x][tileCoord.y],
                    v0, v1, v2, v3, normal00, normal10, normal11, normal01);
            }
            if (thisTileClaimedByPlayer != southTileClaimedByPlayer)
            /******************************************************/
            {
                RendererTiles::Get()->UpdateDrawTile(
                    Hash("PlayerClaimedTileBorderS"), p->idsTileLayers[tileCoord.x][tileCoord.y],
                    v0, v1, v2, v3, normal00, normal10, normal11, normal01);
            }
        }
    }
}