//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class ObjectsCollection;
    class Object;
    class MapArea;
    class Mob;
    class Companion;

    class Tile
    /*//////*/
    {
      public:
        Tile(MapArea *, Point2, int);
        void CalculateNormal(Point3F, Point3F, Point3F);
        void RemoveObject(Object *);
        int GetObjectsCount();
        std::shared_ptr<Object> GetObjectFromRawPtr(Object *);
        std::shared_ptr<Object> GetObjectAt(int);
        int GetRoofType();
        int GetGroundType();
        std::shared_ptr<Mob> GetMob();
        std::shared_ptr<Companion> GetCompanion();
        float GetElevation();
        float GetRedVariation();
        float GetGreenVariation();
        float GetBlueVariation();
        const Point3F &GetNormal();
        const ObjectsCollection &GetObjects();
        GLuint GetTileRendId();
        int GetGroundLayerType();
        float GetClaimProgress();
        MapArea *GetParentMapArea() const;
        const Point2 &GetCoordinate() const;
        void SetCompanion(std::shared_ptr<Companion>);
        void ClearCompanion();
        void SetMob(std::shared_ptr<Mob>);
        void ClearMob();
        void AddObject(std::shared_ptr<Object>);
        void CreateAddObject(std::string_view, int = 1);
        void CreateAddObject(int, int = 1);
        void SetGroundLayerType(int);
        void SetGroundType(int);
        void SetRoofType(int);
        bool HasNoMob();
        void ClearObjects();
        void ReplaceObject(std::shared_ptr<Object>, std::shared_ptr<Object>);
        void SetElevation(float);
        void AlterElevation(float);
        void SetRedVariation(float);
        void SetGreenVariation(float);
        void SetBlueVariation(float);
        void AlterRedVariation(float);
        void AlterGreenVariation(float);
        void AlterBlueVariation(float);
        void SetVertex0(Vertex3F);
        void SetVertex1(Vertex3F);
        void SetVertex2(Vertex3F);
        void SetVertex3(Vertex3F);
        void SetTileRendId(GLuint);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////