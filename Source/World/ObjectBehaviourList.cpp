//////////////////////////////////////////////////////////////////////
#include "ObjectBehaviourList.hpp"
#include "ObjectBehaviourEntry.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectBehaviourList::Pimpl
    /*////////////////////////////*/
    {
      public:
        void CreateEntryIfDoesntExist(std::string_view);

        std::map<int, ObjectBehaviourEntry> list;
    };

    ObjectBehaviourList::ObjectBehaviourList()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////////*/
    {
    }

    void ObjectBehaviourList::Initialize()
    /*//////////////////////////////////*/
    {
        AddLabel("ObjectKindling", "Kindling");
        AddLabel("ObjectFishingNet", "Fishing net");
        AddLabel("ObjectFish", "Fish");
        AddLabel("ObjectCookedFish", "Cooked fish");
        AddLabel("ObjectTree1", "Tree type 1");
        AddFlag("ObjectTree1", ObjectBehaviourFlags::Unmovable);
        AddFlag("ObjectTree1", ObjectBehaviourFlags::MovementBlock);
        AddLabel("ObjectTree2", "Tree type 2");
        AddFlag("ObjectTree2", ObjectBehaviourFlags::Unmovable);
        AddFlag("ObjectTree2", ObjectBehaviourFlags::MovementBlock);
        AddLabel("ObjectTallGrass6", "Tall grass");
        AddFlag("ObjectTallGrass6", ObjectBehaviourFlags::Unmovable);
        AddLabel("ObjectWoodWallN", "Wood wall (north)");
        AddFlag("ObjectWoodWallN", ObjectBehaviourFlags::NoShadow);
        AddFlag("ObjectWoodWallN", ObjectBehaviourFlags::MovementBlockN);
        AddLabel("ObjectWoodWallE", "Wood wall (east)");
        AddFlag("ObjectWoodWallE", ObjectBehaviourFlags::NoShadow);
        AddFlag("ObjectWoodWallE", ObjectBehaviourFlags::MovementBlockE);
        AddLabel("ObjectWoodWallS", "Wood wall (south)");
        AddFlag("ObjectWoodWallS", ObjectBehaviourFlags::NoShadow);
        AddFlag("ObjectWoodWallS", ObjectBehaviourFlags::MovementBlockS);
        AddLabel("ObjectWoodWallW", "Wood wall (west)");
        AddFlag("ObjectWoodWallW", ObjectBehaviourFlags::NoShadow);
        AddFlag("ObjectWoodWallW", ObjectBehaviourFlags::MovementBlockW);
        AddLabel("ObjectChampignon", "Champignon");
        AddFlag("ObjectChampignon", ObjectBehaviourFlags::NoShadow);
        AddLabel("ObjectPinkFlower", "Pink flower");
        AddFlag("ObjectPinkFlower", ObjectBehaviourFlags::NoShadow);
        AddLabel("ObjectWoodenBox", "Wooden box");
        SetContainerSlots("ObjectWoodenBox", 8);
        SetContainerSlots("ObjectBurningCampfire", 8);
        AddLabel("ObjectSmallStone", "Small stone");
        AddFlag("ObjectSmallStone", ObjectBehaviourFlags::NoShadow);
        AddLabel("ObjectBranch", "Branch");
        AddFlag("ObjectBranch", ObjectBehaviourFlags::NoShadow);
    }

    void ObjectBehaviourList::AddLabel(std::string_view objectName, std::string_view label)
    /*///////////////////////////////////////////////////////////////////////////////////*/
    {
        p->CreateEntryIfDoesntExist(objectName);
        p->list.at(Hash(objectName.data())).label = label;
    }

    void ObjectBehaviourList::AddFlag(std::string_view objectName, ObjectBehaviourFlags flag)
    /*/////////////////////////////////////////////////////////////////////////////////////*/
    {
        p->CreateEntryIfDoesntExist(objectName);
        p->list.at(Hash(objectName.data())).flags |= static_cast<int>(flag);
    }

    void ObjectBehaviourList::SetContainerSlots(std::string_view objectName, int containerSlots)
    /*////////////////////////////////////////////////////////////////////////////////////////*/
    {
        p->CreateEntryIfDoesntExist(objectName);
        p->list.at(Hash(objectName.data())).containerSlots = containerSlots;
    }

    bool ObjectBehaviourList::IsContainer(int objectTypeHash)
    /*/////////////////////////////////////////////////////*/
    {
        if (p->list.count(objectTypeHash))
            return p->list.at(objectTypeHash).containerSlots > 0;
        return false;
    }

    bool ObjectBehaviourList::HasBehaviourData(int objectTypeHash)
    /*//////////////////////////////////////////////////////////*/
    {
        return p->list.count(objectTypeHash) != 0;
    }

    int ObjectBehaviourList::GetFlags(int objectTypeHash)
    /*/////////////////////////////////////////////////*/
    {
        if (p->list.count(objectTypeHash))
            return p->list.at(objectTypeHash).flags;
        return 0;
    }

    std::string_view ObjectBehaviourList::GetLabel(int objectTypeHash)
    /*//////////////////////////////////////////////////////////////*/
    {
        if (p->list.count(objectTypeHash))
            return p->list.at(objectTypeHash).label;
        return "";
    }

    int ObjectBehaviourList::GetContainerSlots(int objectTypeHash)
    /*//////////////////////////////////////////////////////////*/
    {
        if (p->list.count(objectTypeHash))
            return p->list.at(objectTypeHash).containerSlots;
        return 0;
    }

    void ObjectBehaviourList::Clear()
    /*/////////////////////////////*/
    {
        p->list.clear();
    }

    void ObjectBehaviourList::Pimpl::CreateEntryIfDoesntExist(std::string_view objectName)
    /*//////////////////////////////////////////////////////////////////////////////////*/
    {
        auto objectNameHash = Hash(objectName.data());
        if (list.count(objectNameHash) == 0)
            list.insert({objectNameHash, ObjectBehaviourEntry()});
    }
}
//////////////////////////////////////////////////////////////////////