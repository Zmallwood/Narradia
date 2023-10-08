#include "GuiTextBox.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DImages.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/Input/KeyboardInput.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "GuiContainer.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiTextBox::Pimpl {
      public:
        inline static GuiTextBox *active_text_box_ = nullptr;
        std::shared_ptr<std::function<void()>> text_change_event_;
        RenderId rendid_background_image_;
        RenderId rendid_text_;
        RectangleF bounds_;
        GuiContainer *parent_container_ = nullptr;
        int cursor_position_ = 0;
        std::string text_;
    };

    GuiTextBox::GuiTextBox(
        RectangleF bounds_, GuiContainer *parentContainer_, std::string text_,
        std::shared_ptr<std::function<void()>> textChangeEvent_)
        : p(std::make_shared<Pimpl>()) {
        p->bounds_ = bounds_;
        p->parent_container_ = parentContainer_;
        p->text_change_event_ = textChangeEvent_;
        p->rendid_background_image_ = Renderer2DImages::Get()->NewImage();
        p->rendid_text_ = TextRenderer::Get()->NewString();
        p->cursor_position_ = text_.length();
        p->text_ = text_;
    }

    void GuiTextBox::Update() {
        auto mouse_position_f = GetMousePositionF();
        auto used_bounds = p->bounds_;
        if (p->parent_container_) {
            used_bounds.x += p->parent_container_->GetPosition().x;
            used_bounds.y += p->parent_container_->GetPosition().y;
        }
        if (used_bounds.Contains(mouse_position_f)) {
            MouseInput::Get()->GetLeftButton().AddFiredAction(
                "ClickTextBox",
                [&]
                /********************/
                {
                    GuiTextBox::Pimpl::active_text_box_ = this;
                    KeyboardInput::Get()->ResetTextInput();
                },
                0);
        }
        if (Pimpl::active_text_box_ == this) {
            auto new_text_input = KeyboardInput::Get()->PickTextInput();
            p->text_ += new_text_input;
            p->cursor_position_ += new_text_input.length();
            if (new_text_input != "")
                if (p->text_change_event_)
                    (*p->text_change_event_)();
            if (KeyboardInput::Get()->KeyHasBeenFiredPickResult(SDLK_RIGHT)) {
                p->cursor_position_ =
                    std::min(static_cast<int>(p->text_.length()), p->cursor_position_ + 1);
            }
            else if (KeyboardInput::Get()->KeyHasBeenFiredPickResult(SDLK_LEFT)) {
                p->cursor_position_ = std::max(0, p->cursor_position_ - 1);
            }
            else if (KeyboardInput::Get()->KeyHasBeenFiredPickResult(SDLK_BACKSPACE)) {
                if (p->cursor_position_ > 0) {
                    p->text_.erase(p->cursor_position_ - 1, 1);
                    p->cursor_position_--;
                    if (p->text_change_event_)
                        (*p->text_change_event_)();
                }
            }
            else if (KeyboardInput::Get()->KeyHasBeenFiredPickResult(SDLK_DELETE)) {
                if (p->cursor_position_ < p->text_.length()) {
                    p->text_.erase(p->cursor_position_, 1);
                    if (p->text_change_event_)
                        (*p->text_change_event_)();
                }
            }
        }
    }

    void GuiTextBox::Render() const {
        auto used_bounds = p->bounds_;
        if (p->parent_container_) {
            used_bounds.x += p->parent_container_->GetPosition().x;
            used_bounds.y += p->parent_container_->GetPosition().y;
        }
        if (Pimpl::active_text_box_ == this)
            Renderer2DImages::Get()->DrawImage(
                "GTextBoxActiveBackground", p->rendid_background_image_, used_bounds);
        else
            Renderer2DImages::Get()->DrawImage(
                "GTextBoxBackground", p->rendid_background_image_, used_bounds);
        auto used_text = p->text_;
        if (Pimpl::active_text_box_ == this) {
            if (SDL_GetTicks() % 1000 > 500)
                used_text.insert(p->cursor_position_, "|");
            else
                used_text.insert(p->cursor_position_, " ");
        }
        TextRenderer::Get()->DrawString(
            p->rendid_text_, used_text, used_bounds.GetPosition().Translate(0.005f, 0.015f));
    }

    std::string_view GuiTextBox::GetText() {
        return p->text_;
    }

    void GuiTextBox::SetText(const std::string &newText) {
        p->text_ = newText;
    }
}