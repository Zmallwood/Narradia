#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;

    class InteractionMenuEntry
    /*///////////////////*/
    {
      private:
        InteractionMenuEntry(std::string_view, std::function<void(Object *&)>);
        void Render();
        InteractionMenuEntry Copy();
        RectangleF &GetBounds();
        InteractionMenuEntry SetBounds(RectangleF);
        Object *&GetTargetObject();
        InteractionMenuEntry SetTargetObject(Object *);
        void MakeHovered();
        bool IsHovered();
        void ResetHovering();
        const std::string_view &GetLabel();
        const std::function<void(Object *&)> &GetAction();
        static float GetMarginX();
        static float GetMarginY();
        class Pimpl;
        std::shared_ptr<Pimpl> p;
        friend class InteractionMenu;
    };
    
    class InteractionMenu : public Singleton<InteractionMenu>
    /*///////////////////////////////////////////////*/ {
      public:
        InteractionMenu();
        int GetTicksClosed();

      private:
        void Update();
        void Render();
        void AddEntryToMenu(std::string_view, Object * = nullptr);
        void CreateAvailableActions();
        void ConstructMenu();
        void CloseMenu();
        static float GetMarginY();
        class Pimpl;
        std::shared_ptr<Pimpl> p;
        friend class PlayScene;
    };
}