//////////////////////////////////////////////////////////////////////
#include "Cursor.hpp"
#include "Engine/Core/Graphics/Rendering/Rendering.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Rendering/Renderer2DImages.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Cursor::Pimpl
    /*///////////////*/
    {
      public:
        static constexpr float kCursorSize = 0.04f;
        const int hashCursorDefault = Hash("CursorDefault");
        const int hashCursorAttack = Hash("CursorAttack");
        const int hashCursorHovering = Hash("CursorHovering");
        const int hashCursorRotating = Hash("CursorRotating");
        GLuint idxCursorImage;
        Point2 savedMousePosPx;
        CursorTypes currCursorType = CursorTypes::Normal;
        bool visibleThisFrame = true;
    };

    Cursor::Cursor()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        SDL_ShowCursor(0);
        p->idxCursorImage = Renderer2DImages::Get().NewImage();
    }

    void Cursor::Render() const
    /*///////////////////////*/
    {
        if (!p->visibleThisFrame)
            return;
        const auto mousePos = GetMousePositionF();
        const auto usedCursorSize =
            SizeF{Pimpl::kCursorSize, ConvertWidthToHeight(Pimpl::kCursorSize)};
        const auto bounds = RectangleF{
            mousePos.Translate(-usedCursorSize.width / 2.0f, -usedCursorSize.height / 2.0f),
            usedCursorSize};
        int imageNameHash;
        switch (p->currCursorType)
        /************************/
        {
        case CursorTypes::Hovering:
            /*********************/
            imageNameHash = p->hashCursorHovering;
            break;
        case CursorTypes::Rotating:
            /*********************/
            imageNameHash = p->hashCursorRotating;
            break;
        case CursorTypes::Attack:
            /*******************/
            imageNameHash = p->hashCursorAttack;
            break;
        case CursorTypes::Normal:
        default:
            /*******************/
            imageNameHash = p->hashCursorDefault;
        }
        Renderer2DImages::Get().DrawImage(imageNameHash, p->idxCursorImage, bounds);
    }

    void Cursor::Reset()
    /*////////////////*/
    {
        p->currCursorType = CursorTypes::Normal;
        p->visibleThisFrame = true;
    }

    void Cursor::SavePosition()
    /*///////////////////////*/
    {
        p->savedMousePosPx = GetMousePositionPx();
    }

    void Cursor::RestoreSavedPosition() const
    /*/////////////////////////////////////*/
    {
        SDL_WarpMouseInWindow(
            Graphics::Get().GetWindow(), p->savedMousePosPx.x, p->savedMousePosPx.y);
    }

    Point2 Cursor::GetSavedPosition() const
    /*///////////////////////////////////*/
    {
        return p->savedMousePosPx;
    }

    void Cursor::SetCursorType(CursorTypes newCursorType)
    /*/////////////////////////////////////////////////*/
    {
        p->currCursorType = newCursorType;
    }

    void Cursor::LockMousePos() const
    /*/////////////////////////////*/
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    void Cursor::UnlockMousePos() const
    /*///////////////////////////////*/
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    void Cursor::HideThisFrame()
    /*////////////////////////*/
    {
        p->visibleThisFrame = false;
    }
}
//////////////////////////////////////////////////////////////////////