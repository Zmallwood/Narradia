#include "GuiButton.hpp"
#include "Engine/Core/Cursor.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "GuiContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiButton::Pimpl {
      public:
        std::string_view text_;
        RectangleF bounds_;
        std::function<void()> action_;
        std::string_view image_name_;
        std::string_view image_name_hovered_;
        RenderId rendid_image_;
        RenderId rendid_label_text_;
        bool hovered_;
        GuiContainer *parent_container_ = nullptr;
    };

    GuiButton::GuiButton(
        const std::string_view &text_, RectangleF bounds_, std::function<void()> action_,
        GuiContainer *parentContainer_, const std::string_view &imageName_,
        const std::string_view &imageNameHovered_)
        : p(std::make_shared<Pimpl>()) {
        p->text_ = text_;
        p->bounds_ = bounds_;
        p->action_ = action_;
        p->parent_container_ = parentContainer_;
        p->image_name_ = imageName_;
        p->image_name_hovered_ = imageNameHovered_;
        p->rendid_image_ = Renderer2DImages::Get()->NewImage();
        p->rendid_label_text_ = TextRenderer::Get()->NewString();
    }

    void GuiButton::Update() {
        p->hovered_ = false;
        auto used_bounds = p->bounds_;
        if (p->parent_container_) {
            used_bounds.x += p->parent_container_->GetPosition().x;
            used_bounds.y += p->parent_container_->GetPosition().y;
        }
        if (used_bounds.Contains(GetMousePositionF())) {
            Cursor::Get()->SetCursorType(CursorTypes::Hovering);
            p->hovered_ = true;
            MouseInput::Get()->GetLeftButton().AddFiredAction(
                "PerformGuiButtonAction", [&] { p->action_(); }, 1);
        }
    }

    void GuiButton::Render() const {
        const std::string_view *used_image_name;
        if (p->hovered_)
            used_image_name = &p->image_name_hovered_;
        else
            used_image_name = &p->image_name_;
        auto used_bounds = p->bounds_;
        if (p->parent_container_) {
            used_bounds.x += p->parent_container_->GetPosition().x;
            used_bounds.y += p->parent_container_->GetPosition().y;
        }
        Renderer2DImages::Get()->DrawImage(*used_image_name, p->rendid_image_, used_bounds);
        TextRenderer::Get()->DrawString(
            p->rendid_label_text_, p->text_, used_bounds.Center(), Colors::wheat, true);
    }
}