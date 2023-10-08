#pragma once
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Object;

    class Inventory
    /*///////////*/
    {
      public:
        void Add(std::string_view, int = 1);
        void Add(std::shared_ptr<Object>);
        bool ContainsObjectOfType(std::string_view);
        void RemoveObjectOfType(std::string_view);

        std::map<int, std::shared_ptr<Object>> objects_;

      private:
        static constexpr int kMaxNumberInventoryObjects = 1000;
    };

    struct MovementData
    /*//////////////*/
    {
        Point2F position = {0.0f, 0.0f};
        Point3 world_area_position = {0, 0, 0};
        float movement_speed = 4.5f;
        bool is_moving = false;
        float step_size = 0.015f;
        float facing_angle = 0.0f;
        float jump_height = 0.0f;
    };

    struct StatusData
    /*///////////*/
    {
        int health = 10;
        int max_health = 10;
        int stamina = 10;
        int max_stamina = 10;
        int rage = 0;
        int max_rage = 10;
    };

    struct CombatData {
        int ticks_last_hit_given_other = 0;
        int attack_speed = 1000;
    };

    class ActorData
    /*///////////*/
    {
      public:
        void Update();

        int ticks_last_update_ = 0;
        float delta_t_ = 0;
        int experience_ = 0;
        MovementData movement_;
        StatusData status_;
        Inventory inventory_;
        CombatData combat_;
    };

    struct Skill
    /*///////*/
    {
        std::string_view label;
        float level = 1.0f;
    };

    class SkillSet
    /*//////////*/
    {
      public:
        SkillSet();

        std::map<std::string_view, Skill> skills_;
        int available_points_ = 20;
    };
}