//////////////////////////////////////////////////////////////////////
#include "InteractionMenu.hpp"
#include "ActionRepeat.hpp"
#include "Engine/Core/Graphics/Rendering/Renderer2DSolidColors.hpp"
#include "Engine/Core/Graphics/Rendering/Text/TextRenderer.hpp"
#include "Engine/Core/Input/MouseInput.hpp"
#include "Engine/Core/TextOutBox.hpp"
#include "Engine/GuiCore/GuiWindowObjectSlot.hpp"
#include "Engine/GuiCore/SceneGui.hpp"
#include "Scenes/PlayScene/Gui/InventoryGui.hpp"
#include "Scenes/PlayScene/Gui/OpenContainerGui.hpp"
#include "Scenes/PlayScene/Gui/SplitStackGui.hpp"
#include "Scenes/PlayScene/Modules/MobTargeting.hpp"
#include "Scenes/PlayScene/ObjectHandling.hpp"
#include "Scenes/PlayScene/PlayScene.hpp"
#include "Scenes/SharedSceneModules/MouseRotation.hpp"
#include "Scenes/SharedSceneModules/TileHovering.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/ObjectsCollection.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class InteractionMenuEntry::Pimpl
    /*/////////////////////////////*/
    {
      public:
        static constexpr float k_margin_x = 0.003f;
        std::function<void(Object *&)> action;
        std::string_view label;
        RenderId id_label;
        RectangleF bounds;
        bool hovered = false;
        Object *targeted_object = nullptr;
    };

    InteractionMenuEntry::InteractionMenuEntry(
        std::string_view init_label, std::function<void(Object *&)> init_action)
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////////////////////////////////*/
    {
        p->label = init_label;
        p->action = init_action;
        p->id_label = TextRenderer::Get()->NewString();
    }

    void
    InteractionMenuEntry::Render()
    /*//////////////////////////*/
    {
        TextRenderer::Get()->DrawString(
            p->id_label, p->label, p->bounds.GetPosition().Translate(GetMarginX(), GetMarginY()));
        ResetHovering();
    }

    auto
    InteractionMenuEntry::Copy() -> InteractionMenuEntry
    /*////////////////////////////////////////////////*/
    {
        return *this;
    }

    RectangleF &
    InteractionMenuEntry::GetBounds()
    /*/////////////////////////////*/
    {
        return p->bounds;
    }

    auto
    InteractionMenuEntry::SetBounds(RectangleF new_bounds) -> InteractionMenuEntry
    /*//////////////////////////////////////////////////////////////////////////*/
    {
        p->bounds = new_bounds;
        return *this;
    }

    auto
    InteractionMenuEntry::SetTargetObject(Object *object) -> InteractionMenuEntry
    /*/////////////////////////////////////////////////////////////////////////*/
    {
        p->targeted_object = object;
        return *this;
    }

    void
    InteractionMenuEntry::MakeHovered()
    /*///////////////////////////////*/
    {
        p->hovered = true;
    }

    bool
    InteractionMenuEntry::IsHovered()
    /*/////////////////////////////*/
    {
        return p->hovered;
    }

    void
    InteractionMenuEntry::ResetHovering()
    /*/////////////////////////////////*/
    {
        p->hovered = false;
    }

    Object *&
    InteractionMenuEntry::GetTargetObject()
    /*///////////////////////////////////*/
    {
        return p->targeted_object;
    }

    float
    InteractionMenuEntry::GetMarginX()
    /*//////////////////////////////*/
    {
        return Pimpl::k_margin_x;
    }

    float
    InteractionMenuEntry::GetMarginY()
    /*//////////////////////////////*/
    {
        return ConvertWidthToHeight(Pimpl::k_margin_x);
    }

    const std::string_view &
    InteractionMenuEntry::GetLabel()
    /*////////////////////////////*/
    {
        return p->label;
    }

    const std::function<void(Object *&)> &
    InteractionMenuEntry::GetAction()
    /*//////////////////////////////////*/
    {
        return p->action;
    }

    class InteractionMenu::Pimpl
    /*////////////////////////*/
    {
      public:
        static constexpr float k_menu_width = 0.1f;
        static constexpr float k_line_height = 0.02f;
        static constexpr float k_margin_x = 0.003f;
        bool shown = false;
        std::map<std::string_view, InteractionMenuEntry> available_menu_entries;
        Point2F position;
        std::vector<InteractionMenuEntry> current_menu_entries;
        RenderId id_menu_back;
        RenderId id_menu_title;
        RenderId id_hovered_entry_back;
        Point2 clicked_tile = {-1, -1};
        int ticks_closed = SDL_GetTicks();
    };

    InteractionMenu::InteractionMenu()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
        p->id_menu_back = Renderer2DSolidColors::Get()->NewRectangle();
        p->id_hovered_entry_back = Renderer2DSolidColors::Get()->NewRectangle();
        p->id_menu_title = TextRenderer::Get()->NewString();
        CreateAvailableActions();
    }

    float
    InteractionMenu::GetMarginY()
    /*/////////////////////////*/
    {
        return ConvertWidthToHeight(Pimpl::k_margin_x);
    }

    void
    InteractionMenu::CreateAvailableActions()
    /*/////////////////////////////////////*/
    {
        p->available_menu_entries = {
#include "MenuActions/CreateActions/CreateStoneAxe.inc.cpp"
#include "MenuActions/CreateActions/CreateWoodLog.inc.cpp"
#include "MenuActions/CreateActions/CreateWoodPlank.inc.cpp"
#include "MenuActions/CreateActions/CreateWoodWallE.inc.cpp"
#include "MenuActions/CreateActions/CreateWoodWallN.inc.cpp"
#include "MenuActions/CreateActions/CreateWoodWallS.inc.cpp"
#include "MenuActions/CreateActions/CreateWoodWallW.inc.cpp"
#include "MenuActions/CreateActions/CreateWoodenBox.inc.cpp"
#include "MenuActions/CreateActions/MineCaveEntrance.inc.cpp"
#include "MenuActions/ObjectActions/ChopDownTree.inc.cpp"
#include "MenuActions/ObjectActions/CreateBurningCampfire.inc.cpp"
#include "MenuActions/ObjectActions/CutGrass.inc.cpp"
#include "MenuActions/ObjectActions/EatItem.inc.cpp"
#include "MenuActions/ObjectActions/OpenContainer.inc.cpp"
#include "MenuActions/ObjectActions/SplitStack.inc.cpp"
#include "MenuActions/TileActions/Fish.inc.cpp"
#include "MenuActions/TileActions/Forage.inc.cpp"
#include "MenuActions/TileActions/LayCobblestone.inc.cpp"
#include "MenuActions/TileActions/LayWoodFloor.inc.cpp"
#include "MenuActions/TileActions/SetWoodRoof.inc.cpp"
        };
    }

    void
    InteractionMenu::Update()
    /*/////////////////////*/
    {
        auto mouse_position = GetMousePositionF();
        if (p->shown)
        /***********/
        {
            for (auto &menu_entry : p->current_menu_entries)
            /**********************************************/
            {
                if (menu_entry.GetBounds().Contains(mouse_position))
                /**************************************************/
                {
                    MouseInput::Get()->GetLeftButton().AddFiredAction(
                        "RClickMenuExecuteAction" + std::string(menu_entry.GetLabel()),
                        [&]
                        /************************************/
                        {
                            if (menu_entry.GetBounds().Contains(GetMousePositionF()))
                            /*******************************************************/
                            {
                                menu_entry.GetAction()(menu_entry.GetTargetObject());
                                ActionRepeat::Get()->SetRepeatingAction(
                                    [&] { menu_entry.GetAction()(menu_entry.GetTargetObject()); });
                                CloseMenu();
                            }
                        },
                        10);

                    menu_entry.MakeHovered();
                }
            }
            MouseInput::Get()->GetLeftButton().AddFiredAction(
                "RClickMenuHideMenu", [&]
                /***********************/
                {
                    p->shown = false;
                    p->ticks_closed = SDL_GetTicks();
                });
        }
        else if (!p->shown && nullptr == MobTargeting::Get()->GetTargetedMob())
        /********************************************************************/
        {
            MouseInput::Get()->GetRightButton().AddReleasedAction(
                "RClickMenuShowMenu",
                [=, this]
                /*******/
                {
                    if (MouseInput::Get()->GetRightButton().ClickDurationPeekResult() <
                        MouseButton::GetDefaultClickSpeed())
                    /**************************************************************/
                    {
                        ConstructMenu();
                    }
                },
                3, 0, true);
        }
    }

    void
    InteractionMenu::CloseMenu()
    /*////////////////////////*/
    {
        p->ticks_closed = SDL_GetTicks();
        p->shown = false;
    }

    void
    InteractionMenu::AddEntryToMenu(std::string_view actionName, Object *object)
    /*////////////////////////////////////////////////////////////////////////*/
    {
        auto entry_index = p->current_menu_entries.size();
        auto entry_bounds = RectangleF{
            p->position.x,
            p->position.y + 2 * GetMarginY() + Pimpl::k_line_height +
                entry_index * (Pimpl::k_line_height + GetMarginY()),
            Pimpl::k_menu_width, Pimpl::k_line_height};
        p->current_menu_entries.push_back(p->available_menu_entries.at(actionName)
                                              .Copy()
                                              .SetBounds(entry_bounds)
                                              .SetTargetObject(object));
    }

    void
    InteractionMenu::ConstructMenu()
    /*////////////////////////////*/
    {
        auto mouse_position = GetMousePositionF();
        auto block_tile_hovering = TileHovering::Get();
        auto player = Player::Get();
        auto player_world_area_position = player->GetWorldAreaPos();
        auto map_area = World::Get()->GetMapAreaAtZLevel(player_world_area_position.z);
        auto tile = map_area->GetTile(block_tile_hovering->hoveredTile);
        auto gui_window_inventory = InventoryGui::Get();
        if (tile == nullptr && GuiWindowObjectSlot::hoveredObject == nullptr)
            return;
        if (tile->GetMob() == nullptr || GuiWindowObjectSlot::hoveredObject)
        /******************************************************************/
        {
            p->shown = true;
            MouseInput::Get()->GetLeftButton().AddFiredAction(
                "RClickMenuHideOnMouseLeftClick", [&] { CloseMenu(); }, 6);
            p->position = mouse_position;
            p->current_menu_entries.clear();
            AddEntryToMenu("MineCaveEntrance");
            auto fn_create_entries_for_object = [&](Object *object)
            /*****************************************************/
            {
                if (!object)
                    return;
                if (object->GetObjectType() == Hash("ObjectRedApple") ||
                    object->GetObjectType() == Hash("ObjectCookedFi.hpp"))
                /********************************************************/
                {
                    AddEntryToMenu("EatItem", object);
                }
                else if (
                    object->GetObjectType() == Hash("ObjectTree1") ||
                    object->GetObjectType() == Hash("ObjectTree2"))
                /***************************************************/
                {
                    AddEntryToMenu("ChopDownTree", object);
                }
                else if (object->GetObjectType() == Hash("ObjectFelledTree"))
                /*************************************************************/
                {
                    AddEntryToMenu("CreateWoodLog", object);
                }
                else if (object->GetObjectType() == Hash("ObjectWoodLog"))
                /**********************************************************/
                {
                    AddEntryToMenu("CreateWoodPlank", object);
                }
                else if (object->GetObjectType() == Hash("ObjectKindling"))
                /***********************************************************/
                {
                    AddEntryToMenu("CreateBurningCampfire", object);
                }
                else if (object->GetObjectType() == Hash("ObjectTallGrass6"))
                /*************************************************************/
                {
                    AddEntryToMenu("CutGrass", object);
                    AddEntryToMenu("Forage");
                }
                else if (object->GetObjectType() == Hash("ObjectWoodPlank"))
                /************************************************************/
                {
                    AddEntryToMenu("CreateWoodenBox", object);
                }
                else if (
                    object->GetObjectType() == Hash("ObjectSmallStone") ||
                    object->GetObjectType() == Hash("ObjectBranch"))
                /********************************************************/
                {
                    AddEntryToMenu("CreateStoneAxe", object);
                }
                if (ObjectBehaviourList::Get()->HasBehaviourData(object->GetObjectType()) &&
                    ObjectBehaviourList::Get()->IsContainer(object->GetObjectType()) > 0)
                /***********************************************************************/
                {
                    AddEntryToMenu("OpenContainer", object);
                }
                if (object->GetQuantity() > 1)
                    AddEntryToMenu("SplitStack", object);
            };
            if (GuiWindowObjectSlot::hoveredObject)
            /*************************************/
            {
                fn_create_entries_for_object(GuiWindowObjectSlot::hoveredObject);
                return;
            }
            else
            /**/
            {
                for (auto object : tile->GetObjects().list)
                    fn_create_entries_for_object(object.get());
            }
            if (tile->GetGroundType() != Hash("GroundWater") &&
                tile->GetObjects().Contains(Hash("ObjectTallGrass6")) == false)
            /*****************************************************************/
            {
                AddEntryToMenu("LayWoodFloor");
                AddEntryToMenu("LayCobblestone");
            }
            if (tile->GetGroundType() != Hash("GroundWater"))
            /***********************************************/
            {
                AddEntryToMenu("CreateWoodWallN");
                AddEntryToMenu("CreateWoodWallE");
                AddEntryToMenu("CreateWoodWallS");
                AddEntryToMenu("CreateWoodWallW");
            }
            if (tile->GetGroundType() != Hash("GroundWater"))
                AddEntryToMenu("SetWoodRoof");
            if (tile->GetGroundType() == Hash("GroundWater"))
                AddEntryToMenu("Fish");
            p->clicked_tile = block_tile_hovering->hoveredTile;
        }
        else
        /**/
        {
            CloseMenu();
        }
    }

    void
    InteractionMenu::Render()
    /*/////////////////////*/
    {
        if (!p->shown)
            return;
        auto rectangle = RectangleF{
            p->position.x, p->position.y, Pimpl::k_menu_width,
            (p->current_menu_entries.size() + 1) * (Pimpl::k_line_height + GetMarginY())};
        Renderer2DSolidColors::Get()->FillRectangle(
            p->id_menu_back, rectangle, Colors::alphaElegantBlue);
        TextRenderer::Get()->DrawString(
            p->id_menu_title,
            "Actions:", {rectangle.x + Pimpl::k_margin_x, rectangle.y + GetMarginY()},
            Colors::yellow);
        for (auto &entry : p->current_menu_entries)
        /*****************************************/
        {
            if (entry.IsHovered())
            /********************/
            {
                Renderer2DSolidColors::Get()->FillRectangle(
                    p->id_hovered_entry_back, entry.GetBounds(), Colors::alphaMildBlue);
            }
            entry.Render();
        }
    }

    int
    InteractionMenu::GetTicksClosed()
    /*/////////////////////////////*/
    {
        return p->ticks_closed;
    }
}