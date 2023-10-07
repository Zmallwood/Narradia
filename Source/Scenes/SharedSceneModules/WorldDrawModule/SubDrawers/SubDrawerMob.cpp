#include "SubDrawerMob.hpp"
#include "Engine/Core/Graphics/Rendering/RendererModels.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/Camera.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/ConfigurationWorldDraw.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/MovementUtilities.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/RenderLoop.hpp"
#include "World/Actors/Mob.hpp"
#include "World/Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
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
}