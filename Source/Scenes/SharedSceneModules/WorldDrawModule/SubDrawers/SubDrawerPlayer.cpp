//////////////////////////////////////////////////////////////////////
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
        RenderId idBillboardTextPlayerName;
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
        p->idBillboardTextPlayerName = TextRenderer::Get()->NewBillboardString();
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
        float animTile;
        if (Player::Get()->data.movement.isMoving)
            animTile = SDL_GetTicks() * 3;
        else
            animTile = 0.0f;
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
                    Hash("Player2Sword"), animTile, pos, Player::Get()->GetFacingAngle(), 0.6f);
            }
            else if (Player::Get()->playerBuild == PlayerBuilds::Club)
            /*******************************************************/
            {
                RendererModels::Get()->DrawModel(
                    Hash("Player2Club"), animTile, pos, Player::Get()->GetFacingAngle(), 0.6f);
            }
            else
            /**/
            {
                RendererModels::Get()->DrawModel(
                    Hash("Player2"), animTile, pos, Player::Get()->GetFacingAngle(), 0.6f);
            }
        }
        else
        /**/
        {
            auto handsPosition = pos;
            handsPosition.y = Camera::Get()->cameraPosition.y - 0.5f * kTileSize;
            RendererModels::Get()->DrawModel(
                Hash("PlayerRightHand"), animTile, handsPosition, Player::Get()->GetFacingAngle(),
                0.6f);
            RendererModels::Get()->DrawModel(
                Hash("PlayerLeftHand"), animTile, handsPosition, Player::Get()->GetFacingAngle(),
                0.6f);
        }
        p->DrawLabel();
    }

    void
    SubDrawerPlayer::Pimpl::DrawLabel()
    /*/////////////////////////////*/
    {
        auto pos = Player::Get()->GetSpaceCoord().Translate(0.0f, GetPlayerElevation(), 0.0f);
        auto billboardYPos = 4.0f;
        auto billboardPos = pos.Translate(0.0f, billboardYPos, 0.0f);
        auto billboardTextPos = Camera::Get()->MoveCloserToCamera(billboardPos, 0.01f);
        auto billboardSize = SizeF{0.9f, 0.03f};
        auto playerText = "Player";
        TextRenderer::Get()->DrawBillboardString(
            idBillboardTextPlayerName, playerText, billboardTextPos, billboardSize);
    }

    float
    SubDrawerPlayer::Pimpl::GetPlayerElevation()
    /*//////////////////////////////////////*/
    {
        const auto playerPos = Player::Get()->GetPosition();
        auto playerTileDx = playerPos.x - static_cast<int>(playerPos.x) - 0.5f;
        auto playerTileDy = playerPos.y - static_cast<int>(playerPos.y) - 0.5f;
        auto elev00 = RenderLoop::playerElev00;
        auto elev10 = RenderLoop::playerElev10;
        auto elev11 = RenderLoop::playerElev11;
        auto elev01 = RenderLoop::playerElev01;
        auto tileAvgElev = RenderLoop::playerTileAvgElev;
        auto elevDx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
        auto elevDy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
        auto playerElev = tileAvgElev * kElevAmount + playerTileDx * elevDx * kElevAmount +
                          playerTileDy * elevDy * kElevAmount;
        return playerElev;
    }
}