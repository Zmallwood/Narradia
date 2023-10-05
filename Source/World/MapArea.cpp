//////////////////////////////////////////////////////////////////////
#include "MapArea.hpp"
#include "Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapArea::Pimpl
    /*////////////////*/ {
      public:
        inline static constexpr Size mapSize = {100, 100};
        std::map<Mob *, Point2> mobsMirror;
        std::map<Object *, Point2> objectsMirror;
        std::map<Companion *, Point2> companionsMirror;
        std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    };

    MapArea::MapArea()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////////*/ {
    }

    void MapArea::Create()
    /*//////////////////*/ {
        for (auto x = 0; x < Pimpl::mapSize.width; x++)
        /*********************************************/ {
            p->tiles.push_back(std::vector<std::shared_ptr<Tile>>());
            for (auto y = 0; y < Pimpl::mapSize.height; y++)
            /**********************************************/ {
                auto newTile = std::make_shared<Tile>(this, Point2{x, y}, Hash("GroundGrass"));
                p->tiles.at(x).push_back(newTile);
            }
        }
    }

    Tile *MapArea::GetTile(Point2 coordinate)
    /*/////////////////////////////////////*/ {
        if (IsInsideMap(coordinate))
            return p->tiles.at(coordinate.x).at(coordinate.y).get();
        return nullptr;
    }

    Tile *MapArea::GetTile(int x, int y)
    /*////////////////////////////////*/ {
        if (IsInsideMap(x, y))
            return p->tiles.at(x).at(y).get();
        return nullptr;
    }

    Size MapArea::GetMapSize()
    /*//////////////////////*/ {
        return Pimpl::mapSize;
    }

    const std::map<Mob *, Point2> &MapArea::GetMobsMirror()
    /*///////////////////////////////////////////////////*/ {
        return p->mobsMirror;
    }

    const std::map<Object *, Point2> &MapArea::GetObjectsMirror()
    /*/////////////////////////////////////////////////////////*/ {
        return p->objectsMirror;
    }

    const std::map<Companion *, Point2> &MapArea::GetCompanionsMirror()
    /*///////////////////////////////////////////////////////////////*/ {
        return p->companionsMirror;
    }

    void MapArea::RemoveMobMirror(std::map<Mob *, Point2>::const_iterator entry)
    /*////////////////////////////////////////////////////////////////////////*/ {
        p->mobsMirror.erase(entry);
    }

    void MapArea::RemoveMobMirror(Mob *mob)
    /*///////////////////////////////////*/ {
        p->mobsMirror.erase(mob);
    }

    void MapArea::RemoveCompanionMirror(std::map<Companion *, Point2>::const_iterator entry)
    /*////////////////////////////////////////////////////////////////////////////////////*/ {
        p->companionsMirror.erase(entry);
    }

    void MapArea::RemoveObjectMirror(Object *object)
    /*////////////////////////////////////////////*/ {
        p->objectsMirror.erase(object);
    }

    void MapArea::AddMobMirror(Mob *mob, Point2 coordinate)
    /*///////////////////////////////////////////////////*/ {
        p->mobsMirror.insert({mob, coordinate});
    }

    void MapArea::AddCompanionMirror(Companion *companion, Point2 coordinate)
    /*/////////////////////////////////////////////////////////////////////*/ {
        p->companionsMirror.insert({companion, coordinate});
    }

    void MapArea::AddObjectMirror(Object *object, Point2 coordinate)
    /*////////////////////////////////////////////////////////////*/ {
        p->objectsMirror.insert({object, coordinate});
    }

    bool MapArea::IsInsideMap(Point2 coordinate)
    /*////////////////////////////////////////*/ {
        return IsInsideMap(coordinate.x, coordinate.y);
    }

    bool MapArea::IsInsideMap(int x, int y)
    /*///////////////////////////////////*/ {
        return x >= 0 && y >= 0 && x < Pimpl::mapSize.width && y < Pimpl::mapSize.height;
    }
}
//////////////////////////////////////////////////////////////////////