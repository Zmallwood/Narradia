#include "MapArea.hpp"
#include "Tile.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class MapArea::Pimpl
    /*////////////////*/
    {
      public:
        inline static constexpr Size kMapSize = {100, 100};
        std::map<Mob *, Point2> mobs_mirror_;
        std::map<Object *, Point2> objects_mirror_;
        std::map<Companion *, Point2> companions_mirror_;
        std::vector<std::vector<std::shared_ptr<Tile>>> tiles_;
    };

    MapArea::MapArea()
        : p(std::make_shared<Pimpl>())
    /*//////////////////////////////*/
    {
    }

    void
    MapArea::Create()
    /*/////////////*/
    {
        for (auto x = 0; x < Pimpl::kMapSize.width; x++)
        /*********************************************/
        {
            p->tiles_.push_back(std::vector<std::shared_ptr<Tile>>());
            for (auto y = 0; y < Pimpl::kMapSize.height; y++)
            /**********************************************/
            {
                auto new_tile = std::make_shared<Tile>(this, Point2{x, y}, Hash("GroundGrass"));
                p->tiles_.at(x).push_back(new_tile);
            }
        }
    }

    Tile *
    MapArea::GetTile(Point2 coordinate)
    /*///////////////////////////////*/
    {
        if (IsInsideMap(coordinate))
            return p->tiles_.at(coordinate.x).at(coordinate.y).get();
        return nullptr;
    }

    Tile *
    MapArea::GetTile(int x, int y)
    /*//////////////////////////*/
    {
        if (IsInsideMap(x, y))
            return p->tiles_.at(x).at(y).get();
        return nullptr;
    }

    Size
    MapArea::GetMapSize()
    /*/////////////////*/
    {
        return Pimpl::kMapSize;
    }

    const std::map<Mob *, Point2> &
    MapArea::GetMobsMirror()
    /*///////////////////////////*/
    {
        return p->mobs_mirror_;
    }

    const std::map<Object *, Point2> &
    MapArea::GetObjectsMirror()
    /*//////////////////////////////*/
    {
        return p->objects_mirror_;
    }

    const std::map<Companion *, Point2> &
    MapArea::GetCompanionsMirror()
    /*/////////////////////////////////*/
    {
        return p->companions_mirror_;
    }

    void
    MapArea::RemoveMobMirror(std::map<Mob *, Point2>::const_iterator entry)
    /*///////////////////////////////////////////////////////////////////*/
    {
        p->mobs_mirror_.erase(entry);
    }

    void
    MapArea::RemoveMobMirror(Mob *mob)
    /*//////////////////////////////*/
    {
        p->mobs_mirror_.erase(mob);
    }

    void
    MapArea::RemoveCompanionMirror(std::map<Companion *, Point2>::const_iterator entry)
    /*///////////////////////////////////////////////////////////////////////////////*/
    {
        p->companions_mirror_.erase(entry);
    }

    void
    MapArea::RemoveObjectMirror(Object *object)
    /*///////////////////////////////////////*/
    {
        p->objects_mirror_.erase(object);
    }

    void
    MapArea::AddMobMirror(Mob *mob, Point2 coordinate)
    /*//////////////////////////////////////////////*/
    {
        p->mobs_mirror_.insert({mob, coordinate});
    }

    void
    MapArea::AddCompanionMirror(Companion *companion, Point2 coordinate)
    /*////////////////////////////////////////////////////////////////*/
    {
        p->companions_mirror_.insert({companion, coordinate});
    }

    void
    MapArea::AddObjectMirror(Object *object, Point2 coordinate)
    /*///////////////////////////////////////////////////////*/
    {
        p->objects_mirror_.insert({object, coordinate});
    }

    bool
    MapArea::IsInsideMap(Point2 coordinate)
    /*///////////////////////////////////*/
    {
        return IsInsideMap(coordinate.x, coordinate.y);
    }

    bool
    MapArea::IsInsideMap(int x, int y)
    /*//////////////////////////////*/
    {
        return x >= 0 && y >= 0 && x < Pimpl::kMapSize.width && y < Pimpl::kMapSize.height;
    }
}