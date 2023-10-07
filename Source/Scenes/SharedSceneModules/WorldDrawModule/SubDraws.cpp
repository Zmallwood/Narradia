//////////////////////////////////////////////////////////////////////
#include "SubDraws.hpp"
#include "Camera.hpp"
#include "ConfigurationWorldDraw.hpp"
#include "Engine/Core/Graphics/Rendering/CameraGl.hpp"
#include "Engine/Core/Graphics/Rendering/RendererBillboardImages.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
#include "Engine/Core/Graphics/Rendering/RendererTiles.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "RenderLoop.hpp"
#include "Scenes/PlayScene/Modules/MobTargeting.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "MovementUtilities.hpp"
#include "World/Actors/Companion.hpp"
#include "World/Actors/Mob.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/Tile.hpp"
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

    class SubDrawerMob::Pimpl
    /*///////////////////*/
    {
      public:
        void InitializeIds();
        int GetAnimationValue();
        void DrawShadow();
        void DrawMobModel();
        void IfCaseDrawMobLabel();
        std::map<int, std::map<int, RenderId>> idsBillboardTexts;
        const int kShowHitEffectDuration = 600;
    };

    SubDrawerMob::SubDrawerMob()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    SubDrawerMob::Create()
    /*////////////////*/
    {
        p->InitializeIds();
    }

    void
    SubDrawerMob::DrawMob()
    /*/////////////////*/
    {
        auto tile = RenderLoop::currTile;
        auto mob = tile->GetMob();
        if (nullptr == mob)
            return;
        p->DrawShadow();
        p->DrawMobModel();
        p->IfCaseDrawMobLabel();
    }

    void
    SubDrawerMob::Pimpl::InitializeIds()
    /*//////////////////////////////*/
    {
        for (auto x = -kMaxRenderRadius; x < kMaxRenderRadius; x++)
            for (auto y = -kMaxRenderRadius; y < kMaxRenderRadius; y++)
                idsBillboardTexts[x][y] = TextRenderer::Get()->NewBillboardString();
    }

    int
    SubDrawerMob::Pimpl::GetAnimationValue()
    /*//////////////////////////////////*/
    {
        auto tileCoord = RenderLoop::currTileCoord;
        return tileCoord.x * tileCoord.y * 10 + SDL_GetTicks();
    }

    void
    SubDrawerMob::Pimpl::DrawShadow()
    /*///////////////////////////*/
    {
        auto v0 = RenderLoop::currVertTile.v0;
        auto x0 = v0.position.x;
        auto z0 = v0.position.z;
        auto mob = RenderLoop::currTile->GetMob();
        auto minorMovementOffset = GetMinorMovementOffsetForMob(mob.get());
        auto elev00 = RenderLoop::currElev00;
        auto elev10 = RenderLoop::currElev10;
        auto elev11 = RenderLoop::currElev11;
        auto elev01 = RenderLoop::currElev01;
        auto elevDx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
        auto elevDy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
        auto mobElev = RenderLoop::currTileAvgElev * kElevAmount +
                       minorMovementOffset.x * elevDx * kElevAmount +
                       minorMovementOffset.y * elevDy * kElevAmount;
        RendererModels::Get()->DrawModel(
            Hash("Shadow"), 0,
            {x0 + kTileSize / 2 + minorMovementOffset.x * kTileSize, mobElev + 0.05f * kTileSize,
             z0 + kTileSize / 2 + minorMovementOffset.y * kTileSize},
            0.0f, 0.6f);
    }

    void
    SubDrawerMob::Pimpl::DrawMobModel()
    /*/////////////////////////////*/
    {
        auto v0 = RenderLoop::currVertTile.v0;
        auto x0 = v0.position.x;
        auto z0 = v0.position.z;
        auto mob = RenderLoop::currTile->GetMob();
        auto animValue = GetAnimationValue();
        auto minorMovementOffset = GetMinorMovementOffsetForMob(mob.get());
        auto deltaX = RenderLoop::currTileCoord.x - mob->GetPreviousCoordinate().x;
        auto deltaY = RenderLoop::currTileCoord.y - mob->GetPreviousCoordinate().y;
        auto absDeltaX = std::abs(deltaX);
        auto absDeltaY = std::abs(deltaY);
        auto normX = 0;
        auto normY = 0;
        if (deltaX)
            normX = deltaX / absDeltaX;
        if (deltaY)
            normY = deltaY / absDeltaY;
        auto facingAngle = -90.0f - std::atan2(normY, deltaX) * 180.0f / M_PI;
        auto elev00 = RenderLoop::currElev00;
        auto elev10 = RenderLoop::currElev10;
        auto elev11 = RenderLoop::currElev11;
        auto elev01 = RenderLoop::currElev01;
        auto elevDx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
        auto elevDy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
        auto mobElev = RenderLoop::currTileAvgElev * kElevAmount +
                       minorMovementOffset.x * elevDx * kElevAmount +
                       minorMovementOffset.y * elevDy * kElevAmount;
        RendererModels::Get()->DrawModel(
            mob->GetMobType(), animValue,
            {x0 + kTileSize / 2 + minorMovementOffset.x * kTileSize,
             mobElev + mob->GetDistanceAboveGround(),
             z0 + kTileSize / 2 + minorMovementOffset.y * kTileSize},
            facingAngle, 0.5f, 1.0f);
        if (SDL_GetTicks() < mob->GetTicksLastHitRecieved() + kShowHitEffectDuration)
        /***************************************************************************/
        {
            RendererModels::Get()->DrawModel(
                Hash("HitEffect"), animValue,
                {x0 + kTileSize / 2 + minorMovementOffset.x * kTileSize,
                 mobElev + mob->GetDistanceAboveGround(),
                 z0 + kTileSize / 2 + minorMovementOffset.y * kTileSize},
                facingAngle, 0.5f, 1.0f);
        }
    }

    void
    SubDrawerMob::Pimpl::IfCaseDrawMobLabel()
    /*///////////////////////////////////*/
    {
        auto v0 = RenderLoop::currVertTile.v0;
        auto x0 = v0.position.x;
        auto z0 = v0.position.z;
        auto mob = RenderLoop::currTile->GetMob();
        auto minorMovementOffset = GetMinorMovementOffsetForMob(mob.get());
        auto billboardYPos = 3.f;
        auto billboardPos = Point3F{
            x0 + kTileSize / 2 + minorMovementOffset.x * kTileSize,
            RenderLoop::currTileAvgElev * kElevAmount + mob->GetDistanceAboveGround() +
                billboardYPos,
            z0 + kTileSize / 2 + minorMovementOffset.y * kTileSize};
        auto billboardTextPos = Camera::Get()->MoveCloserToCamera(billboardPos, 0.01f);
        auto billboardSize = SizeF{0.9f, 0.03f};
        if (TileHovering::Get()->hoveredTile == RenderLoop::currTileCoord)
        /***************************************************************/
        {
            TextRenderer::Get()->DrawBillboardString(
                idsBillboardTexts[RenderLoop::currX][RenderLoop::currY], "Mob Lvl. 1",
                billboardTextPos, billboardSize);
        }
    }

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

    void
    SubDrawerSky::DrawSky()
    /*/////////////////*/
    {
        const auto playerWorldAreaPos = Player::Get()->GetWorldAreaPos();
        if (playerWorldAreaPos.z >= 0)
            glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        else
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}