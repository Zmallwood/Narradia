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
    }; // Class

    Tile::Tile(MapArea *parentMapArea_, Point2 coordinate_, int groundType_)
        : p(std::make_shared<Pimpl>())
    /*////////////////////////////////////////////////////////////////////*/ {
        p->parentMapArea = parentMapArea_;
        p->coordinate = coordinate_;
        p->groundType = groundType_;
        p->objects = std::make_shared<ObjectsCollection>(this);
    } // Function

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
    } // Function

    void Tile::RemoveObject(Object *object)
    /*///////////////////////////////////*/ { p->objects->Remove(object); } // Function

    int Tile::GetObjectsCount()
    /*///////////////////////*/ { return p->objects->list.size(); } // Function

    std::shared_ptr<Object> Tile::GetObjectFromRawPtr(Object *object)
    /*/////////////////////////////////////////////////////////////*/ {
        return p->objects->GetObjectFromRawPtr(object);
    } // Function

    std::shared_ptr<Object> Tile::GetObjectAt(int i)
    /*////////////////////////////////////////////*/ { return p->objects->list.at(i); } // Function

    int Tile::GetRoofType()
    /*///////////////////*/ { return p->roofType; } // Function

    int Tile::GetGroundType()
    /*/////////////////////*/ { return p->groundType; } // Function

    std::shared_ptr<Mob> Tile::GetMob()
    /*///////////////////////////////*/ { return p->mob; } // Function

    std::shared_ptr<Companion> Tile::GetCompanion()
    /*///////////////////////////////////////////*/ { return p->companion; } // Function

    float Tile::GetElevation()
    /*//////////////////////*/ { return p->elevation; } // Function

    float Tile::GetRedVariation()
    /*/////////////////////////*/ { return p->redVariation; } // Function

    float Tile::GetGreenVariation()
    /*///////////////////////////*/ { return p->greenVariation; } // Function

    float Tile::GetBlueVariation()
    /*//////////////////////////*/ { return p->blueVariation; } // Function

    const Point3F &Tile::GetNormal()
    /*////////////////////////////*/ { return p->normal; } // Function

    const ObjectsCollection &Tile::GetObjects()
    /*///////////////////////////////////////*/ { return *p->objects; } // Function

    GLuint Tile::GetTileRendId()
    /*////////////////////////*/ { return p->tileRendId; } // Function

    int Tile::GetGroundLayerType()
    /*//////////////////////////*/ { return p->groundLayerType; } // Function

    MapArea *Tile::GetParentMapArea() const
    /*///////////////////////////////////*/ { return p->parentMapArea; } // Function

    const Point2 &Tile::GetCoordinate() const
    /*/////////////////////////////////////*/ { return p->coordinate; } // Function

    void Tile::SetCompanion(std::shared_ptr<Companion> newCompanion)
    /*////////////////////////////////////////////////////////////*/ {
        p->companion = newCompanion;
    } // Function

    void Tile::ClearCompanion()
    /*///////////////////////*/ { p->companion = nullptr; } // Function

    void Tile::SetMob(std::shared_ptr<Mob> newMob)
    /*//////////////////////////////////////////*/ { p->mob = newMob; } // Function

    void Tile::ClearMob()
    /*/////////////////*/ { p->mob = nullptr; } // Function

    void Tile::AddObject(std::shared_ptr<Object> object)
    /*////////////////////////////////////////////////*/ { p->objects->Add(object); } // Function

    void Tile::CreateAddObject(std::string_view objectType, int quantity)
    /*/////////////////////////////////////////////////////////////////*/ {
        p->objects->CreateAdd(objectType, quantity);
    } // Function

    void Tile::CreateAddObject(int objectTypeHash, int quantity)
    /*////////////////////////////////////////////////////////*/ {
        p->objects->CreateAdd(objectTypeHash, quantity);
    } // Function

    void Tile::SetGroundLayerType(int newGroundLayerType)
    /*/////////////////////////////////////////////////*/ {
        p->groundLayerType = newGroundLayerType;
    } // Function

    void Tile::SetGroundType(int newGroundType)
    /*///////////////////////////////////////*/ { p->groundType = newGroundType; } // Function

    void Tile::SetRoofType(int newRoofType)
    /*///////////////////////////////////*/ { p->roofType = newRoofType; } // Function

    bool Tile::HasNoMob()
    /*/////////////////*/ { return p->mob == nullptr; } // Function

    void Tile::ClearObjects()
    /*/////////////////////*/ { p->objects->Clear(); } // Function

    void Tile::ReplaceObject(std::shared_ptr<Object> origObject, std::shared_ptr<Object> newObject)
    /*///////////////////////////////////////////////////////////////////////////////////////////*/
    {
        p->objects->Replace(origObject, newObject);
    } // Function

    void Tile::SetElevation(float newElevation)
    /*///////////////////////////////////////*/ { p->elevation = newElevation; } // Function

    void Tile::AlterElevation(float elevationAdd)
    /*///////////////////////////////////////*/ { p->elevation += elevationAdd; } // Function

    void Tile::SetRedVariation(float newRedVariation)
    /*/////////////////////////////////////////////*/ {
        p->redVariation = newRedVariation;
    } // Function

    void Tile::SetGreenVariation(float newGreenVariation)
    /*/////////////////////////////////////////////////*/ {
        p->greenVariation = newGreenVariation;
    } // Function

    void Tile::SetBlueVariation(float newBlueVariation)
    /*///////////////////////////////////////////////*/ {
        p->blueVariation = newBlueVariation;
    } // Function

    void Tile::AlterRedVariation(float deltaRedVariation)
    /*/////////////////////////////////////////////////*/ {
        p->redVariation += deltaRedVariation;
    } // Function

    void Tile::AlterGreenVariation(float deltaGreenVariation)
    /*/////////////////////////////////////////////////////*/ {
        p->greenVariation += deltaGreenVariation;
    } // Function

    void Tile::AlterBlueVariation(float deltaBlueVariation)
    /*///////////////////////////////////////////////////*/ {
        p->blueVariation += deltaBlueVariation;
    } // Function

    void Tile::SetVertex0(Vertex3F vertex)
    /*//////////////////////////////////*/ { p->geometry[0] = vertex; } // Function

    void Tile::SetVertex1(Vertex3F vertex)
    /*//////////////////////////////////*/ { p->geometry[1] = vertex; } // Function

    void Tile::SetVertex2(Vertex3F vertex)
    /*//////////////////////////////////*/ { p->geometry[2] = vertex; } // Function

    void Tile::SetVertex3(Vertex3F vertex)
    /*//////////////////////////////////*/ { p->geometry[3] = vertex; } // Function

    void Tile::SetTileRendId(GLuint newTileRendId)
    /*//////////////////////////////////////////*/ { p->tileRendId = newTileRendId; } // Function
}
//////////////////////////////////////////////////////////////////////