#include "TileHovering.hpp"
#include "Engine/Core/Graphics/Rendering/CameraGl.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
#include "WorldDrawModule/Camera.hpp"
#include "WorldDrawModule/ConfigWorldDraw.hpp"
#include "WorldDrawModule/WorldDraw.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    TileHovering::TileHovering() {
        rendid_hovered_object_text_ = TextRenderer::Get()->NewString();
    }

    void TileHovering::Update() {
        if (Camera::Get()->camera_distance_ == 2.0f) {
            hovered_tile_ = {-1, -1};
            return;
        }
        hovered_object_hash_ = 0;
        auto &view_matrix = CameraGl::Get()->GetViewMatrix();
        auto &perspective_matrix = CameraGl::Get()->GetPerspectiveMatrix();
        auto mouse_position_f = GetMousePositionF();
        auto canvas_size = GetCanvasSize();
        auto player_x = Player::Get()->GetPosition().x;
        auto player_y = Player::Get()->GetPosition().y;
        auto tile_size = kTileSize;
        auto elev_amount = kElevAmount;
        auto player_world_area_position = Player::Get()->GetWorldAreaPos();
        auto *map_area = World::Get()->GetMapAreaAtZLevel(player_world_area_position.z);
        auto mouse_world_near_plane = glm::unProject(
            glm::vec3(
                mouse_position_f.x * canvas_size.width,
                (1.0f - mouse_position_f.y) * canvas_size.height, 0.0f),
            view_matrix, perspective_matrix,
            glm::ivec4(0, 0, canvas_size.width, canvas_size.height));
        auto mouse_world_far_plane = glm::unProject(
            glm::vec3(
                mouse_position_f.x * canvas_size.width,
                (1.0f - mouse_position_f.y) * canvas_size.height, 1.0f),
            view_matrix, perspective_matrix,
            glm::ivec4(0, 0, canvas_size.width, canvas_size.height));
        auto columns_count = 111;
        auto rows_count = 111;
        auto player_x_major = static_cast<int>(player_x);
        auto player_y_major = static_cast<int>(player_y);
        bool tile_found = false;
        auto fn_iteration = [&](int x, int y) -> bool {
            auto map_x = player_x_major + x;
            auto map_y = player_y_major + y;
            if (!MapArea::IsInsideMap({map_x, map_y}))
                return false;
            auto tile_coord = Point2{map_x, map_y};
            auto tile = map_area->GetTile(tile_coord);
            auto elev00 = static_cast<float>(tile->GetElevation());
            auto elev10 = elev00;
            auto elev11 = elev00;
            auto elev01 = elev00;
            auto tile_avg_elev = (elev00 + elev10 + elev01 + elev11) / 4.0f;
            auto coord10 = tile_coord.Translate(1, 0);
            auto coord11 = tile_coord.Translate(1, 1);
            auto coord01 = tile_coord.Translate(0, 1);
            if (MapArea::IsInsideMap(coord10))
                elev10 = map_area->GetTile(coord10)->GetElevation();
            if (MapArea::IsInsideMap(coord11))
                elev11 = map_area->GetTile(coord11)->GetElevation();
            if (MapArea::IsInsideMap(coord01))
                elev01 = map_area->GetTile(coord01)->GetElevation();
            auto x0 = tile_coord.x * tile_size;
            auto y0 = elev00 * elev_amount;
            auto z0 = tile_coord.y * tile_size;
            auto x2 = tile_coord.x * tile_size + tile_size;
            auto y2 = elev11 * elev_amount;
            auto z2 = tile_coord.y * tile_size + tile_size;
            auto center = glm::vec3{(x0 + x2) / 2, (y0 + y2) / 2, (z0 + z2) / 2};
            auto closest_point =
                glm::closestPointOnLine(center, mouse_world_near_plane, mouse_world_far_plane);
            if (glm::distance(center, closest_point) < tile_size / 2) {
                hovered_tile_ = {map_x, map_y};
                if (tile->GetObjectsCount() > 0)
                    hovered_object_hash_ = tile->GetObjectAt(0)->GetObjectType();
                tile_found = true;
                return true;
            }
            return false;
        };
        for (int y = -(rows_count - 1) / 2; y < (rows_count - 1) / 2 && !tile_found; y++) {
            for (int x = -(columns_count - 1) / 2; x < (columns_count - 1) / 2 && !tile_found;
                 x++) {
                if (fn_iteration(x, y))
                    return;
            }
        }
    }

    void TileHovering::Render() {
        if (hovered_object_hash_ == 0)
            return;
        if (false == ObjectBehaviourList::Get()->HasBehaviourData(hovered_object_hash_))
            return;
        TextRenderer::Get()->DrawString(
            rendid_hovered_object_text_,
            ObjectBehaviourList::Get()->GetLabel(hovered_object_hash_).data(),
            GetMousePositionF().Translate(0.02f, 0.01f));
    }
}