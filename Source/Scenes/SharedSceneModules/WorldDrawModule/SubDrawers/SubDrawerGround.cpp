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
        std::map<int, std::map<int, RenderId>> rendids_tile_layers_;
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
                p->rendids_tile_layers_[x][y] = RendererTiles::Get()->NewTile();
    }

    void
    SubDrawerGround::DrawGround(bool doDrawTerritoryBorders)
    /*//////////////////////////////////////////////////*/
    {
        auto tile = RenderLoop::current_tile_;
        auto tile_coord = RenderLoop::current_tile_coordinate_;
        auto this_tile_claimed_by_player = RenderLoop::current_this_tile_claimed_by_player_;
        auto east_tile_claimed_by_player = RenderLoop::current_east_tile_claimed_by_player_;
        auto south_tile_claimed_by_player = RenderLoop::current_south_tile_claimed_by_player_;
        auto v0 = RenderLoop::current_vertex_tile_.v0;
        auto v1 = RenderLoop::current_vertex_tile_.v1;
        auto v2 = RenderLoop::current_vertex_tile_.v2;
        auto v3 = RenderLoop::current_vertex_tile_.v3;
        auto normal00 = RenderLoop::current_vertex_tile_.v0.normal;
        auto normal10 = RenderLoop::current_vertex_tile_.v1.normal;
        auto normal11 = RenderLoop::current_vertex_tile_.v2.normal;
        auto normal01 = RenderLoop::current_vertex_tile_.v3.normal;
        auto ground_type = tile->GetGroundType();
        auto tile_variation = (tile_coord.x * tile_coord.y) % 3;
        if (ground_type == Hash("GroundWater"))
        /************************************/
        {
            auto water_anim_index =
                ((SDL_GetTicks() * 2 + tile_coord.x * tile_coord.y * 6) % 2700) / 900;
            if (water_anim_index > 0)
                ground_type = Hash("GroundWater_" + std::to_string(water_anim_index));
            else
                ground_type = Hash("GroundWater");
        }
        else if (ground_type == Hash("GroundRiver"))
        /*******************************************/
        {
            auto river_anim_index =
                ((SDL_GetTicks() * 3 + tile_coord.x * tile_coord.y * 6) % 2700) / 900;
            if (river_anim_index > 0)
                ground_type = Hash("GroundRiver_" + std::to_string(river_anim_index));
            else
                ground_type = Hash("GroundRiver");
        }
        else if (ground_type == Hash("GroundGrass"))
        /*******************************************/
        {
            ground_type = Hash("GroundGrass_" + std::to_string(tile_variation));
        }
        else if (ground_type == Hash("GroundRock"))
        /******************************************/
        {
            ground_type = Hash("GroundRock_" + std::to_string(tile_variation));
        }
        RendererTiles::Get()->DrawTile(ground_type, tile->GetTileRendId());
        auto ground_layer_type = tile->GetGroundLayerType();
        if (ground_layer_type)
        /******************/
        {
            v0.position.y += 0.03f;
            v1.position.y += 0.03f;
            v2.position.y += 0.03f;
            v3.position.y += 0.03f;
            RendererTiles::Get()->UpdateDrawTile(
                ground_layer_type, p->rendids_tile_layers_[tile_coord.x][tile_coord.y], v0, v1, v2, v3,
                normal00, normal10, normal11, normal01);
        }
        auto hovered_tile = TileHovering::Get()->hovered_tile_;
        if (hovered_tile.x == tile_coord.x && hovered_tile.y == tile_coord.y)
        /***************************************************************/
        {
            v0.position.y += 0.03f;
            v1.position.y += 0.03f;
            v2.position.y += 0.03f;
            v3.position.y += 0.03f;
            RendererTiles::Get()->UpdateDrawTile(
                Hash("TileHovered"), p->rendids_tile_layers_[tile_coord.x][tile_coord.y], v0, v1, v2, v3,
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
                Hash("TileTargetedMob"), p->rendids_tile_layers_[tile_coord.x][tile_coord.y], v0, v1, v2, v3,
                normal00, normal10, normal11, normal01);
        }
        v0.position.y += 0.03f;
        v1.position.y += 0.03f;
        v2.position.y += 0.03f;
        v3.position.y += 0.03f;
        if (doDrawTerritoryBorders)
        /*************************/
        {
            if (this_tile_claimed_by_player != east_tile_claimed_by_player)
            /*****************************************************/
            {
                RendererTiles::Get()->UpdateDrawTile(
                    Hash("PlayerClaimedTileBorderE"), p->rendids_tile_layers_[tile_coord.x][tile_coord.y],
                    v0, v1, v2, v3, normal00, normal10, normal11, normal01);
            }
            if (this_tile_claimed_by_player != south_tile_claimed_by_player)
            /******************************************************/
            {
                RendererTiles::Get()->UpdateDrawTile(
                    Hash("PlayerClaimedTileBorderS"), p->rendids_tile_layers_[tile_coord.x][tile_coord.y],
                    v0, v1, v2, v3, normal00, normal10, normal11, normal01);
            }
        }
    }
}