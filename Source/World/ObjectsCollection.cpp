#include "ObjectsCollection.hpp"
#include "MapArea.hpp"
#include "Object.hpp"
#include "Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    ObjectsCollection::ObjectsCollection(Tile *parentTile_)
        : kParentTile(parentTile_) {
    }

    void ObjectsCollection::CreateAdd(std::string_view objectType, int quantity) {
        auto new_object = std::make_shared<Object>(objectType, quantity, this);
        true_list_.push_back(new_object);
        kParentTile->GetParentMapArea()->AddObjectMirror(
            new_object.get(), kParentTile->GetCoordinate());
    }

    void ObjectsCollection::CreateAdd(int objectTypeHash, int quantity) {
        auto new_object = std::make_shared<Object>(objectTypeHash, quantity, this);
        true_list_.push_back(new_object);
        kParentTile->GetParentMapArea()->AddObjectMirror(
            new_object.get(), kParentTile->GetCoordinate());
    }

    void ObjectsCollection::Add(std::shared_ptr<Object> object) {
        object->SetParentObjectsCollection(this);
        true_list_.push_back(object);
        kParentTile->GetParentMapArea()->AddObjectMirror(
            object.get(), kParentTile->GetCoordinate());
    }

    void ObjectsCollection::Clear() {
        true_list_.clear();
    }

    bool ObjectsCollection::Contains(int objectNameHash) const {
        for (auto object_entry : list_)
            if (object_entry->GetObjectType() == objectNameHash)
                return true;
        return false;
    }

    void ObjectsCollection::Replace(
        std::shared_ptr<Object> origObject, std::shared_ptr<Object> newObject) {
        kParentTile->GetParentMapArea()->RemoveObjectMirror(origObject.get());
        for (auto &object_entry : true_list_) {
            if (object_entry == origObject) {
                newObject->SetParentObjectsCollection(this);
                object_entry = newObject;
                return;
            }
        }
        kParentTile->GetParentMapArea()->AddObjectMirror(
            newObject.get(), kParentTile->GetCoordinate());
    }

    void ObjectsCollection::Remove(Object *object) {
        auto i = 0;
        for (auto &object_entry : true_list_) {
            if (object_entry.get() == object) {
                true_list_.erase(true_list_.begin() + i);
                kParentTile->GetParentMapArea()->RemoveObjectMirror(object);
                return;
            }
            i++;
        }
    }

    std::shared_ptr<Object> ObjectsCollection::GetObjectFromRawPtr(Object *object) {
        for (auto &object_entry : list_)
            if (object_entry.get() == object)
                return object_entry;
        return nullptr;
    }
}