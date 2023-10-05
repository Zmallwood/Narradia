//////////////////////////////////////////////////////////////////////
#include "Tile.hpp"
#include "ObjectsCollection.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Tile::Pimpl
    /*/////////////*/ {
      public:
        int groundType = 0;
        std::shared_ptr<Mob> mob = nullptr;
        std::shared_ptr<Companion> companion = nullptr;
        float elevation = 0.0f;
        int groundLayerType = 0;
        int roofType = 0;
        std::shared_ptr<ObjectsCollection> objects;
        Point3F normal = {0.0f, 0.0f, 0.0f};
        float redVariation = 0.0f;
        float greenVariation = 0.0f;
        float blueVariation = 0.0f;
        std::array<Vertex3F, 4> geometry;
        MapArea *parentMapArea = nullptr;
        Point2 coordinate;
        GLuint tileRendId;
    };

    Tile::Tile(MapArea *parentMapArea_, Point2 coordinate_, int groundType_)
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////////////////////////////*/ {
        p->parentMapArea = parentMapArea_;
        p->coordinate = coordinate_;
        p->groundType = groundType_;
        p->objects = std::make_shared<ObjectsCollection>(this);
    }

    void Tile::CalculateNormal(Point3F v0, Point3F v1, Point3F v2)
    /*//////////////////////////////////////////////////////////*/ {
        auto point0 = glm::vec3(v0.x, v0.y, v0.z);
        auto point1 = glm::vec3(v1.x, v1.y, v1.z);
        auto Point2 = glm::vec3(v2.x, v2.y, v2.z);
        auto vector0 = point1 - point0;
        auto vector1 = Point2 - point0;
        auto vector2 = glm::cross(vector0, vector1);
        auto normalGl = glm::normalize(vector2);
        p->normal = {-normalGl.x, -normalGl.y, -normalGl.z};
    }

    void Tile::RemoveObject(Object *object)
    /*///////////////////////////////////*/ {
        p->objects->Remove(object);
    }

    int Tile::GetObjectsCount()
    /*///////////////////////*/ {
        return p->objects->list.size();
    }

    std::shared_ptr<Object> Tile::GetObjectFromRawPtr(Object *object)
    /*/////////////////////////////////////////////////////////////*/ {
        return p->objects->GetObjectFromRawPtr(object);
    }

    std::shared_ptr<Object> Tile::GetObjectAt(int i)
    /*////////////////////////////////////////////*/ {
        return p->objects->list.at(i);
    }

    int Tile::GetRoofType()
    /*///////////////////*/ {
        return p->roofType;
    }

    int Tile::GetGroundType()
    /*/////////////////////*/ {
        return p->groundType;
    }

    std::shared_ptr<Mob> Tile::GetMob()
    /*///////////////////////////////*/ {
        return p->mob;
    }

    std::shared_ptr<Companion> Tile::GetCompanion()
    /*///////////////////////////////////////////*/ {
        return p->companion;
    }

    float Tile::GetElevation()
    /*//////////////////////*/ {
        return p->elevation;
    }

    float Tile::GetRedVariation()
    /*/////////////////////////*/ {
        return p->redVariation;
    }

    float Tile::GetGreenVariation()
    /*///////////////////////////*/ {
        return p->greenVariation;
    }

    float Tile::GetBlueVariation()
    /*//////////////////////////*/ {
        return p->blueVariation;
    }

    const Point3F &Tile::GetNormal()
    /*////////////////////////////*/ {
        return p->normal;
    }

    const ObjectsCollection &Tile::GetObjects()
    /*///////////////////////////////////////*/ {
        return *p->objects;
    }

    GLuint Tile::GetTileRendId()
    /*////////////////////////*/ {
        return p->tileRendId;
    }

    int Tile::GetGroundLayerType()
    /*//////////////////////////*/ {
        return p->groundLayerType;
    }

    MapArea *Tile::GetParentMapArea() const
    /*///////////////////////////////////*/ {
        return p->parentMapArea;
    }

    const Point2 &Tile::GetCoordinate() const
    /*/////////////////////////////////////*/ {
        return p->coordinate;
    }

    void Tile::SetCompanion(std::shared_ptr<Companion> newCompanion)
    /*////////////////////////////////////////////////////////////*/ {
        p->companion = newCompanion;
    }

    void Tile::ClearCompanion()
    /*///////////////////////*/ {
        p->companion = nullptr;
    }

    void Tile::SetMob(std::shared_ptr<Mob> newMob)
    /*//////////////////////////////////////////*/ {
        p->mob = newMob;
    }

    void Tile::ClearMob()
    /*/////////////////*/ {
        p->mob = nullptr;
    }

    void Tile::AddObject(std::shared_ptr<Object> object)
    /*////////////////////////////////////////////////*/ {
        p->objects->Add(object);
    }

    void Tile::CreateAddObject(std::string_view objectType, int quantity)
    /*/////////////////////////////////////////////////////////////////*/ {
        p->objects->CreateAdd(objectType, quantity);
    }

    void Tile::CreateAddObject(int objectTypeHash, int quantity)
    /*////////////////////////////////////////////////////////*/ {
        p->objects->CreateAdd(objectTypeHash, quantity);
    }

    void Tile::SetGroundLayerType(int newGroundLayerType)
    /*/////////////////////////////////////////////////*/ {
        p->groundLayerType = newGroundLayerType;
    }

    void Tile::SetGroundType(int newGroundType)
    /*///////////////////////////////////////*/ {
        p->groundType = newGroundType;
    }

    void Tile::SetRoofType(int newRoofType)
    /*///////////////////////////////////*/ {
        p->roofType = newRoofType;
    }

    bool Tile::HasNoMob()
    /*/////////////////*/ {
        return p->mob == nullptr;
    }

    void Tile::ClearObjects()
    /*/////////////////////*/ {
        p->objects->Clear();
    }

    void Tile::ReplaceObject(std::shared_ptr<Object> origObject, std::shared_ptr<Object> newObject)
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        p->objects->Replace(origObject, newObject);
    }

    void Tile::SetElevation(float newElevation)
    /*///////////////////////////////////////*/ {
        p->elevation = newElevation;
    }

    void Tile::AlterElevation(float elevationAdd)
    /*///////////////////////////////////////*/ {
        p->elevation += elevationAdd;
    }

    void Tile::SetRedVariation(float newRedVariation)
    /*/////////////////////////////////////////////*/ {
        p->redVariation = newRedVariation;
    }

    void Tile::SetGreenVariation(float newGreenVariation)
    /*/////////////////////////////////////////////////*/ {
        p->greenVariation = newGreenVariation;
    }

    void Tile::SetBlueVariation(float newBlueVariation)
    /*///////////////////////////////////////////////*/ {
        p->blueVariation = newBlueVariation;
    }

    void Tile::AlterRedVariation(float deltaRedVariation)
    /*/////////////////////////////////////////////////*/ {
        p->redVariation += deltaRedVariation;
    }

    void Tile::AlterGreenVariation(float deltaGreenVariation)
    /*/////////////////////////////////////////////////////*/ {
        p->greenVariation += deltaGreenVariation;
    }

    void Tile::AlterBlueVariation(float deltaBlueVariation)
    /*///////////////////////////////////////////////////*/ {
        p->blueVariation += deltaBlueVariation;
    }

    void Tile::SetVertex0(Vertex3F vertex)
    /*//////////////////////////////////*/ {
        p->geometry[0] = vertex;
    }

    void Tile::SetVertex1(Vertex3F vertex)
    /*//////////////////////////////////*/ {
        p->geometry[1] = vertex;
    }

    void Tile::SetVertex2(Vertex3F vertex)
    /*//////////////////////////////////*/ {
        p->geometry[2] = vertex;
    }

    void Tile::SetVertex3(Vertex3F vertex)
    /*//////////////////////////////////*/ {
        p->geometry[3] = vertex;
    }

    void Tile::SetTileRendId(GLuint newTileRendId)
    /*//////////////////////////////////////////*/ {
        p->tileRendId = newTileRendId;
    }
}
//////////////////////////////////////////////////////////////////////