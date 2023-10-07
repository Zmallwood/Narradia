#include "GuiMovableContainer.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class GuiMovableContainer::Pimpl
    /*////////////////////////////*/
    {
      public:
        std::shared_ptr<Pimpl> p;
        RectangleF relDragableArea;
        bool movementTrigger = false;
        bool isMoving = false;
        Point2F mousePosStartMoving;
        Point2F posStartMoving;
    };

    GuiMovableContainer::GuiMovableContainer(
        RectangleF relDragableArea_, Point2F position_, SizeF size_)
        : GuiContainer(position_, size_),
          p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////////////////////*/
    {
        p->relDragableArea = relDragableArea_;
    }

    void
    GuiMovableContainer::Update()
    /*/////////////////////////*/
    {
        auto mousePosF = GetMousePositionF();
        auto absDragableArea = p->relDragableArea.Translate(GetPosition().x, GetPosition().y);
        if (absDragableArea.Contains(mousePosF))
        /**************************************/
        {
            if (!p->isMoving && !p->movementTrigger)
            /**************************************/
            {
                MouseInput::Get()->GetLeftButton().AddFiredAction(
                    "GuiWindowStartMoving" + std::to_string(SDL_GetTicks()),
                    [&]
                    /*****************/ {
                        p->posStartMoving = GetPosition();
                        p->mousePosStartMoving = GetMousePositionF();
                        p->isMoving = true;
                    },
                    SDL_GetTicks());
            }
        }
        if (p->isMoving)
        /**************/
        {
            auto dx = mousePosF.x - p->mousePosStartMoving.x;
            auto dy = mousePosF.y - p->mousePosStartMoving.y;
            SetPosition({p->posStartMoving.x + dx, p->posStartMoving.y + dy});
            p->movementTrigger = true;
            MouseInput::Get()->GetLeftButton().AddReleasedAction(
                "GuiWindowStopMoving" + std::to_string(SDL_GetTicks()),
                [&]
                /*****************/
                {
                    p->isMoving = false;
                    p->movementTrigger = false;
                },
                SDL_GetTicks());
        }
        GuiContainer::Update();
    }

    void
    GuiMovableContainer::Render() const
    /*///////////////////////////////*/
    {
        GuiContainer::Render();
    }
}