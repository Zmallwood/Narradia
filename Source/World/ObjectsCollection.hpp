//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Tile;
    class Object;

    class ObjectsCollection
    /*///////////////////*/
    {
      public:
        ObjectsCollection(Tile *);
        void CreateAdd(std::string_view, int = 1);
        void CreateAdd(int, int = 1);
        void Add(std::shared_ptr<Object>);
        void Clear();
        bool Contains(int) const;
        void Replace(std::shared_ptr<Object>, std::shared_ptr<Object>);
        void Remove(Object *);
        std::shared_ptr<Object> GetObjectFromRawPtr(Object *);
        const std::vector<std::shared_ptr<Object>> &list = trueList;

      private:
        std::vector<std::shared_ptr<Object>> trueList;
        const Tile *parentTile = nullptr;
    };
}