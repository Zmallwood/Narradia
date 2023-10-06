//////////////////////////////////////////////////////////////////////
#include "SubDraws.hpp"
#include "Camera.hpp"
#include "Configuration.hpp"
#include "Engine/Core/Graphics/Rendering/Rendering.hpp"
#include "RenderLoop.hpp"
#include "Scenes/Shared/TileHovering.hpp"
#include "Utilities.hpp"
#include "World/Companion.hpp"
#include "World/MapArea.hpp"
#include "World/Mob.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "Engine/Core/Graphics/Rendering/CameraGl.hpp"
#include "Engine/Core/Graphics/Rendering/RendererBillboardImages.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SubDrawCompanion::Pimpl
    /*/////////////////////////*/
    {
      public:
        void DrawExclamationMark();
        std::map<int, std::map<int, RenderId>> idsCompanionBboardExlamationMarks;
    };

    SubDrawCompanion::SubDrawCompanion()
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////*/
    {
    }

    void SubDrawCompanion::Create()
    /*///////////////////////////*/
    {
        for (auto x = -kMaxRenderRadius; x < kMaxRenderRadius; x++)
        /*********************************************************/
        {
            for (auto y = -kMaxRenderRadius; y < kMaxRenderRadius; y++)
            /*********************************************************/
            {
                p->idsCompanionBboardExlamationMarks[x][y] =
                    RendererBillboardImages::Get().NewBillboardImage();
            }
        }
    }

    void SubDrawCompanion::DrawCompanion()
    /*//////////////////////////////////*/
    {
        auto tile = RenderLoop::currTile;
        auto tileSize = kTileSize;
        auto elevAmount = kElevAmount;
        auto tileAvgElev = RenderLoop::currTileAvgElev;
        auto v0 = RenderLoop::currVertTile.v0;
        auto x0 = v0.position.x;
        auto y0 = v0.position.y;
        auto z0 = v0.position.z;
        auto x = RenderLoop::currX;
        auto y = RenderLoop::currY;
        auto companion = tile->GetCompanion();
        if (companion)
        /************/
        {
            auto minorMovementOffset = GetMinorMovementOffsetForCompanion(companion.get());
            auto elev00 = RenderLoop::currElev00;
            auto elev10 = RenderLoop::currElev10;
            auto elev11 = RenderLoop::currElev11;
            auto elev01 = RenderLoop::currElev01;
            auto tileAvgElev = RenderLoop::playerTileAvgElev;
            auto elevAmount = kElevAmount;
            auto tileSize = kTileSize;
            auto elevDx = ((elev10 - elev00) + (elev11 - elev01)) / 2.0f;
            auto elevDy = ((elev01 - elev00) + (elev11 - elev10)) / 2.0f;
            auto companionElev = RenderLoop::currTileAvgElev * elevAmount +
                                 minorMovementOffset.x * elevDx * elevAmount +
                                 minorMovementOffset.y * elevDy * elevAmount;
            auto deltaX = RenderLoop::currTileCoord.x - companion->GetPreviousCoordinate().x;
            auto deltaY = RenderLoop::currTileCoord.y - companion->GetPreviousCoordinate().y;
            auto absDeltaX = std::abs(deltaX);
            auto absDeltaY = std::abs(deltaY);
            auto normX = 0;
            auto normY = 0;
            if (deltaX)
                normX = deltaX / absDeltaX;
            if (deltaY)
                normY = deltaY / absDeltaY;
            auto facingAngle = -90.0f - std::atan2(normY, deltaX) * 180.0f / M_PI;
            RendererModels::Get().DrawModel(
                Hash("Shadow"), 0,
                {x0 + tileSize / 2 + minorMovementOffset.x * tileSize,
                 companionElev + 0.05f * tileSize,
                 z0 + tileSize / 2 + minorMovementOffset.y * tileSize},
                0.0f, 0.6f);
            auto animValue = SDL_GetTicks() * 3;
            RendererModels::Get().DrawModel(
                Hash("Companion"), animValue,
                {x0 + tileSize / 2 + minorMovementOffset.x * tileSize, companionElev,
                 z0 + tileSize / 2 + minorMovementOffset.y * tileSize},
                facingAngle, 0.6f, 1.0f);
            p->DrawExclamationMark();
        }
    }

    void SubDrawCompanion::Pimpl::DrawExclamationMark()
    /*///////////////////////////////////////////////*/
    {
        auto tile = RenderLoop::currTile;
        auto companion = tile->GetCompanion();
        if (companion->GetStamina() > 0)
            return;
        auto x = RenderLoop::currX;
        auto y = RenderLoop::currY;
        auto v0 = RenderLoop::currVertTile.v0;
        auto x0 = v0.position.x;
        auto y0 = v0.position.y;
        auto z0 = v0.position.z;
        auto billboardYPos = 3.0f;
        auto billboardPos = Point3F{
            x0 + kTileSize / 2, RenderLoop::currTileAvgElev * kElevAmount + billboardYPos,
            z0 + kTileSize / 2};
        auto billboardSize = SizeF{0.9f, 0.03f};
        auto exlamationMarkWidth = 0.1f;
        auto exlamationMarkHeight = 0.1f;
        auto exlamationMarkPos = billboardPos.Translate(0.f, kTileSize, 0.f);
        exlamationMarkPos = Camera::Get().MoveCloserToCamera(exlamationMarkPos, kTileSize);
        auto exlamationMarkBounds = RectangleF{-.5f, -.5f, 1.f, 1.f};
        auto exlamationMarkSize = SizeF{exlamationMarkWidth, exlamationMarkHeight};
        RendererBillboardImages::Get().DrawBillboardImage(
            Hash("ExclamationMark"), idsCompanionBboardExlamationMarks[x][y], exlamationMarkPos,
            exlamationMarkBounds, exlamationMarkSize);
    }

    class SubDrawGround::Pimpl
    /*//////////////////////*/
    {
      public:
        std::map<int, std::map<int, RenderId>> idsTileLayers;
    };

    SubDrawGround::SubDrawGround()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void SubDrawGround::Create()
    /*////////////////////////*/
    {
        for (auto y = 0; y < MapArea::GetMapSize().height; y++)
            for (auto x = 0; x < MapArea::GetMapSize().width; x++)
                p->idsTileLayers[x][y] = RendererTiles::Get().NewTile();
    }

    void SubDrawGround::DrawGround(bool doDrawTerritoryBorders)
    /*///////////////////////////////////////////////////////*/
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
        RendererTiles::Get().DrawTile(groundType, tile->GetTileRendId());
        auto groundLayerType = tile->GetGroundLayerType();
        if (groundLayerType)
        /******************/
        {
            v0.position.y += 0.03f;
            v1.position.y += 0.03f;
            v2.position.y += 0.03f;
            v3.position.y += 0.03f;
            RendererTiles::Get().UpdateDrawTile(
                groundLayerType, p->idsTileLayers[tileCoord.x][tileCoord.y], v0, v1, v2, v3,
                normal00, normal10, normal11, normal01);
        }
        auto hoveredTile = TileHovering::Get().hoveredTile;
        if (hoveredTile.x == tileCoord.x && hoveredTile.y == tileCoord.y)
        /***************************************************************/
        {
            v0.position.y += 0.03f;
            v1.position.y += 0.03f;
            v2.position.y += 0.03f;
            v3.position.y += 0.03f;
            RendererTiles::Get().UpdateDrawTile(
                Hash("TileHovered"), p->idsTileLayers[tileCoord.x][tileCoord.y], v0, v1, v2, v3,
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
                RendererTiles::Get().UpdateDrawTile(
                    Hash("PlayerClaimedTileBorderE"), p->idsTileLayers[tileCoord.x][tileCoord.y],
                    v0, v1, v2, v3, normal00, normal10, normal11, normal01);
            }
            if (thisTileClaimedByPlayer != southTileClaimedByPlayer)
            /******************************************************/
            {
                RendererTiles::Get().UpdateDrawTile(
                    Hash("PlayerClaimedTileBorderS"), p->idsTileLayers[tileCoord.x][tileCoord.y],
                    v0, v1, v2, v3, normal00, normal10, normal11, normal01);
            }
        }
    }

    class SubDrawMob::Pimpl
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

    SubDrawMob::SubDrawMob()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void SubDrawMob::Create()
    /*/////////////////////*/
    {
        p->InitializeIds();
    }

    void SubDrawMob::DrawMob()
    /*//////////////////////*/
    {
        auto tile = RenderLoop::currTile;
        auto mob = tile->GetMob();
        if (nullptr == mob)
            return;
        p->DrawShadow();
        p->DrawMobModel();
        p->IfCaseDrawMobLabel();
    }

    void SubDrawMob::Pimpl::InitializeIds()
    /*///////////////////////////////////*/
    {
        for (auto x = -kMaxRenderRadius; x < kMaxRenderRadius; x++)
            for (auto y = -kMaxRenderRadius; y < kMaxRenderRadius; y++)
                idsBillboardTexts[x][y] = TextRenderer::Get().NewBillboardString();
    }

    int SubDrawMob::Pimpl::GetAnimationValue()
    /*//////////////////////////////////////*/
    {
        auto tileCoord = RenderLoop::currTileCoord;
        return tileCoord.x * tileCoord.y * 10 + SDL_GetTicks();
    }

    void SubDrawMob::Pimpl::DrawShadow()
    /*////////////////////////////////*/
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
        RendererModels::Get().DrawModel(
            Hash("Shadow"), 0,
            {x0 + kTileSize / 2 + minorMovementOffset.x * kTileSize, mobElev + 0.05f * kTileSize,
             z0 + kTileSize / 2 + minorMovementOffset.y * kTileSize},
            0.0f, 0.6f);
    }

    void SubDrawMob::Pimpl::DrawMobModel()
    /*//////////////////////////////////*/
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
        RendererModels::Get().DrawModel(
            mob->GetMobType(), animValue,
            {x0 + kTileSize / 2 + minorMovementOffset.x * kTileSize,
             mobElev + mob->GetDistanceAboveGround(),
             z0 + kTileSize / 2 + minorMovementOffset.y * kTileSize},
            facingAngle, 0.5f, 1.0f);
        if (SDL_GetTicks() < mob->GetTicksLastHitRecieved() + kShowHitEffectDuration)
        /***************************************************************************/
        {
            RendererModels::Get().DrawModel(
                Hash("HitEffect"), animValue,
                {x0 + kTileSize / 2 + minorMovementOffset.x * kTileSize,
                 mobElev + mob->GetDistanceAboveGround(),
                 z0 + kTileSize / 2 + minorMovementOffset.y * kTileSize},
                facingAngle, 0.5f, 1.0f);
        }
    }

    void SubDrawMob::Pimpl::IfCaseDrawMobLabel()
    /*////////////////////////////////////////*/
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
        auto billboardTextPos = Camera::Get().MoveCloserToCamera(billboardPos, 0.01f);
        auto billboardSize = SizeF{0.9f, 0.03f};
        if (TileHovering::Get().hoveredTile == RenderLoop::currTileCoord)
        /**********************************************************************/
        {
            TextRenderer::Get().DrawBillboardString(
                idsBillboardTexts[RenderLoop::currX][RenderLoop::currY], "Mob Lvl. 1",
                billboardTextPos, billboardSize);
        }
    }

    class SubDrawPlayer::Pimpl
    /*//////////////////////*/
    {
      public:
        void DrawLabel();
        float GetPlayerElevation();
        RenderId idBillboardTextPlayerName;
    };

    SubDrawPlayer::SubDrawPlayer()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void SubDrawPlayer::Create()
    /*////////////////////////*/
    {
        p->idBillboardTextPlayerName = TextRenderer::Get().NewBillboardString();
    }

    void SubDrawPlayer::DrawPlayer()
    /*////////////////////////////*/
    {
        auto pos = Player::Get().GetSpaceCoord().Translate(0.0f, p->GetPlayerElevation(), 0.0f);
        if (Player::Get().mounted)
        /**********************/
        {
            RendererModels::Get().DrawModel(
                Hash("Mount1"), 0, pos, Player::Get().GetFacingAngle(), 0.8f);
        }
        float animTile;
        if (Player::Get().data.movement.isMoving)
            animTile = SDL_GetTicks() * 3;
        else
            animTile = 0.0f;
        RendererModels::Get().DrawModel(
            Hash("Shadow"), 0,
            Player::Get().GetSpaceCoord().Translate(
                0.0f,
                Player::Get().data.movement.jumpHeight + p->GetPlayerElevation() +
                    0.05f * kTileSize,
                0.0f),
            0.0f, 0.6f);
        if (Camera::Get().cameraDistance > 2.0f)
        /**************************************/
        {
            RendererModels::Get().DrawModel(
                Hash("Player2"), animTile, pos, Player::Get().GetFacingAngle(), 0.6f);
        }
        else
        /**/
        {
            auto handsPosition = pos;
            handsPosition.y = Camera::Get().cameraPosition.y - 0.5f * kTileSize;
            RendererModels::Get().DrawModel(
                Hash("PlayerRightHand"), animTile, handsPosition, Player::Get().GetFacingAngle(),
                0.6f);
            RendererModels::Get().DrawModel(
                Hash("PlayerLeftHand"), animTile, handsPosition, Player::Get().GetFacingAngle(),
                0.6f);
        }
        p->DrawLabel();
    }

    void SubDrawPlayer::Pimpl::DrawLabel()
    /*//////////////////////////////////*/
    {
        auto pos = Player::Get().GetSpaceCoord().Translate(0.0f, GetPlayerElevation(), 0.0f);
        auto billboardYPos = 4.0f;
        auto billboardPos = pos.Translate(0.0f, billboardYPos, 0.0f);
        auto billboardTextPos = Camera::Get().MoveCloserToCamera(billboardPos, 0.01f);
        auto billboardSize = SizeF{0.9f, 0.03f};
        auto playerText = "Player";
        TextRenderer::Get().DrawBillboardString(
            idBillboardTextPlayerName, playerText, billboardTextPos, billboardSize);
    }

    float SubDrawPlayer::Pimpl::GetPlayerElevation()
    /*////////////////////////////////////////////*/
    {
        const auto playerPos = Player::Get().GetPosition();
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

    void SubDrawSky::DrawSky()
    /*//////////////////////*/
    {
        const auto playerWorldAreaPos = Player::Get().GetWorldAreaPos();
        if (playerWorldAreaPos.z >= 0)
            glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        else
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
//////////////////////////////////////////////////////////////////////