//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;

    class Inventory
    /*///////////*/ {
      public:
        void Add(std::string_view, int = 1);
        void Add(std::shared_ptr<Object>);
        bool ContainsObjectOfType(std::string_view);
        void RemoveObjectOfType(std::string_view);
        std::map<int, std::shared_ptr<Object>> objects;

      private:
        static constexpr int kMaxNumberInventoryObjects = 1000;
    };

    class MovementData
    /*//////////////*/ {
      public:
        Point2F position = {0.0f, 0.0f};
        Point3 worldAreaPos = {0, 0, 0};
        float moveSpeed = 4.5f;
        bool isMoving = false;
        float stepSize = 0.015f;
        float facingAngle = 0.0f;
        float jumpHeight = 0.0f;
    };

    class StatsData
    /*///////////*/ {
      public:
        int hp = 10;
        int maxHp = 10;
        int stam = 1;
        int maxStam = 10;
    };

    class ActorData
    /*///////////*/ {
      public:
        void Update();
        int ticksLastUpdate = 0;
        float deltaT = 0;
        int exp = 0;
        MovementData movement;
        StatsData stats;
        Inventory inventory;
    };

    struct Skill
    /*////////*/ {
        std::string_view label;
        float level = 1.0f;
    };
    
    class SkillSet
    /*//////////*/ {
      public:
        SkillSet();
        std::map<std::string_view, Skill> skills;
        int availablePoints = 20;
    };
}
//////////////////////////////////////////////////////////////////////