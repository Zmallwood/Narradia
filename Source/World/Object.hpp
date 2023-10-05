//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectsCollection;

    class Object
    /*////////*/
    {
      public:
        Object(std::string_view, int = 1, ObjectsCollection * = nullptr);
        Object(int, int = 1, ObjectsCollection * = nullptr);
        int GetObjectType();
        const std::map<int, std::shared_ptr<Object>> &GetContainedObjects();
        void RemoveContainedObject(int);
        void AddContainedObject(int, std::shared_ptr<Object>);
        void SetModelScaling(float);
        void DecreaseQuantity();
        int GetQuantity();
        void AlterQuantity(int);
        float GetWorkProgress();
        void AddToWorkProgress(float);
        void SetModelRotation(float);
        ObjectsCollection *GetParentObjectsCollection();
        void SetParentObjectsCollection(ObjectsCollection *);
        float GetModelScaling();
        float GetModelRotation();
        float GetRedVariation();
        float GetGreenVariation();
        float GetBlueVariation();
        void SetRedVariation(float);
        void SetGreenVariation(float);
        void SetBlueVariation(float);
        float GetTransformationProgress();
        void IncreaseTransformationProgress(float);
        bool TransformationFinished();
        bool IsContainer();
        void TransformInto(std::string_view);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////