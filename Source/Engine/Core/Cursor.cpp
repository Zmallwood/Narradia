#include "Cursor.hpp"
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
        const int kHashCursorDefault = Hash("CursorDefault");
        const int kHashCursorAttack = Hash("CursorAttack");
        const int kHashCursorHovering = Hash("CursorHovering");
        const int kHashCursorRotating = Hash("CursorRotating");
        RenderId rendid_cursor_image_;
        Point2 saved_mouse_position_pixels_;
        CursorTypes current_cursor_type_ = CursorTypes::Normal;
        bool visible_this_frame_ = true;
    };

    Cursor::Cursor()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        SDL_ShowCursor(0);
        p->rendid_cursor_image_ = Renderer2DImages::Get()->NewImage();
    }

    void
    Cursor::Render() const
    /*//////////////////*/
    {
        if (!p->visible_this_frame_)
            return;
        const auto mouse_position = GetMousePositionF();
        const auto used_cursor_size =
            SizeF{Pimpl::kCursorSize, ConvertWidthToHeight(Pimpl::kCursorSize)};
        const auto bounds = RectangleF{
            mouse_position.Translate(-used_cursor_size.width / 2.0f, -used_cursor_size.height / 2.0f),
            used_cursor_size};
        int image_name_hash;
        switch (p->current_cursor_type_)
        /************************/
        {
        case CursorTypes::Hovering:
            /*********************/
            image_name_hash = p->kHashCursorHovering;
            break;
        case CursorTypes::Rotating:
            /*********************/
            image_name_hash = p->kHashCursorRotating;
            break;
        case CursorTypes::Attack:
            /*******************/
            image_name_hash = p->kHashCursorAttack;
            break;
        case CursorTypes::Normal:
        default:
            /*******************/
            image_name_hash = p->kHashCursorDefault;
        }
        Renderer2DImages::Get()->DrawImage(image_name_hash, p->rendid_cursor_image_, bounds);
    }

    void
    Cursor::Reset()
    /*///////////*/
    {
        p->current_cursor_type_ = CursorTypes::Normal;
        p->visible_this_frame_ = true;
    }

    void
    Cursor::SavePosition()
    /*//////////////////*/
    {
        p->saved_mouse_position_pixels_ = GetMousePositionPx();
    }

    void
    Cursor::RestoreSavedPosition() const
    /*////////////////////////////////*/
    {
        SDL_WarpMouseInWindow(
            Graphics::Get()->GetWindow(), p->saved_mouse_position_pixels_.x, p->saved_mouse_position_pixels_.y);
    }

    Point2
    Cursor::GetSavedPosition() const
    /*////////////////////////////*/
    {
        return p->saved_mouse_position_pixels_;
    }

    void
    Cursor::SetCursorType(CursorTypes newCursorType)
    /*////////////////////////////////////////////*/
    {
        p->current_cursor_type_ = newCursorType;
    }

    void
    Cursor::LockMousePos() const
    /*////////////////////////*/
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    void
    Cursor::UnlockMousePos() const
    /*//////////////////////////*/
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    void
    Cursor::HideThisFrame()
    /*///////////////////*/
    {
        p->visible_this_frame_ = false;
    }
}