#include "SubDrawerPlayer.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
#include "World/Actors/Player.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/ConfigurationWorldDraw.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/Camera.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/RenderLoop.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawerPlayer::Pimpl
    /*//////////////////////*/
    {
      public:
        void DrawLabel();
        float GetPlayerElevation();

        RenderId rendid_billboard_text_player_name_;
    };

    SubDrawerPlayer::SubDrawerPlayer()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    SubDrawerPlayer::Create()
    /*///////////////////*/
    {
        p->rendid_billboard_text_player_name_ = TextRenderer::Get()->NewBillboardString();
    }

    void
    SubDrawerPlayer::DrawPlayer()
    /*///////////////////////*/
    {
        auto pos = Player::Get()->GetSpaceCoord().Translate(0.0f, p->GetPlayerElevation(), 0.0f);
        if (Player::Get()->mounted)
        /************************/
        {
            RendererModels::Get()->DrawModel(
                Hash("Mount1"), 0, pos, Player::Get()->GetFacingAngle(), 0.8f);
        }
        float anim_tile;
        if (Player::Get()->data.movement.isMoving)
            anim_tile = SDL_GetTicks() * 3;
        else
            anim_tile = 0.0f;
        RendererModels::Get()->DrawModel(
            Hash("Shadow"), 0,
            Player::Get()->GetSpaceCoord().Translate(
                0.0f,
                Player::Get()->data.movement.jumpHeight + p->GetPlayerElevation() +
                    0.05f * kTileSize,
                0.0f),
            0.0f, 0.6f);
        if (Camera::Get()->cameraDistance > 2.0f)
        /**************************************/
        {
            if (Player::Get()->playerBuild == PlayerBuilds::Sword)
            /***************************************************/
            {
                RendererModels::Get()->DrawModel(
                    Hash("Player2Sword"), anim_tile, pos, Player::Get()->GetFacingAngle(), 0.6f);
            }
            else if (Player::Get()->playerBuild == PlayerBuilds::Club)
            /*******************************************************/
            {
                RendererModels::Get()->DrawModel(
                    Hash("Player2Club"), anim_tile, pos, Player::Get()->GetFacingAngle(), 0.6f);
            }
            else
            /**/
            {
                RendererModels::Get()->DrawModel(
                    Hash("Player2"), anim_tile, pos, Player::Get()->GetFacingAngle(), 0.6f);
            }
        }
        else
        /**/
        {
            auto hands_position = pos;
            hands_position.y = Camera::Get()->cameraPosition.y - 0.5f * kTileSize;
            RendererModels::Get()->DrawModel(
                Hash("PlayerRightHand"), anim_tile, hands_position, Player::Get()->GetFacingAngle(),
                0.6f);
            RendererModels::Get()->DrawModel(
                Hash("PlayerLeftHand"), anim_tile, hands_position, Player::Get()->GetFacingAngle(),
                0.6f);
        }
        p->DrawLabel();
    }

    void
    SubDrawerPlayer::Pimpl::DrawLabel()
    /*/////////////////////////////*/
    {
        auto pos = Player::Get()->GetSpaceCoord().Translate(0.0f, GetPlayerElevation(), 0.0f);
        auto billboard_y_pos = 4.0f;
        auto billboard_pos = pos.Translate(0.0f, billboard_y_pos, 0.0f);
        auto billboard_text_pos = Camera::Get()->MoveCloserToCamera(billboard_pos, 0.01f);
        auto billboard_size = SizeF{0.9f, 0.03f};
        auto player_text = "Player";
        TextRenderer::Get()->DrawBillboardString(
            rendid_billboard_text_player_name_, player_text, billboard_text_pos, billboard_size);
    }

    float
    SubDrawerPlayer::Pimpl::GetPlayerElevation()
    /*//////////////////////////////////////*/
    {
        const auto player_pos = Player::Get()->GetPosition();
        auto player_tile_dx = player_pos.x - static_cast<int>(player_pos.x) - 0.5f;
        auto player_tile_dy = player_pos.y - static_cast<int>(player_pos.y) - 0.5f;
        auto elev00 = RenderLoop::playerElev00;
        auto elev10 = RenderLoop::playerElev10;
        auto elev11 = RenderLoop::playerElev11;
        auto elev01 = RenderLoop::playerElev01;
        auto tile_avg_elev = RenderLoop::playerTileAvgElev;
        auto elev_dx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
        auto elev_dy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
        auto player_elev = tile_avg_elev * kElevAmount + player_tile_dx * elev_dx * kElevAmount +
                          player_tile_dy * elev_dy * kElevAmount;
        return player_elev;
    }
}