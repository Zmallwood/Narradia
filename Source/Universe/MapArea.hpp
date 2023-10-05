//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Tile;
    class Mob;
    class Companion;
    class Object;

    class MapArea
    /*/////////*/ {
      public:
        MapArea();
        void Create();
        Tile *GetTile(Point2);
        Tile *GetTile(int, int);
        static Size GetMapSize();
        const std::map<Mob *, Point2> &GetMobsMirror();
        const std::map<Object *, Point2> &GetObjectsMirror();
        const std::map<Companion *, Point2> &GetCompanionsMirror();
        void RemoveMobMirror(std::map<Mob *, Point2>::const_iterator);
        void RemoveMobMirror(Mob *);
        void RemoveCompanionMirror(std::map<Companion *, Point2>::const_iterator);
        void RemoveObjectMirror(Object *);
        void AddMobMirror(Mob *, Point2);
        void AddCompanionMirror(Companion *, Point2);
        void AddObjectMirror(Object *, Point2);
        static bool IsInsideMap(Point2);
        static bool IsInsideMap(int, int);

      private:
        class Pimpl;
        std::shared_ptr<Pimpl> p;
    };
}
//////////////////////////////////////////////////////////////////////