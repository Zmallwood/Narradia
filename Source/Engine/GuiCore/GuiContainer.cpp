#include "GuiContainer.hpp"
#include "GuiButton.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiContainer::Pimpl
    /*/////////////////////*/
    {
      public:
        Point2F position_;
        SizeF size_;
        std::vector<std::shared_ptr<GuiButton>> gui_buttons_;
        std::vector<std::shared_ptr<GuiComponent>> gui_components_;
    };

    GuiContainer::GuiContainer(Point2F position_, SizeF size_)
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////////////////////*/
    {
        p->position_ = position_;
        p->size_ = size_;
    }

    RectangleF
    GuiContainer::GetBounds() const
    /*///////////////////////////*/
    {
        return {p->position_.x, p->position_.y, p->size_.width, p->size_.height};
    }

    const SizeF &
    GuiContainer::GetSize() const
    /*/////////////////////////*/
    {
        return p->size_;
    }

    const Point2F &
    GuiContainer::GetPosition() const
    /*/////////////////////////////*/
    {
        return p->position_;
    }

    void
    GuiContainer::SetPosition(Point2F newPosition)
    /*//////////////////////////////////////////*/
    {
        p->position_ = newPosition;
    }

    void
    GuiContainer::Update()
    /*//////////////////*/
    {
        for (auto &gui_button : p->gui_buttons_)
            gui_button->Update();
        for (auto &gui_component : p->gui_components_)
            gui_component->Update();
    }

    void
    GuiContainer::Render() const
    /*////////////////////////*/
    {
        for (auto &gui_button : p->gui_buttons_)
            gui_button->Render();
        for (auto &gui_component : p->gui_components_)
            gui_component->Render();
    }

    void
    GuiContainer::AddGuiButton(
        const std::string_view &text, RectangleF bounds, std::function<void()> action,
        const std::string_view &imageName, const std::string_view &imageNameHovered)
    /*//////////////////////////////////////////////////////////////////////////////*/
    {
        std::shared_ptr<GuiButton> new_gui_button;
        if (imageName != "" && imageNameHovered != "")
            new_gui_button = std::make_shared<GuiButton>(
                text, bounds, action, this, imageName, imageNameHovered);
        else
            new_gui_button = std::make_shared<GuiButton>(text, bounds, action, this);
        p->gui_buttons_.push_back(new_gui_button);
    }

    void
    GuiContainer::AddGuiComponent(std::shared_ptr<GuiComponent> newComponent)
    /*/////////////////////////////////////////////////////////////////////*/
    {
        p->gui_components_.push_back(newComponent);
    }
}