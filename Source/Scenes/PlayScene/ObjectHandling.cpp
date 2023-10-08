#include "ObjectHandling.hpp"
#include "Modules/ObjectMoving.hpp"
#include "World/Actors/Player.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void RemoveObjectMirror(Object *&object) {
        if (ObjectMoving::Get()->object_in_air_.get() == object)
            ObjectMoving::Get()->object_in_air_ = nullptr;
        for (auto &inventory_object : Player::Get()->data_.inventory_.objects_) {
            if (inventory_object.second.get() == object) {
                Player::Get()->data_.inventory_.objects_.erase(inventory_object.first);
                object = nullptr;
                return;
            }
            for (auto &entry : inventory_object.second->GetContainedObjects()) {
                if (entry.second.get() == object) {
                    inventory_object.second->RemoveContainedObject(entry.first);
                    object = nullptr;
                    return;
                }
            }
        }
        auto map_area = World::Get()->GetCurrentMapArea();
        for (auto world_object : map_area->GetObjectsMirror()) {
            if (world_object.first == object) {
                auto coord = world_object.second;
                auto tile = map_area->GetTile(coord);
                tile->RemoveObject(object);
                object = nullptr;
                return;
            }
            for (auto &entry : world_object.first->GetContainedObjects()) {
                if (entry.second.get() == object) {
                    world_object.first->RemoveContainedObject(entry.first);
                    object = nullptr;
                    return;
                }
            }
        }
    }

    std::shared_ptr<Object> MoveObject(Object *object) {
        if (ObjectMoving::Get()->object_in_air_.get() == object) {
            auto result = ObjectMoving::Get()->object_in_air_;
            ObjectMoving::Get()->object_in_air_ = nullptr;
            return result;
        }
        for (auto &inventory_object : Player::Get()->data_.inventory_.objects_) {
            if (inventory_object.second.get() == object) {
                auto result = inventory_object.second;
                Player::Get()->data_.inventory_.objects_.erase(inventory_object.first);
                return result;
            }
            for (auto &entry : inventory_object.second->GetContainedObjects()) {
                if (entry.second.get() == object) {
                    auto result = entry.second;
                    inventory_object.second->RemoveContainedObject(entry.first);
                    return result;
                }
            }
        }
        auto map_area = World::Get()->GetCurrentMapArea();
        for (auto world_object : map_area->GetObjectsMirror()) {
            if (world_object.first == object) {
                auto coord = world_object.second;
                auto tile = map_area->GetTile(coord);
                auto result = tile->GetObjectFromRawPtr(object);
                tile->RemoveObject(object);
                return result;
            }
            for (auto &entry : world_object.first->GetContainedObjects()) {
                if (entry.second.get() == object) {
                    auto result = entry.second;
                    world_object.first->RemoveContainedObject(entry.first);
                    return result;
                }
            }
        }
        return nullptr;
    }
};