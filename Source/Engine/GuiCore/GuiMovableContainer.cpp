#include "GuiMovableContainer.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiMovableContainer::Pimpl {
      public:
        RectangleF rel_dragable_area_;
        bool movement_trigger_ = false;
        bool is_moving_ = false;
        Point2F mouse_position_start_moving_;
        Point2F self_position_start_moving_;
    };

    GuiMovableContainer::GuiMovableContainer(
        RectangleF relDragableArea_, Point2F position_, SizeF size_)
        : GuiContainer(position_, size_),
          p(std::make_shared<Pimpl>()) {
        p->rel_dragable_area_ = relDragableArea_;
    }

    void GuiMovableContainer::Update() {
        auto mouse_position_f = GetMousePositionF();
        auto abs_dragable_area = p->rel_dragable_area_.Translate(GetPosition().x, GetPosition().y);
        if (abs_dragable_area.Contains(mouse_position_f)) {
            if (!p->is_moving_ && !p->movement_trigger_) {
                MouseInput::Get()->GetLeftButton().AddFiredAction(
                    "GuiWindowStartMoving" + std::to_string(SDL_GetTicks()),
                    [&]
                    /*****************/ {
                        p->self_position_start_moving_ = GetPosition();
                        p->mouse_position_start_moving_ = GetMousePositionF();
                        p->is_moving_ = true;
                    },
                    SDL_GetTicks());
            }
        }
        if (p->is_moving_) {
            auto dx = mouse_position_f.x - p->mouse_position_start_moving_.x;
            auto dy = mouse_position_f.y - p->mouse_position_start_moving_.y;
            SetPosition(
                {p->self_position_start_moving_.x + dx, p->self_position_start_moving_.y + dy});
            p->movement_trigger_ = true;
            MouseInput::Get()->GetLeftButton().AddReleasedAction(
                "GuiWindowStopMoving" + std::to_string(SDL_GetTicks()),
                [&]
                /*****************/
                {
                    p->is_moving_ = false;
                    p->movement_trigger_ = false;
                },
                SDL_GetTicks());
        }
        GuiContainer::Update();
    }

    void GuiMovableContainer::Render() const {
        GuiContainer::Render();
    }
}