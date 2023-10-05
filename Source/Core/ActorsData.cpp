//////////////////////////////////////////////////////////////////////
#include "ActorsData.hpp"
#include "Universe/Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void Inventory::Add(std::string_view objectName, int quantity)
    /*//////////////////////////////////////////////////////////*/ {
        for (auto i = 0; i < kMaxNumberInventoryObjects; i++)
        /***************************************************/ {
            if (objects.count(i) == 0)
            /************************/ {
                auto newObject = std::make_shared<Object>(objectName, quantity);
                objects.insert({i, newObject});
                return;
            }
        }
    } // Function

    void Inventory::Add(std::shared_ptr<Object> object)
    /*///////////////////////////////////////////////*/ {
        for (auto i = 0; i < kMaxNumberInventoryObjects; i++)
        /***************************************************/ {
            if (objects.count(i) == 0)
            /************************/ {
                objects.insert({i, object});
                return;
            }
        }
    } // Function

    bool Inventory::ContainsObjectOfType(std::string_view objectType)
    /*/////////////////////////////////////////////////////////////*/ {
        for (auto &entry : objects)
            if (entry.second->GetObjectType() == Hash(objectType)) return true;
        return false;
    } // Function

    void Inventory::RemoveObjectOfType(std::string_view objectType)
    /*///////////////////////////////////////////////////////////*/ {
        for (auto &entry : objects)
        /*************************/ {
            if (entry.second->GetObjectType() == Hash(objectType))
            /****************************************************/ {
                objects.erase(entry.first);
                return;
            }
        }
    } // Function

    void ActorData::Update()
    /*////////////////////*/ {
        if (movement.isMoving)
            if (ticksLastUpdate != 0) deltaT += (SDL_GetTicks() - ticksLastUpdate);
        ticksLastUpdate = SDL_GetTicks();
    }

    SkillSet::SkillSet()
    /*////////////////*/ {
        skills["NormalMeleeDamage"] = Skill{"Normal Melee Damage"};
        skills["MeleeDefense"] = Skill{"Melee Defense"};
        skills["Evasion"] = Skill{"Evasion"};
        skills["AttackSpeed"] = Skill{"AttackSpeed"};
        skills["MaxHealth"] = Skill{"Max Health"};
        skills["CriticalDamage"] = Skill{"Critical Damage"};
        skills["CriticalRate"] = Skill{"Critical Rate"};
        skills["MovementSpeed"] = Skill{"Movement Speed"};
    } // Function
};
//////////////////////////////////////////////////////////////////////