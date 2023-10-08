#include "SubDrawerMob.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/Camera.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/ConfigWorldDraw.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/MovementUtilities.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/RenderLoop.hpp"
#include "World/Actors/Mob.hpp"
#include "World/Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawerMob::Pimpl {
      public:
        void InitializeIds();
        int GetAnimationValue();
        void DrawShadow();
        void DrawMobModel();
        void IfCaseDrawMobLabel();

        std::map<int, std::map<int, RenderId>> rendids_billboard_texts_;
        const int kShowHitEffectDuration = 600;
    };

    SubDrawerMob::SubDrawerMob()
        : p(std::make_shared<Pimpl>()) {
    }

    void SubDrawerMob::Create() {
        p->InitializeIds();
    }

    void SubDrawerMob::DrawMob() {
        auto tile = RenderLoop::current_tile_;
        auto mob = tile->GetMob();
        if (nullptr == mob)
            return;
        p->DrawShadow();
        p->DrawMobModel();
        p->IfCaseDrawMobLabel();
    }

    void SubDrawerMob::Pimpl::InitializeIds() {
        for (auto x = -kMaxRenderRadius; x < kMaxRenderRadius; x++)
            for (auto y = -kMaxRenderRadius; y < kMaxRenderRadius; y++)
                rendids_billboard_texts_[x][y] = TextRenderer::Get()->NewBillboardString();
    }

    int SubDrawerMob::Pimpl::GetAnimationValue() {
        auto tile_coord = RenderLoop::current_tile_coordinate_;
        return tile_coord.x * tile_coord.y * 10 + SDL_GetTicks();
    }

    void SubDrawerMob::Pimpl::DrawShadow() {
        auto v0 = RenderLoop::current_vertex_tile_.v0;
        auto x0 = v0.position.x;
        auto z0 = v0.position.z;
        auto mob = RenderLoop::current_tile_->GetMob();
        auto minor_movement_offset = GetMinorMovementOffsetForMob(mob.get());
        auto elev00 = RenderLoop::current_elev00_;
        auto elev10 = RenderLoop::current_elev10_;
        auto elev11 = RenderLoop::current_elev11_;
        auto elev01 = RenderLoop::current_elev01_;
        auto elev_dx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
        auto elev_dy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
        auto mob_elev = RenderLoop::current_tile_avg_elev_ * kElevAmount +
                        minor_movement_offset.x * elev_dx * kElevAmount +
                        minor_movement_offset.y * elev_dy * kElevAmount;
        RendererModels::Get()->DrawModel(
            Hash("Shadow"), 0,
            {x0 + kTileSize / 2 + minor_movement_offset.x * kTileSize, mob_elev + 0.05f * kTileSize,
             z0 + kTileSize / 2 + minor_movement_offset.y * kTileSize},
            0.0f, 0.6f);
    }

    void SubDrawerMob::Pimpl::DrawMobModel() {
        auto v0 = RenderLoop::current_vertex_tile_.v0;
        auto x0 = v0.position.x;
        auto z0 = v0.position.z;
        auto mob = RenderLoop::current_tile_->GetMob();
        auto anim_value = GetAnimationValue();
        auto minor_movement_offset = GetMinorMovementOffsetForMob(mob.get());
        auto delta_x = RenderLoop::current_tile_coordinate_.x - mob->GetPreviousCoordinate().x;
        auto delta_y = RenderLoop::current_tile_coordinate_.y - mob->GetPreviousCoordinate().y;
        auto abs_delta_x = std::abs(delta_x);
        auto abs_delta_y = std::abs(delta_y);
        auto norm_x = 0;
        auto norm_y = 0;
        if (delta_x)
            norm_x = delta_x / abs_delta_x;
        if (delta_y)
            norm_y = delta_y / abs_delta_y;
        auto facing_angle = -90.0f - std::atan2(norm_y, delta_x) * 180.0f / M_PI;
        auto elev00 = RenderLoop::current_elev00_;
        auto elev10 = RenderLoop::current_elev10_;
        auto elev11 = RenderLoop::current_elev11_;
        auto elev01 = RenderLoop::current_elev01_;
        auto elev_dx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
        auto elev_dy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
        auto mob_elev = RenderLoop::current_tile_avg_elev_ * kElevAmount +
                        minor_movement_offset.x * elev_dx * kElevAmount +
                        minor_movement_offset.y * elev_dy * kElevAmount;
        RendererModels::Get()->DrawModel(
            mob->GetMobType(), anim_value,
            {x0 + kTileSize / 2 + minor_movement_offset.x * kTileSize,
             mob_elev + mob->GetDistanceAboveGround(),
             z0 + kTileSize / 2 + minor_movement_offset.y * kTileSize},
            facing_angle, 0.5f, 1.0f);
        if (SDL_GetTicks() < mob->GetTicksLastHitRecieved() + kShowHitEffectDuration) {
            RendererModels::Get()->DrawModel(
                Hash("HitEffect"), anim_value,
                {x0 + kTileSize / 2 + minor_movement_offset.x * kTileSize,
                 mob_elev + mob->GetDistanceAboveGround(),
                 z0 + kTileSize / 2 + minor_movement_offset.y * kTileSize},
                facing_angle, 0.5f, 1.0f);
        }
    }

    void SubDrawerMob::Pimpl::IfCaseDrawMobLabel() {
        auto v0 = RenderLoop::current_vertex_tile_.v0;
        auto x0 = v0.position.x;
        auto z0 = v0.position.z;
        auto mob = RenderLoop::current_tile_->GetMob();
        auto minor_movement_offset = GetMinorMovementOffsetForMob(mob.get());
        auto billboard_y_pos = 3.f;
        auto billboard_pos = Point3F{
            x0 + kTileSize / 2 + minor_movement_offset.x * kTileSize,
            RenderLoop::current_tile_avg_elev_ * kElevAmount + mob->GetDistanceAboveGround() +
                billboard_y_pos,
            z0 + kTileSize / 2 + minor_movement_offset.y * kTileSize};
        auto billboard_text_pos = Camera::Get()->MoveCloserToCamera(billboard_pos, 0.01f);
        auto billboard_size = SizeF{0.9f, 0.03f};
        if (TileHovering::Get()->hovered_tile_ == RenderLoop::current_tile_coordinate_) {
            TextRenderer::Get()->DrawBillboardString(
                rendids_billboard_texts_[RenderLoop::current_x_][RenderLoop::current_y_],
                "Mob Lvl. 1", billboard_text_pos, billboard_size);
        }
    }
}