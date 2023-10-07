#include "SceneGui.hpp"
#include "GuiButton.hpp"
#include "GuiComponent.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SceneGui::Pimpl
    /*/////////////////*/
    {
      public:
        std::vector<std::shared_ptr<GuiComponent>> gui_components_;
        std::vector<std::shared_ptr<GuiButton>> gui_buttons_;
    };

    SceneGui::SceneGui()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    SceneGui::AddGuiButton(
        std::string_view text, RectangleF bounds, std::function<void()> action,
        std::string_view imageName, std::string_view imageNameHovered)
    /*///////////////////////////////////////////////////////////////////////*/
    {
        std::shared_ptr<GuiButton> new_gui_button;
        if (imageName != "" && imageNameHovered != "")
            new_gui_button = std::make_shared<GuiButton>(
                text, bounds, action, nullptr, imageName, imageNameHovered);
        else
            new_gui_button = std::make_shared<GuiButton>(text, bounds, action);
        p->gui_buttons_.push_back(new_gui_button);
    }

    void
    SceneGui::Update()
    /*//////////////*/
    {
        for (auto &gui_button : p->gui_buttons_)
            gui_button->Update();
        for (auto &gui_component : p->gui_components_)
            gui_component->Update();
    }

    void
    SceneGui::Render() const
    /*////////////////////*/
    {
        Log();
        for (auto &gui_button : p->gui_buttons_ | std::views::reverse)
        /*********************************************************/
        {
            gui_button->Render();
        }
        Log();
        for (auto &gui_component : p->gui_components_ | std::views::reverse)
        /***************************************************************/
        {
            gui_component->Render();
        }
    }

    void
    SceneGui::AddGuiComponent(std::shared_ptr<GuiComponent> newComponent)
    /*/////////////////////////////////////////////////////////////////*/
    {
        p->gui_components_.push_back(newComponent);
    }

    void
    SceneGui::RemoveGuiComponent(GuiComponent *component)
    /*/////////////////////////////////////////////////*/
    {
        auto i = 0;
        for (auto &entry : p->gui_components_)
        /**********************************/
        {
            if (entry.get() == component)
            /***************************/
            {
                p->gui_components_.erase(p->gui_components_.begin() + i);
                break;
            }
            i++;
        }
    }

    auto
    SceneGui::GetGuiComponents() const -> const std::vector<std::shared_ptr<GuiComponent>> &
    /*////////////////////////////////////////////////////////////////////////////////////*/
    {
        return p->gui_components_;
    }
}