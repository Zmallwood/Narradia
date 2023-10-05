//////////////////////////////////////////////////////////////////////
#include "ObjectsCollection.hpp"
#include "Object.hpp"
#include "Tile.hpp"
#include "MapArea.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ObjectsCollection::ObjectsCollection(Tile *parentTile_)
        : parentTile(parentTile_)
    /*///////////////////////////////////////////////////*/ {} // Function

    void ObjectsCollection::CreateAdd(std::string_view objectType, int quantity)
    /*////////////////////////////////////////////////////////////////////////*/ {
        auto newObject = std::make_shared<Object>(objectType, quantity, this);
        trueList.push_back(newObject);
        parentTile->GetParentMapArea()->AddObjectMirror(
            newObject.get(), parentTile->GetCoordinate());
    } // Function

    void ObjectsCollection::CreateAdd(int objectTypeHash, int quantity)
    /*///////////////////////////////////////////////////////////////*/ {
        auto newObject = std::make_shared<Object>(objectTypeHash, quantity, this);
        trueList.push_back(newObject);
        parentTile->GetParentMapArea()->AddObjectMirror(
            newObject.get(), parentTile->GetCoordinate());
    } // Function

    void ObjectsCollection::Add(std::shared_ptr<Object> object)
    /*///////////////////////////////////////////////////////*/ {
        object->SetParentObjectsCollection(this);
        trueList.push_back(object);
        parentTile->GetParentMapArea()->AddObjectMirror(object.get(), parentTile->GetCoordinate());
    } // Function

    void ObjectsCollection::Clear()
    /*///////////////////////////*/ { trueList.clear(); } // Function

    bool ObjectsCollection::Contains(int objectNameHash) const
    /*//////////////////////////////////////////////////////*/ {
        for (auto objectEntry : list)
            if (objectEntry->GetObjectType() == objectNameHash) return true;
        return false;
    } // Function

    void ObjectsCollection::Replace(
        std::shared_ptr<Object> origObject, std::shared_ptr<Object> newObject)
    /*/////////////////////////////////////////////////////////////////////////////////////////////*/
    {
        parentTile->GetParentMapArea()->RemoveObjectMirror(origObject.get());
        for (auto &objectEntry : trueList)
        /********************************/ {
            if (objectEntry == origObject)
            /****************************/ {
                newObject->SetParentObjectsCollection(this);
                objectEntry = newObject;
                return;
            }
        }
        parentTile->GetParentMapArea()->AddObjectMirror(
            newObject.get(), parentTile->GetCoordinate());
    } // Function

    void ObjectsCollection::Remove(Object *object)
    /*//////////////////////////////////////////*/ {
        auto i = 0;
        for (auto &objectEntry : trueList)
        /********************************/ {
            if (objectEntry.get() == object)
            /******************************/ {
                trueList.erase(trueList.begin() + i);
                parentTile->GetParentMapArea()->RemoveObjectMirror(object);
                return;
            }
            i++;
        }
    } // Function

    std::shared_ptr<Object> ObjectsCollection::GetObjectFromRawPtr(Object *object)
    /*//////////////////////////////////////////////////////////////////////////*/ {
        for (auto &objectEntry : list)
            if (objectEntry.get() == object) return objectEntry;
        return nullptr;
    } // Function
}
//////////////////////////////////////////////////////////////////////