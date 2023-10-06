//////////////////////////////////////////////////////////////////////
#include "ObjectHandling.hpp"
#include "Modules/ObjectMoving.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/Player.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    RemoveObjectMirror(Object *&object)
    /*////////////////////////////////////*/
    {
        if (ObjectMoving::Get().objectInAir.get() == object)
            ObjectMoving::Get().objectInAir = nullptr;
        for (auto &inventoryObject : Player::Get().data.inventory.objects)
        /****************************************************************/
        {
            if (inventoryObject.second.get() == object)
            /*****************************************/
            {
                Player::Get().data.inventory.objects.erase(inventoryObject.first);
                object = nullptr;
                return;
            }
            for (auto &entry : inventoryObject.second->GetContainedObjects())
            /***************************************************************/
            {
                if (entry.second.get() == object)
                /*******************************/
                {
                    inventoryObject.second->RemoveContainedObject(entry.first);
                    object = nullptr;
                    return;
                }
            }
        }
        auto mapArea = World::Get().GetCurrentMapArea();
        for (auto worldObject : mapArea->GetObjectsMirror())
        /**************************************************/
        {
            if (worldObject.first == object)
            /******************************/
            {
                auto coord = worldObject.second;
                auto tile = mapArea->GetTile(coord);
                tile->RemoveObject(object);
                object = nullptr;
                return;
            }
            for (auto &entry : worldObject.first->GetContainedObjects())
            /**********************************************************/
            {
                if (entry.second.get() == object)
                /*******************************/
                {
                    worldObject.first->RemoveContainedObject(entry.first);
                    object = nullptr;
                    return;
                }
            }
        }
    }

    std::shared_ptr<Object>
    MoveObject(Object *object)
    /*//////////////////////////////////////////////*/
    {
        if (ObjectMoving::Get().objectInAir.get() == object)
        /**************************************************/
        {
            auto result = ObjectMoving::Get().objectInAir;
            ObjectMoving::Get().objectInAir = nullptr;
            return result;
        }
        for (auto &inventoryObject : Player::Get().data.inventory.objects)
        /****************************************************************/
        {
            if (inventoryObject.second.get() == object)
            /*****************************************/
            {
                auto result = inventoryObject.second;
                Player::Get().data.inventory.objects.erase(inventoryObject.first);
                return result;
            }
            for (auto &entry : inventoryObject.second->GetContainedObjects())
            /***************************************************************/
            {
                if (entry.second.get() == object)
                /*******************************/
                {
                    auto result = entry.second;
                    inventoryObject.second->RemoveContainedObject(entry.first);
                    return result;
                }
            }
        }
        auto mapArea = World::Get().GetCurrentMapArea();
        for (auto worldObject : mapArea->GetObjectsMirror())
        /**************************************************/
        {
            if (worldObject.first == object)
            /******************************/
            {
                auto coord = worldObject.second;
                auto tile = mapArea->GetTile(coord);
                auto result = tile->GetObjectFromRawPtr(object);
                tile->RemoveObject(object);
                return result;
            }
            for (auto &entry : worldObject.first->GetContainedObjects())
            /**********************************************************/
            {
                if (entry.second.get() == object)
                /*******************************/
                {
                    auto result = entry.second;
                    worldObject.first->RemoveContainedObject(entry.first);
                    return result;
                }
            }
        }
        return nullptr;
    }
};
//////////////////////////////////////////////////////////////////////