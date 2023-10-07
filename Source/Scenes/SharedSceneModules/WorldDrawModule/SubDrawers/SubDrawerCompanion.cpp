//////////////////////////////////////////////////////////////////////
#include "SubDrawerCompanion.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/ConfigurationWorldDraw.hpp"
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
        std::map<int, std::map<int, RenderId>> idsCompanionBboardExlamationMarks;
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
                p->idsCompanionBboardExlamationMarks[x][y] =
                    RendererBillboardImages::Get()->NewBillboardImage();
            }
        }
    }

    void
    SubDrawerCompanion::DrawCompanion()
    /*/////////////////////////////*/
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
            RendererModels::Get()->DrawModel(
                Hash("Shadow"), 0,
                {x0 + tileSize / 2 + minorMovementOffset.x * tileSize,
                 companionElev + 0.05f * tileSize,
                 z0 + tileSize / 2 + minorMovementOffset.y * tileSize},
                0.0f, 0.6f);
            auto animValue = SDL_GetTicks() * 3;
            RendererModels::Get()->DrawModel(
                Hash("Companion"), animValue,
                {x0 + tileSize / 2 + minorMovementOffset.x * tileSize, companionElev,
                 z0 + tileSize / 2 + minorMovementOffset.y * tileSize},
                facingAngle, 0.6f, 1.0f);
            p->DrawExclamationMark();
        }
    }

    void
    SubDrawerCompanion::Pimpl::DrawExclamationMark()
    /*//////////////////////////////////////////*/
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
        exlamationMarkPos = Camera::Get()->MoveCloserToCamera(exlamationMarkPos, kTileSize);
        auto exlamationMarkBounds = RectangleF{-.5f, -.5f, 1.f, 1.f};
        auto exlamationMarkSize = SizeF{exlamationMarkWidth, exlamationMarkHeight};
        RendererBillboardImages::Get()->DrawBillboardImage(
            Hash("ExclamationMark"), idsCompanionBboardExlamationMarks[x][y], exlamationMarkPos,
            exlamationMarkBounds, exlamationMarkSize);
    }
}