#include "Object.hpp"
#include "ObjectBehaviourList.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object::Pimpl
    /*///////////////*/
    {
      public:
        int object_type_ = 0;
        int quantity_ = 1;
        float work_progress_ = 0.0f;
        std::map<int, std::shared_ptr<Object>> contained_objects_;
        float model_scaling_ = 1.0f;
        float model_rotation_ = 0.0f;
        float red_variation_ = 1.0f;
        float green_variation_ = 1.0f;
        float blue_variation_ = 1.0f;
        float transformation_progress_ = 0.0f;
        ObjectsCollection *parent_objects_collection_ = nullptr;
    };

    Object::Object(
        std::string_view objectTypeName, int quantity_, ObjectsCollection *parentObjectsCollection_)
        : Object(Hash(objectTypeName.data()), quantity_, parentObjectsCollection_)
    /*////////////////////////////////////////////////////////////////////////////////////////////*/
    {
    }

    Object::Object(int objectType_, int quantity_, ObjectsCollection *parentObjectsCollection_)
        : p(std::make_shared<Pimpl>())
    /*///////////////////////////////////////////////////////////////////////////////////////*/
    {
        p->object_type_ = objectType_;
        p->quantity_ = quantity_;
        p->parent_objects_collection_ = parentObjectsCollection_;
    }

    int
    Object::GetObjectType()
    /*///////////////////*/
    {
        return p->object_type_;
    }

    void
    Object::RemoveContainedObject(int index)
    /*////////////////////////////////////*/
    {
        p->contained_objects_.erase(index);
    }

    void
    Object::AddContainedObject(int index, std::shared_ptr<Object> object)
    /*/////////////////////////////////////////////////////////////////*/
    {
        p->contained_objects_.insert({index, object});
    }

    void
    Object::SetModelScaling(float newModelScaling)
    /*//////////////////////////////////////////*/
    {
        p->model_scaling_ = newModelScaling;
    }

    void
    Object::DecreaseQuantity()
    /*//////////////////////*/
    {
        p->quantity_--;
    }

    int
    Object::GetQuantity()
    /*/////////////////*/
    {
        return p->quantity_;
    }

    void
    Object::AlterQuantity(int delta)
    /*////////////////////////////*/
    {
        p->quantity_ += delta;
    }

    float
    Object::GetWorkProgress()
    /*/////////////////////*/
    {
        return p->work_progress_;
    }

    void
    Object::AddToWorkProgress(float amount)
    /*///////////////////////////////////*/
    {
        p->work_progress_ += amount;
    }

    void
    Object::SetModelRotation(float newModelRotation)
    /*////////////////////////////////////////////*/
    {
        p->model_rotation_ = newModelRotation;
    }

    void
    Object::SetParentObjectsCollection(ObjectsCollection *newParentObjectsCollection)
    /*/////////////////////////////////////////////////////////////////////////////*/
    {
        p->parent_objects_collection_ = newParentObjectsCollection;
    }

    float
    Object::GetModelScaling()
    /*/////////////////////*/
    {
        return p->model_scaling_;
    }

    float
    Object::GetModelRotation()
    /*//////////////////////*/
    {
        return p->model_rotation_;
    }

    float
    Object::GetRedVariation()
    /*/////////////////////*/
    {
        return p->red_variation_;
    }

    float
    Object::GetGreenVariation()
    /*///////////////////////*/
    {
        return p->green_variation_;
    }

    float
    Object::GetBlueVariation()
    /*//////////////////////*/
    {
        return p->blue_variation_;
    }

    void
    Object::SetRedVariation(float newRedVariation)
    /*//////////////////////////////////////////*/
    {
        p->red_variation_ = newRedVariation;
    }

    void
    Object::SetGreenVariation(float newGreenVariation)
    /*//////////////////////////////////////////////*/
    {
        p->green_variation_ = newGreenVariation;
    }

    void
    Object::SetBlueVariation(float newBlueVariation)
    /*////////////////////////////////////////////*/
    {
        p->blue_variation_ = newBlueVariation;
    }

    float
    Object::GetTransformationProgress()
    /*///////////////////////////////*/
    {
        return p->transformation_progress_;
    }

    void
    Object::IncreaseTransformationProgress(float amount)
    /*////////////////////////////////////////////////*/
    {
        if (p->transformation_progress_ < 1.0f)
            p->transformation_progress_ += amount;
    }

    bool
    Object::TransformationFinished()
    /*////////////////////////////*/
    {
        return p->transformation_progress_ >= 1.0f;
    }

    bool
    Object::IsContainer()
    /*/////////////////*/
    {
        return ObjectBehaviourList::Get()->GetContainerSlots(p->object_type_) > 0;
    }

    void
    Object::TransformInto(std::string_view newObjectName)
    /*/////////////////////////////////////////////////*/
    {
        p->object_type_ = Hash(newObjectName);
        p->transformation_progress_ = 0.0f;
    }

    const std::map<int, std::shared_ptr<Object>> &
    Object::GetContainedObjects()
    /*//////////////////////////////////////////*/
    {
        return p->contained_objects_;
    }

    ObjectsCollection *
    Object::GetParentObjectsCollection()
    /*////////////////////////////////*/
    {
        return p->parent_objects_collection_;
    }
}