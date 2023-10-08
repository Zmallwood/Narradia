#include "Tile.hpp"
#include "ObjectsCollection.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Tile::Pimpl {
      public:
        int ground_type_ = 0;
        std::shared_ptr<Mob> mob_ = nullptr;
        std::shared_ptr<Companion> companion_ = nullptr;
        float elevation_ = 0.0f;
        int ground_layer_type_ = 0;
        int roof_type_ = 0;
        std::shared_ptr<ObjectsCollection> objects_;
        Point3F normal_ = {0.0f, 0.0f, 0.0f};
        float red_variation_ = 0.0f;
        float green_variation_ = 0.0f;
        float blue_variation_ = 0.0f;
        std::array<Vertex3F, 4> geometry_;
        MapArea *parent_map_area_ = nullptr;
        Point2 coordinate_;
        GLuint rendid_ground_;
    };

    Tile::Tile(MapArea *parentMapArea_, Point2 coordinate_, int groundType_)
        : p(std::make_shared<Pimpl>()) {
        p->parent_map_area_ = parentMapArea_;
        p->coordinate_ = coordinate_;
        p->ground_type_ = groundType_;
        p->objects_ = std::make_shared<ObjectsCollection>(this);
    }

    void Tile::CalculateNormal(Point3F v0, Point3F v1, Point3F v2) {
        auto point0 = glm::vec3(v0.x, v0.y, v0.z);
        auto point1 = glm::vec3(v1.x, v1.y, v1.z);
        auto Point2 = glm::vec3(v2.x, v2.y, v2.z);
        auto vector0 = point1 - point0;
        auto vector1 = Point2 - point0;
        auto vector2 = glm::cross(vector0, vector1);
        auto normalGl = glm::normalize(vector2);
        p->normal_ = {-normalGl.x, -normalGl.y, -normalGl.z};
    }

    void Tile::RemoveObject(Object *object) {
        p->objects_->Remove(object);
    }

    int Tile::GetObjectsCount() {
        return p->objects_->list_.size();
    }

    std::shared_ptr<Object> Tile::GetObjectFromRawPtr(Object *object) {
        return p->objects_->GetObjectFromRawPtr(object);
    }

    std::shared_ptr<Object> Tile::GetObjectAt(int i) {
        return p->objects_->list_.at(i);
    }

    int Tile::GetRoofType() {
        return p->roof_type_;
    }

    int Tile::GetGroundType() {
        return p->ground_type_;
    }

    std::shared_ptr<Mob> Tile::GetMob() {
        return p->mob_;
    }

    std::shared_ptr<Companion> Tile::GetCompanion() {
        return p->companion_;
    }

    float Tile::GetElevation() {
        return p->elevation_;
    }

    float Tile::GetRedVariation() {
        return p->red_variation_;
    }

    float Tile::GetGreenVariation() {
        return p->green_variation_;
    }

    float Tile::GetBlueVariation() {
        return p->blue_variation_;
    }

    const Point3F &Tile::GetNormal() {
        return p->normal_;
    }

    const ObjectsCollection &Tile::GetObjects() {
        return *p->objects_;
    }

    GLuint Tile::GetTileRendId() {
        return p->rendid_ground_;
    }

    int Tile::GetGroundLayerType() {
        return p->ground_layer_type_;
    }

    MapArea *Tile::GetParentMapArea() const {
        return p->parent_map_area_;
    }

    const Point2 &Tile::GetCoordinate() const {
        return p->coordinate_;
    }

    void Tile::SetCompanion(std::shared_ptr<Companion> newCompanion) {
        p->companion_ = newCompanion;
    }

    void Tile::ClearCompanion() {
        p->companion_ = nullptr;
    }

    void Tile::SetMob(std::shared_ptr<Mob> newMob) {
        p->mob_ = newMob;
    }

    void Tile::ClearMob() {
        p->mob_ = nullptr;
    }

    void Tile::AddObject(std::shared_ptr<Object> object) {
        p->objects_->Add(object);
    }

    void Tile::CreateAddObject(std::string_view objectType, int quantity) {
        p->objects_->CreateAdd(objectType, quantity);
    }

    void Tile::CreateAddObject(int objectTypeHash, int quantity) {
        p->objects_->CreateAdd(objectTypeHash, quantity);
    }

    void Tile::SetGroundLayerType(int newGroundLayerType) {
        p->ground_layer_type_ = newGroundLayerType;
    }

    void Tile::SetGroundType(int newGroundType) {
        p->ground_type_ = newGroundType;
    }

    void Tile::SetRoofType(int newRoofType) {
        p->roof_type_ = newRoofType;
    }

    bool Tile::HasNoMob() {
        return p->mob_ == nullptr;
    }

    void Tile::ClearObjects() {
        p->objects_->Clear();
    }

    void
    Tile::ReplaceObject(std::shared_ptr<Object> origObject, std::shared_ptr<Object> newObject) {
        p->objects_->Replace(origObject, newObject);
    }

    void Tile::SetElevation(float newElevation) {
        p->elevation_ = newElevation;
    }

    void Tile::AlterElevation(float elevationAdd) {
        p->elevation_ += elevationAdd;
    }

    void Tile::SetRedVariation(float newRedVariation) {
        p->red_variation_ = newRedVariation;
    }

    void Tile::SetGreenVariation(float newGreenVariation) {
        p->green_variation_ = newGreenVariation;
    }

    void Tile::SetBlueVariation(float newBlueVariation) {
        p->blue_variation_ = newBlueVariation;
    }

    void Tile::AlterRedVariation(float deltaRedVariation) {
        p->red_variation_ += deltaRedVariation;
    }

    void Tile::AlterGreenVariation(float deltaGreenVariation) {
        p->green_variation_ += deltaGreenVariation;
    }

    void Tile::AlterBlueVariation(float deltaBlueVariation) {
        p->blue_variation_ += deltaBlueVariation;
    }

    void Tile::SetVertex0(Vertex3F vertex) {
        p->geometry_[0] = vertex;
    }

    void Tile::SetVertex1(Vertex3F vertex) {
        p->geometry_[1] = vertex;
    }

    void Tile::SetVertex2(Vertex3F vertex) {
        p->geometry_[2] = vertex;
    }

    void Tile::SetVertex3(Vertex3F vertex) {
        p->geometry_[3] = vertex;
    }

    void Tile::SetTileRendId(GLuint newTileRendId) {
        p->rendid_ground_ = newTileRendId;
    }
}