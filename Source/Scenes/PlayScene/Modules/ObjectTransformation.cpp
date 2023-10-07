//////////////////////////////////////////////////////////////////////
#include "ObjectTransformation.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    ObjectTransformation::Update()
    /*//////////////////////////*/
    {
        if (SDL_GetTicks() > ticks_last_update + k_update_speed)
        /******************************************************/
        {
            ticks_last_update = SDL_GetTicks();
            auto map_area = World::Get()->GetCurrentMapArea();
            for (auto &entry : map_area->GetObjectsMirror())
            /**********************************************/
            {
                auto object = entry.first;
                if (object->IsContainer())
                /************************/
                {
                    auto contained_objects = object->GetContainedObjects();
                    if (object->GetObjectType() == Hash("ObjectBurningCampfire"))
                    /***********************************************************/
                    {
                        for (auto &contained_entry : contained_objects)
                        /*********************************************/
                        {
                            auto contained_object = contained_entry.second;
                            if (contained_object->GetObjectType() == Hash("ObjectFish"))
                            /**********************************************************/
                            {
                                contained_object->IncreaseTransformationProgress(0.1f);
                                if (contained_object->TransformationFinished())
                                /*********************************************/
                                {
                                    contained_object->TransformInto("ObjectCookedFish");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////