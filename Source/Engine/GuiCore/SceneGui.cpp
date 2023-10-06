//////////////////////////////////////////////////////////////////////
#include "SceneGui.hpp"
#include "GuiButton.hpp"
#include "GuiComponent.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class SceneGui::Pimpl
    /*////////////////*/
    {
      public:
        std::vector<std::shared_ptr<GuiComponent>> guiComponents;
        std::vector<std::shared_ptr<GuiButton>> guiButtons;
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
        std::shared_ptr<GuiButton> newGuiButton;
        if (imageName != "" && imageNameHovered != "")
            newGuiButton = std::make_shared<GuiButton>(
                text, bounds, action, nullptr, imageName, imageNameHovered);
        else
            newGuiButton = std::make_shared<GuiButton>(text, bounds, action);
        p->guiButtons.push_back(newGuiButton);
    }

    void
    SceneGui::Update()
    /*//////////////////*/
    {
        for (auto &guiButton : p->guiButtons)
            guiButton->Update();
        for (auto &guiComponent : p->guiComponents)
            guiComponent->Update();
    }

    void
    SceneGui::Render() const
    /*////////////////////////*/
    {
        Log();
        for (auto &guiButton : p->guiButtons | std::views::reverse)
        /*********************************************************/
        {
            guiButton->Render();
        }
        Log();
        for (auto &guiComponent : p->guiComponents | std::views::reverse)
        /***************************************************************/
        {
            guiComponent->Render();
        }
    }

    void
    SceneGui::AddGuiComponent(std::shared_ptr<GuiComponent> newComponent)
    /*/////////////////////////////////////////////////////////////////////*/
    {
        p->guiComponents.push_back(newComponent);
    }

    void
    SceneGui::RemoveGuiComponent(GuiComponent *component)
    /*/////////////////////////////////////////////////////*/
    {
        auto i = 0;
        for (auto &entry : p->guiComponents)
        /**********************************/
        {
            if (entry.get() == component)
            /***************************/
            {
                p->guiComponents.erase(p->guiComponents.begin() + i);
                break;
            }
            i++;
        }
    }

    auto
    SceneGui::GetGuiComponents() const -> const std::vector<std::shared_ptr<GuiComponent>> &
    /*////////////////////////////////////////////////////////////////////////////////////////*/
    {
        return p->guiComponents;
    }
}
//////////////////////////////////////////////////////////////////////