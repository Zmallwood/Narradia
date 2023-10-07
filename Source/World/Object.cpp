//////////////////////////////////////////////////////////////////////
#include "Object.hpp"
#include "ObjectBehaviourList.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object::Pimpl
    /*///////////////*/
    {
      public:
        int objectType = 0;
        int quantity = 1;
        float workProgress = 0.0f;
        std::map<int, std::shared_ptr<Object>> containedObjects;
        float modelScaling = 1.0f;
        float modelRotation = 0.0f;
        float redVariation = 1.0f;
        float greenVariation = 1.0f;
        float blueVariation = 1.0f;
        float transformationProgress = 0.0f;
        ObjectsCollection *parentObjectsCollection = nullptr;
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
        p->objectType = objectType_;
        p->quantity = quantity_;
        p->parentObjectsCollection = parentObjectsCollection_;
    }

    int
    Object::GetObjectType()
    /*///////////////////*/
    {
        return p->objectType;
    }

    void
    Object::RemoveContainedObject(int index)
    /*////////////////////////////////////*/
    {
        p->containedObjects.erase(index);
    }

    void
    Object::AddContainedObject(int index, std::shared_ptr<Object> object)
    /*/////////////////////////////////////////////////////////////////*/
    {
        p->containedObjects.insert({index, object});
    }

    void
    Object::SetModelScaling(float newModelScaling)
    /*//////////////////////////////////////////*/
    {
        p->modelScaling = newModelScaling;
    }

    void
    Object::DecreaseQuantity()
    /*//////////////////////*/
    {
        p->quantity--;
    }

    int
    Object::GetQuantity()
    /*/////////////////*/
    {
        return p->quantity;
    }

    void
    Object::AlterQuantity(int delta)
    /*////////////////////////////*/
    {
        p->quantity += delta;
    }

    float
    Object::GetWorkProgress()
    /*/////////////////////*/
    {
        return p->workProgress;
    }

    void
    Object::AddToWorkProgress(float amount)
    /*///////////////////////////////////*/
    {
        p->workProgress += amount;
    }

    void
    Object::SetModelRotation(float newModelRotation)
    /*////////////////////////////////////////////*/
    {
        p->modelRotation = newModelRotation;
    }

    void
    Object::SetParentObjectsCollection(ObjectsCollection *newParentObjectsCollection)
    /*/////////////////////////////////////////////////////////////////////////////*/
    {
        p->parentObjectsCollection = newParentObjectsCollection;
    }

    float
    Object::GetModelScaling()
    /*/////////////////////*/
    {
        return p->modelScaling;
    }

    float
    Object::GetModelRotation()
    /*//////////////////////*/
    {
        return p->modelRotation;
    }

    float
    Object::GetRedVariation()
    /*/////////////////////*/
    {
        return p->redVariation;
    }

    float
    Object::GetGreenVariation()
    /*///////////////////////*/
    {
        return p->greenVariation;
    }

    float
    Object::GetBlueVariation()
    /*//////////////////////*/
    {
        return p->blueVariation;
    }

    void
    Object::SetRedVariation(float newRedVariation)
    /*//////////////////////////////////////////*/
    {
        p->redVariation = newRedVariation;
    }

    void
    Object::SetGreenVariation(float newGreenVariation)
    /*//////////////////////////////////////////////*/
    {
        p->greenVariation = newGreenVariation;
    }

    void
    Object::SetBlueVariation(float newBlueVariation)
    /*////////////////////////////////////////////*/
    {
        p->blueVariation = newBlueVariation;
    }

    float
    Object::GetTransformationProgress()
    /*///////////////////////////////*/
    {
        return p->transformationProgress;
    }

    void
    Object::IncreaseTransformationProgress(float amount)
    /*////////////////////////////////////////////////*/
    {
        if (p->transformationProgress < 1.0f)
            p->transformationProgress += amount;
    }

    bool
    Object::TransformationFinished()
    /*////////////////////////////*/
    {
        return p->transformationProgress >= 1.0f;
    }

    bool
    Object::IsContainer()
    /*/////////////////*/
    {
        return ObjectBehaviourList::Get().GetContainerSlots(p->objectType) > 0;
    }

    void
    Object::TransformInto(std::string_view newObjectName)
    /*/////////////////////////////////////////////////*/
    {
        p->objectType = Hash(newObjectName);
        p->transformationProgress = 0.0f;
    }

    const std::map<int, std::shared_ptr<Object>> &
    Object::GetContainedObjects()
    /*//////////////////////////////////////////*/
    {
        return p->containedObjects;
    }

    ObjectsCollection *
    Object::GetParentObjectsCollection()
    /*////////////////////////////////*/
    {
        return p->parentObjectsCollection;
    }
}
//////////////////////////////////////////////////////////////////////