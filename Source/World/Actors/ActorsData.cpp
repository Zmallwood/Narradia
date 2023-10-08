#include "ActorsData.hpp"
#include "World/Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void Inventory::Add(std::string_view objectName, int quantity) {
        for (auto i = 0; i < kMaxNumberInventoryObjects; i++) {
            if (objects_.count(i) == 0) {
                auto new_object = std::make_shared<Object>(objectName, quantity);
                objects_.insert({i, new_object});
                return;
            }
        }
    }

    void Inventory::Add(std::shared_ptr<Object> object) {
        for (auto i = 0; i < kMaxNumberInventoryObjects; i++) {
            if (objects_.count(i) == 0) {
                objects_.insert({i, object});
                return;
            }
        }
    }

    bool Inventory::ContainsObjectOfType(std::string_view objectType) {
        for (auto &entry : objects_)
            if (entry.second->GetObjectType() == Hash(objectType))
                return true;
        return false;
    }

    void Inventory::RemoveObjectOfType(std::string_view objectType) {
        for (auto &entry : objects_) {
            if (entry.second->GetObjectType() == Hash(objectType)) {
                objects_.erase(entry.first);
                return;
            }
        }
    }

    void ActorData::Update() {
        if (movement_.is_moving)
            if (ticks_last_update_ != 0)
                delta_t_ += (SDL_GetTicks() - ticks_last_update_);
        ticks_last_update_ = SDL_GetTicks();
    }

    SkillSet::SkillSet() {
        skills_["NormalMeleeDamage"] = Skill{"Normal Melee Damage"};
        skills_["MeleeDefense"] = Skill{"Melee Defense"};
        skills_["Evasion"] = Skill{"Evasion"};
        skills_["AttackSpeed"] = Skill{"AttackSpeed"};
        skills_["MaxHealth"] = Skill{"Max Health"};
        skills_["CriticalDamage"] = Skill{"Critical Damage"};
        skills_["CriticalRate"] = Skill{"Critical Rate"};
        skills_["MovementSpeed"] = Skill{"Movement Speed"};
        skills_["Sword"] = Skill{"Sword"};
        skills_["Club"] = Skill{"Club"};
    }
}