#include "SubDrawerCompanion.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/ConfigWorldDraw.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/RenderLoop.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/Camera.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/MovementUtilities.hpp"
#include "World/Tile.hpp"
#include "World/Actors/Companion.hpp"
#include "Engine/Core/Graphics/Rendering/RendererBillboardImages.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawerCompanion::Pimpl
    /*/////////////////////////*/
    {
      public:
        void DrawExclamationMark();
        std::map<int, std::map<int, RenderId>> rendids_companion_bboards_exclamation_marks_;
    };

    SubDrawerCompanion::SubDrawerCompanion()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////*/
    {
    }

    void
    SubDrawerCompanion::Create()
    /*//////////////////////*/
    {
        for (auto x = -kMaxRenderRadius; x < kMaxRenderRadius; x++)
        /*********************************************************/
        {
            for (auto y = -kMaxRenderRadius; y < kMaxRenderRadius; y++)
            /*********************************************************/
            {
                p->rendids_companion_bboards_exclamation_marks_[x][y] =
                    RendererBillboardImages::Get()->NewBillboardImage();
            }
        }
    }

    void
    SubDrawerCompanion::DrawCompanion()
    /*/////////////////////////////*/
    {
        auto tile = RenderLoop::current_tile_;
        auto tile_size = kTileSize;
        auto elev_amount = kElevAmount;
        auto tile_avg_elev = RenderLoop::current_tile_avg_elev_;
        auto v0 = RenderLoop::current_vertex_tile_.v0;
        auto x0 = v0.position.x;
        auto y0 = v0.position.y;
        auto z0 = v0.position.z;
        auto x = RenderLoop::current_x_;
        auto y = RenderLoop::current_y_;
        auto companion = tile->GetCompanion();
        if (companion)
        /************/
        {
            auto minor_movement_offset = GetMinorMovementOffsetForCompanion(companion.get());
            auto elev00 = RenderLoop::current_elev00_;
            auto elev10 = RenderLoop::current_elev10_;
            auto elev11 = RenderLoop::current_elev11_;
            auto elev01 = RenderLoop::current_elev01_;
            auto tile_avg_elev = RenderLoop::player_tile_avg_elev_;
            auto elev_amount = kElevAmount;
            auto tile_size = kTileSize;
            auto elevDx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
            auto elevDy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
            auto companion_elev = RenderLoop::current_tile_avg_elev_ * elev_amount +
                                 minor_movement_offset.x * elevDx * elev_amount +
                                 minor_movement_offset.y * elevDy * elev_amount;
            auto delta_x = RenderLoop::current_tile_coordinate_.x - companion->GetPreviousCoordinate().x;
            auto delta_y = RenderLoop::current_tile_coordinate_.y - companion->GetPreviousCoordinate().y;
            auto abs_delta_x = std::abs(delta_x);
            auto abs_delta_y = std::abs(delta_y);
            auto norm_x = 0;
            auto norm_y = 0;
            if (delta_x)
                norm_x = delta_x / abs_delta_x;
            if (delta_y)
                norm_y = delta_y / abs_delta_y;
            auto facing_angle = -90.0f - std::atan2(norm_y, delta_x) * 180.0f / M_PI;
            RendererModels::Get()->DrawModel(
                Hash("Shadow"), 0,
                {x0 + tile_size / 2 + minor_movement_offset.x * tile_size,
                 companion_elev + 0.05f * tile_size,
                 z0 + tile_size / 2 + minor_movement_offset.y * tile_size},
                0.0f, 0.6f);
            auto anim_value = SDL_GetTicks() * 3;
            RendererModels::Get()->DrawModel(
                Hash("Companion"), anim_value,
                {x0 + tile_size / 2 + minor_movement_offset.x * tile_size, companion_elev,
                 z0 + tile_size / 2 + minor_movement_offset.y * tile_size},
                facing_angle, 0.6f, 1.0f);
            p->DrawExclamationMark();
        }
    }

    void
    SubDrawerCompanion::Pimpl::DrawExclamationMark()
    /*//////////////////////////////////////////*/
    {
        auto tile = RenderLoop::current_tile_;
        auto companion = tile->GetCompanion();
        if (companion->GetStamina() > 0)
            return;
        auto x = RenderLoop::current_x_;
        auto y = RenderLoop::current_y_;
        auto v0 = RenderLoop::current_vertex_tile_.v0;
        auto x0 = v0.position.x;
        auto y0 = v0.position.y;
        auto z0 = v0.position.z;
        auto billboard_y_pos = 3.0f;
        auto billboard_pos = Point3F{
            x0 + kTileSize / 2, RenderLoop::current_tile_avg_elev_ * kElevAmount + billboard_y_pos,
            z0 + kTileSize / 2};
        auto billboard_size = SizeF{0.9f, 0.03f};
        auto exclamation_mark_width = 0.1f;
        auto exclamation_mark_height = 0.1f;
        auto exclamation_mark_pos = billboard_pos.Translate(0.f, kTileSize, 0.f);
        exclamation_mark_pos = Camera::Get()->MoveCloserToCamera(exclamation_mark_pos, kTileSize);
        auto exclamation_mark_bounds = RectangleF{-.5f, -.5f, 1.f, 1.f};
        auto exclamation_mark_size = SizeF{exclamation_mark_width, exclamation_mark_height};
        RendererBillboardImages::Get()->DrawBillboardImage(
            Hash("ExclamationMark"), rendids_companion_bboards_exclamation_marks_[x][y], exclamation_mark_pos,
            exclamation_mark_bounds, exclamation_mark_size);
    }
}