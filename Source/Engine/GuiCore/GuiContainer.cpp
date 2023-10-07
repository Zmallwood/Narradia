#include "GuiContainer.hpp"
#include "GuiButton.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiContainer::Pimpl
    /*/////////////////////*/
    {
      public:
        Point2F position;
        SizeF size;
        std::vector<std::shared_ptr<GuiButton>> guiButtons;
        std::vector<std::shared_ptr<GuiComponent>> guiComponents;
    };

    GuiContainer::GuiContainer(Point2F position_, SizeF size_)
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////////////////////*/
    {
        p->position = position_;
        p->size = size_;
    }

    RectangleF
    GuiContainer::GetBounds() const
    /*///////////////////////////*/
    {
        return {p->position.x, p->position.y, p->size.width, p->size.height};
    }

    const SizeF &
    GuiContainer::GetSize() const
    /*/////////////////////////*/
    {
        return p->size;
    }

    const Point2F &
    GuiContainer::GetPosition() const
    /*/////////////////////////////*/
    {
        return p->position;
    }

    void
    GuiContainer::SetPosition(Point2F newPosition)
    /*//////////////////////////////////////////*/
    {
        p->position = newPosition;
    }

    void
    GuiContainer::Update()
    /*//////////////////*/
    {
        for (auto &guiButton : p->guiButtons)
            guiButton->Update();
        for (auto &guiComponent : p->guiComponents)
            guiComponent->Update();
    }

    void
    GuiContainer::Render() const
    /*////////////////////////*/
    {
        for (auto &guiButton : p->guiButtons)
            guiButton->Render();
        for (auto &guiComponent : p->guiComponents)
            guiComponent->Render();
    }

    void
    GuiContainer::AddGuiButton(
        const std::string_view &text, RectangleF bounds, std::function<void()> action,
        const std::string_view &imageName, const std::string_view &imageNameHovered)
    /*//////////////////////////////////////////////////////////////////////////////*/
    {
        std::shared_ptr<GuiButton> newGuiButton;
        if (imageName != "" && imageNameHovered != "")
            newGuiButton = std::make_shared<GuiButton>(
                text, bounds, action, this, imageName, imageNameHovered);
        else
            newGuiButton = std::make_shared<GuiButton>(text, bounds, action, this);
        p->guiButtons.push_back(newGuiButton);
    }

    void
    GuiContainer::AddGuiComponent(std::shared_ptr<GuiComponent> newComponent)
    /*/////////////////////////////////////////////////////////////////////*/
    {
        p->guiComponents.push_back(newComponent);
    }
}