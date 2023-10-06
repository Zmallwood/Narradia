//////////////////////////////////////////////////////////////////////
#include "ObjectsCollection.hpp"
#include "MapArea.hpp"
#include "Object.hpp"
#include "Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ObjectsCollection::ObjectsCollection(Tile *parentTile_)
        : parentTile(parentTile_)
    /*///////////////////////////////////////////////////*/
    {
    }

    void ObjectsCollection::CreateAdd(std::string_view objectType, int quantity)
    /*////////////////////////////////////////////////////////////////////////*/
    {
        auto newObject = std::make_shared<Object>(objectType, quantity, this);
        trueList.push_back(newObject);
        parentTile->GetParentMapArea()->AddObjectMirror(
            newObject.get(), parentTile->GetCoordinate());
    }

    void ObjectsCollection::CreateAdd(int objectTypeHash, int quantity)
    /*///////////////////////////////////////////////////////////////*/
    {
        auto newObject = std::make_shared<Object>(objectTypeHash, quantity, this);
        trueList.push_back(newObject);
        parentTile->GetParentMapArea()->AddObjectMirror(
            newObject.get(), parentTile->GetCoordinate());
    }

    void ObjectsCollection::Add(std::shared_ptr<Object> object)
    /*///////////////////////////////////////////////////////*/
    {
        object->SetParentObjectsCollection(this);
        trueList.push_back(object);
        parentTile->GetParentMapArea()->AddObjectMirror(object.get(), parentTile->GetCoordinate());
    }

    void ObjectsCollection::Clear()
    /*///////////////////////////*/
    {
        trueList.clear();
    }

    bool ObjectsCollection::Contains(int objectNameHash) const
    /*//////////////////////////////////////////////////////*/
    {
        for (auto objectEntry : list)
            if (objectEntry->GetObjectType() == objectNameHash)
                return true;
        return false;
    }

    void ObjectsCollection::Replace(
        std::shared_ptr<Object> origObject, std::shared_ptr<Object> newObject)
    /*/////////////////////////////////////////////////////////////////////////////////////////////*/
    {
        parentTile->GetParentMapArea()->RemoveObjectMirror(origObject.get());
        for (auto &objectEntry : trueList)
        /********************************/
        {
            if (objectEntry == origObject)
            /****************************/
            {
                newObject->SetParentObjectsCollection(this);
                objectEntry = newObject;
                return;
            }
        }
        parentTile->GetParentMapArea()->AddObjectMirror(
            newObject.get(), parentTile->GetCoordinate());
    }

    void ObjectsCollection::Remove(Object *object)
    /*//////////////////////////////////////////*/
    {
        auto i = 0;
        for (auto &objectEntry : trueList)
        /********************************/
        {
            if (objectEntry.get() == object)
            /******************************/
            {
                trueList.erase(trueList.begin() + i);
                parentTile->GetParentMapArea()->RemoveObjectMirror(object);
                return;
            }
            i++;
        }
    }

    std::shared_ptr<Object> ObjectsCollection::GetObjectFromRawPtr(Object *object)
    /*//////////////////////////////////////////////////////////////////////////*/
    {
        for (auto &objectEntry : list)
            if (objectEntry.get() == object)
                return objectEntry;
        return nullptr;
    }
}
//////////////////////////////////////////////////////////////////////