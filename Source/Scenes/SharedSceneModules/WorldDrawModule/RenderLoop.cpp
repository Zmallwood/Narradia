#include "RenderLoop.hpp"
#include "Camera.hpp"
#include "ConfigWorldDraw.hpp"
#include "Engine/Core/Graphics/Rendering/CameraGl.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/ObjectsCollection.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    RenderLoop::RenderLoop(std::function<void()> action_)
        : action_(action_) {
    }

    void RenderLoop::operator()() {
        auto player_world_area_position = Player::Get()->GetWorldAreaPos();
        auto player_position = Player::Get()->GetPosition();
        auto player_tile_coord = player_position.ToIntPoint();
        auto map_area = World::Get()->GetMapAreaAtZLevel(player_world_area_position.z);
        current_map_area_ = map_area;
        auto camera = Camera::Get();
        auto elev_amount = kElevAmount;
        auto canvas_size = GetCanvasSize();
        auto &perspective_matrix = CameraGl::Get()->GetPerspectiveMatrix();
        auto view_port =
            glm::vec4(0.0f, canvas_size.height / 2, canvas_size.width, canvas_size.height);
        float camera_offset_amount = kTileSize * 10.0f;
        auto angle = std::atan2(
            camera->camera_position_.z - player_tile_coord.y * kTileSize,
            camera->camera_position_.x - player_tile_coord.x * kTileSize);
        auto extra_distance = std::max(
            (float)std::sin((camera->vertical_angle_) * M_PI / 180.0f) * camera_offset_amount,
            0.0f);
        float p_camera_dx = std::cos(angle) * ((camera_offset_amount + extra_distance) *
                                               camera->camera_distance_ / 30.0f);
        float p_camera_dz = std::sin(angle) * ((camera_offset_amount + extra_distance) *
                                               camera->camera_distance_ / 30.0f);
        Point3F usedPos = Point3F{
            player_tile_coord.x * kTileSize + p_camera_dx, 0.0f,
            player_tile_coord.y * kTileSize + p_camera_dz};
        float angle_radians = -(camera->horizontal_angle_) * static_cast<float>(M_PI) / 180.0f;
        auto up = glm::vec3(0.0f, 1.0f, 0.0f);
        auto model_matrix = glm::rotate(glm::mat4(1.0), angle_radians, up);
        auto render_radius = static_cast<int>(kMaxRenderRadius * 0.7f);
        for (auto y = -render_radius; y < render_radius; y++) {
            for (auto x = -render_radius; x < render_radius; x++) {
                current_x_ = x;
                current_y_ = y;
                Point2 tile_coord = {player_tile_coord.x + x, player_tile_coord.y + y};
                current_tile_coordinate_ = tile_coord;
                if (!MapArea::IsInsideMap(tile_coord.x, tile_coord.y))
                    continue;
                auto distance_squared = x * x + y * y;
                if (distance_squared > render_radius * render_radius)
                    continue;
                current_distance_squared_ = distance_squared;
                auto tile = map_area->GetTile(tile_coord.x, tile_coord.y);
                current_tile_ = tile;
                auto elev00 = static_cast<float>(tile->GetElevation());
                auto elev10 = elev00;
                auto elev11 = elev00;
                auto elev01 = elev00;
                auto coord10 = tile_coord.Translate(1, 0);
                auto coord11 = tile_coord.Translate(1, 1);
                auto coord01 = tile_coord.Translate(0, 1);
                current_coord10_ = coord10;
                current_coord11_ = coord11;
                current_coord01_ = coord01;
                auto normal00 = tile->GetNormal();
                auto normal10 = normal00;
                auto normal11 = normal00;
                auto normal01 = normal00;
                if (MapArea::IsInsideMap(coord10))
                    normal10 = map_area->GetTile(coord10)->GetNormal();
                if (MapArea::IsInsideMap(coord11))
                    normal11 = map_area->GetTile(coord11)->GetNormal();
                if (MapArea::IsInsideMap(coord01))
                    normal01 = map_area->GetTile(coord01)->GetNormal();
                if (MapArea::IsInsideMap(coord10))
                    elev10 = map_area->GetTile(coord10)->GetElevation();
                if (MapArea::IsInsideMap(coord11))
                    elev11 = map_area->GetTile(coord11)->GetElevation();
                if (MapArea::IsInsideMap(coord01))
                    elev01 = map_area->GetTile(coord01)->GetElevation();
                current_elev00_ = elev00;
                current_elev10_ = elev10;
                current_elev11_ = elev11;
                current_elev01_ = elev01;
                auto x0 = tile_coord.x * kTileSize;
                auto y0 = elev00 * elev_amount;
                auto z0 = tile_coord.y * kTileSize;
                auto x1 = tile_coord.x * kTileSize + kTileSize;
                auto y1 = elev10 * elev_amount;
                auto z1 = tile_coord.y * kTileSize;
                auto x2 = tile_coord.x * kTileSize + kTileSize;
                auto y2 = elev11 * elev_amount;
                auto z2 = tile_coord.y * kTileSize + kTileSize;
                auto x3 = tile_coord.x * kTileSize;
                auto y3 = elev01 * elev_amount;
                auto z3 = tile_coord.y * kTileSize + kTileSize;
                auto tile_avg_elev = (elev00 + elev10 + elev01 + elev11) / 4.0f;
                current_tile_avg_elev_ = tile_avg_elev;
                if (tile_coord.x == player_tile_coord.x && tile_coord.y == player_tile_coord.y) {
                    player_elev00_ = elev00;
                    player_elev10_ = elev10;
                    player_elev11_ = elev11;
                    player_elev01_ = elev01;
                    player_tile_avg_elev_ = tile_avg_elev;
                }
                glm::vec3 cRel =
                    glm::vec3{(x0 + x2) / 2.0f - usedPos.x, 0, (z0 + z2) / 2.0f - usedPos.z};
                auto screen_coord = glm::project(cRel, model_matrix, perspective_matrix, view_port);
                if (Camera::Get()->camera_distance_ > 10.0f) {
                    if (screen_coord.x <= -0.4f * canvas_size.width ||
                        screen_coord.x >= canvas_size.width * 1.4f)
                        continue;
                }
                VertexTile vertex_tile;
                vertex_tile.v0.position = {x0, y0, z0};
                vertex_tile.v1.position = {x1, y1, z1};
                vertex_tile.v2.position = {x2, y2, z2};
                vertex_tile.v3.position = {x3, y3, z3};
                vertex_tile.v0.normal = normal00;
                vertex_tile.v1.normal = normal10;
                vertex_tile.v2.normal = normal11;
                vertex_tile.v3.normal = normal01;
                current_vertex_tile_ = vertex_tile;
                bool this_tile_claimed_by_player = false;
                bool east_tile_claimed_by_player = false;
                bool south_tile_claimed_by_player = false;
                this_tile_claimed_by_player = Player::Get()->HasClaimedTile(tile_coord);
                if (x < MapArea::GetMapSize().width - 1) {
                    auto east_tile = Point2{tile_coord.x + 1, tile_coord.y};
                    east_tile_claimed_by_player = Player::Get()->HasClaimedTile(east_tile);
                }
                if (y < MapArea::GetMapSize().height - 1) {
                    auto south_tile = Point2{tile_coord.x, tile_coord.y + 1};
                    south_tile_claimed_by_player = Player::Get()->HasClaimedTile(south_tile);
                }
                current_this_tile_claimed_by_player_ = this_tile_claimed_by_player;
                current_east_tile_claimed_by_player_ = east_tile_claimed_by_player;
                current_south_tile_claimed_by_player_ = south_tile_claimed_by_player;
                bool do_draw_objects = true;
                if (do_draw_objects) {
                    for (auto &object_entry : tile->GetObjects().list_) {
                        auto object_type = object_entry->GetObjectType();
                        auto object_scaling = object_entry->GetModelScaling();
                        auto object_rotation = object_entry->GetModelRotation();
                        auto animation_value =
                            (tile_coord.x * tile_coord.y * 10 + SDL_GetTicks() * 2);
                        auto red_variation = object_entry->GetRedVariation();
                        auto green_variation = object_entry->GetGreenVariation();
                        auto blue_variation = object_entry->GetBlueVariation();
                        bool shadow_already_drawn = false;
                        if (object_type) {
                            std::map<int, ModelRenderParameters> *data = nullptr;
                            if ((tile_coord.x + tile_coord.y) % 2 == 0)
                                data = &data1_;
                            else
                                data = &data2_;
                            if (data->count(Hash("Shadow")) == 0)
                                data->insert({Hash("Shadow"), ModelRenderParameters()});
                            if (!shadow_already_drawn) {
                                if (distance_squared < 600) {
                                    if (object_type != Hash("ObjectTallGrass6") &&
                                        (false == ObjectBehaviourList::Get()->HasBehaviourData(
                                                      object_type) ||
                                         ObjectBehaviourList::Get()->GetFlags(object_type) &
                                             (int)ObjectBehaviourFlags::NoShadow == 0)) {
                                        data->at(Hash("Shadow"))
                                            .positions.push_back(
                                                {x0 + kTileSize / 2,
                                                 tile_avg_elev * elev_amount + 0.05f * kTileSize,
                                                 z0 + kTileSize / 2});
                                        data->at(Hash("Shadow")).rotations.push_back(0.0f);
                                        data->at(Hash("Shadow")).scalings.push_back(1.5f);
                                        data->at(Hash("Shadow")).brightnesses.push_back(1.0f);
                                        data->at(Hash("Shadow"))
                                            .colorMods.push_back({1.0f, 1.0f, 1.0f});
                                        shadow_already_drawn = true;
                                    }
                                }
                            }
                            if (data->count(object_type) == 0)
                                data->insert({object_type, ModelRenderParameters()});
                            auto animation_value =
                                (tile_coord.x * tile_coord.y * 10 + SDL_GetTicks() * 2);
                            data->at(object_type)
                                .positions.push_back(
                                    {x0 + kTileSize / 2, tile_avg_elev * elev_amount,
                                     z0 + kTileSize / 2});
                            data->at(object_type).rotations.push_back(object_rotation);
                            data->at(object_type).scalings.push_back(object_scaling);
                            data->at(object_type).brightnesses.push_back(1.0f);
                            data->at(object_type)
                                .colorMods.push_back(
                                    glm::vec3(red_variation, green_variation, blue_variation));
                        }
                    }
                }
                action_();
            }
        }
    }
}