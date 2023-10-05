//////////////////////////////////////////////////////////////////////
#include "Object.hpp"
#include "ObjectBehaviourList.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object::Pimpl
    /*///////////////*/ {
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
    }; // Class

    Object::Object(
        std::string_view objectTypeName, int quantity_, ObjectsCollection *parentObjectsCollection_)
        : Object(Hash(objectTypeName.data()), quantity_, parentObjectsCollection_)
    /*//////////////////////////////////////////////////////////////////////////////////*/ {
    } // Function

    Object::Object(int objectType_, int quantity_, ObjectsCollection *parentObjectsCollection_)
        : p(std::make_shared<Pimpl>())
    /*///////////////////////////////////////////////////////////////////////////////////////*/ {
        p->objectType = objectType_;
        p->quantity = quantity_;
        p->parentObjectsCollection = parentObjectsCollection_;
    } // Function

    int Object::GetObjectType()
    /*///////////////////////*/ { return p->objectType; } // Function

    void Object::RemoveContainedObject(int index)
    /*/////////////////////////////////////////*/ { p->containedObjects.erase(index); } // Function

    void Object::AddContainedObject(int index, std::shared_ptr<Object> object)
    /*//////////////////////////////////////////////////////////////////////*/ {
        p->containedObjects.insert({index, object});
    } // Function

    void Object::SetModelScaling(float newModelScaling)
    /*///////////////////////////////////////////////*/ {
        p->modelScaling = newModelScaling;
    } // Function

    void Object::DecreaseQuantity()
    /*///////////////////////////*/ { p->quantity--; } // Function

    int Object::GetQuantity()
    /*/////////////////////*/ { return p->quantity; } // Function

    void Object::AlterQuantity(int delta)
    /*/////////////////////////////////*/ { p->quantity += delta; } // Function

    float Object::GetWorkProgress()
    /*///////////////////////////*/ { return p->workProgress; } // Function

    void Object::AddToWorkProgress(float amount)
    /*////////////////////////////////////////*/ { p->workProgress += amount; } // Function

    void Object::SetModelRotation(float newModelRotation)
    /*/////////////////////////////////////////////////*/ {
        p->modelRotation = newModelRotation;
    } // Function

    void Object::SetParentObjectsCollection(ObjectsCollection *newParentObjectsCollection)
    /*//////////////////////////////////////////////////////////////////////////////////*/ {
        p->parentObjectsCollection = newParentObjectsCollection;
    } // Function

    float Object::GetModelScaling()
    /*///////////////////////////*/ { return p->modelScaling; } // Function

    float Object::GetModelRotation()
    /*////////////////////////////*/ { return p->modelRotation; } // Function

    float Object::GetRedVariation()
    /*///////////////////////////*/ { return p->redVariation; } // Function

    float Object::GetGreenVariation()
    /*/////////////////////////////*/ { return p->greenVariation; } // Function

    float Object::GetBlueVariation()
    /*////////////////////////////*/ { return p->blueVariation; } // Function

    void Object::SetRedVariation(float newRedVariation)
    /*///////////////////////////////////////////////*/ {
        p->redVariation = newRedVariation;
    } // Function

    void Object::SetGreenVariation(float newGreenVariation)
    /*///////////////////////////////////////////////////*/ {
        p->greenVariation = newGreenVariation;
    } // Function

    void Object::SetBlueVariation(float newBlueVariation)
    /*/////////////////////////////////////////////////*/ {
        p->blueVariation = newBlueVariation;
    } // Function

    float Object::GetTransformationProgress()
    /*/////////////////////////////////////*/ { return p->transformationProgress; } // Function

    void Object::IncreaseTransformationProgress(float amount)
    /*/////////////////////////////////////////////////////*/ {
        if (p->transformationProgress < 1.0f) p->transformationProgress += amount;
    } // Function

    bool Object::TransformationFinished()
    /*/////////////////////////////////*/ { return p->transformationProgress >= 1.0f; } // Function

    bool Object::IsContainer()
    /*//////////////////////*/ {
        return ObjectBehaviourList::Get().GetContainerSlots(p->objectType) > 0;
    } // Function

    void Object::TransformInto(std::string_view newObjectName)
    /*//////////////////////////////////////////////////////*/ {
        p->objectType = Hash(newObjectName);
        p->transformationProgress = 0.0f;
    } // Function

    const std::map<int, std::shared_ptr<Object>> &Object::GetContainedObjects()
    /*///////////////////////////////////////////////////////////////////////*/ {
        return p->containedObjects;
    } // Function

    ObjectsCollection *Object::GetParentObjectsCollection()
    /*///////////////////////////////////////////////////*/ {
        return p->parentObjectsCollection;
    } // Function
}
//////////////////////////////////////////////////////////////////////