#include "TextOutBox.hpp"
#include "Graphics/Rendering/Renderer2DImages.hpp"
#include "Graphics/Rendering/Text/TextRenderer.hpp"
#include "Input/KeyboardInput.hpp"
#include "SceneManager.hpp"
#include "Scenes/PlayScene/Gui/ExperienceBar.hpp"
#include "TextLine.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class TextOutBox::Pimpl {
      public:
        const RectangleF kbounds = {0.0f, 0.8f, 0.3f, 0.2f};
        const float kTextLineHeight = 0.02f;
        const float kSplitLineHeight = 0.001f;
        RenderId rendid_image_ = 0;
        RenderId rendid_split_line_;
        RenderId rendid_command_line_input_arrow_;
        RenderId rendid_input_text_;
        std::vector<TextLine> text_lines_;
        std::vector<GLuint> rendids_text_lines_;
        bool enabled_ = true;
        bool input_active_ = false;
        int cursor_position_ = 0;
        std::string input_text_;
    };

    TextOutBox::TextOutBox()
        : p(std::make_shared<Pimpl>()) {
        p->rendid_image_ = Renderer2DImages::Get()->NewImage();
        p->rendid_split_line_ = Renderer2DImages::Get()->NewImage();
        p->rendid_command_line_input_arrow_ = Renderer2DImages::Get()->NewImage();
        p->rendid_input_text_ = TextRenderer::Get()->NewString();
        auto max_num_lines = GetMaxNumLines();
        for (auto i = 0; i < max_num_lines; i++)
            p->rendids_text_lines_.push_back(TextRenderer::Get()->NewString());
    }

    void TextOutBox::Update() {
        if (KeyboardInput::Get()->KeyHasBeenFiredPickResult(SDLK_RETURN)) {
            p->input_active_ = !p->input_active_;
        }
        if (p->input_active_) {
            auto new_text_input = KeyboardInput::Get()->PickTextInput();
            p->input_text_.insert(p->cursor_position_, new_text_input);
            p->cursor_position_ += new_text_input.length();
        }
    }

    void TextOutBox::Render() const {
        if (!p->enabled_)
            return;
        auto used_bounds = p->kbounds;
        if (SceneManager::Get()->GetCurrentScene() == SceneNames::Play)
            used_bounds = used_bounds.Translate(0.0f, -ExperienceBar::kBarHeight);
        Renderer2DImages::Get()->DrawImage("TextOutBoxBack", p->rendid_image_, used_bounds);
        auto max_num_lines = GetMaxNumLines();
        for (auto i = 0; i < max_num_lines; i++) {
            auto text_line_index = static_cast<int>(p->text_lines_.size()) - max_num_lines + i;
            if (text_line_index >= 0) {
                auto line_position_y = used_bounds.y + (i + 1) * p->kTextLineHeight;
                Point2F position = {used_bounds.x + 0.01f, line_position_y};
                TextRenderer::Get()->DrawString(
                    p->rendids_text_lines_.at(i), p->text_lines_.at(text_line_index).text, position,
                    p->text_lines_.at(text_line_index).color);
            }
        }
        auto rect = RectangleF{
            0.0f, used_bounds.y + used_bounds.height - 1.3f * p->kTextLineHeight, p->kbounds.width,
            p->kSplitLineHeight};
        Renderer2DImages::Get()->DrawImage("Wheat", p->rendid_split_line_, rect);
        if (p->input_active_) {
            auto rect_arrow = RectangleF{
                0.0f, used_bounds.y + used_bounds.height - 1.3f * p->kTextLineHeight,
                p->kTextLineHeight, p->kTextLineHeight};
            Renderer2DImages::Get()->DrawImage(
                "CommandLineInputArrow", p->rendid_command_line_input_arrow_, rect_arrow);
            auto position_text =
                rect_arrow.GetPosition().Translate(p->kTextLineHeight, p->kTextLineHeight / 2);
            auto used_text = p->input_text_;
            if (SDL_GetTicks() % 600 < 300)
                used_text.insert(p->cursor_position_, "|");
            else
                used_text.insert(p->cursor_position_, " ");
            TextRenderer::Get()->DrawString(p->rendid_input_text_, used_text, position_text);
        }
    }

    void TextOutBox::Print(std::string_view text, Color textColor) {
        if (!p->enabled_)
            return;
        std::string printed_text = std::string(GetCurrentTime().data()) + "." +
                                   std::to_string(SDL_GetTicks() % 1000) + ") " + text.data();
        p->text_lines_.push_back({printed_text, textColor});
    }

    int TextOutBox::GetMaxNumLines() const {
        return static_cast<int>(p->kbounds.height / p->kTextLineHeight) - 2;
    }
}