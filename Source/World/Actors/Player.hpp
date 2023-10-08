#pragma once
#include "ActorsData.hpp"
#include "PlayerBuilds.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Player : public Singleton<Player> {
      public:
        Point2F GetPosition();
        void MoveForward();
        void TurnForward();
        void TurnRight();
        void TurnLeft();
        void TurnBack();
        void TurnRightForward();
        void TurnLeftForward();
        void TurnRightBack();
        void TurnLeftBack();
        Point3F GetSpaceCoord();
        Point3 GetWorldAreaPos();
        float GetFacingAngle();
        void SetFacingAngle(float);
        void ClaimTile(Point2);
        bool HasClaimedTile(Point2);
        void ClearClaimedTiles();
        void Jump();

        std::vector<Point2> claimed_tiles_;
        ActorData data_;
        SkillSet skill_set_;
        bool mounted_ = false;
        PlayerBuilds current_player_build_ = PlayerBuilds::None;

      private:
        void Update();
        void MoveAtAngle(float);

        inline static const int kStaminaRegenerationSpeed = 4500;
        int mine_speed_ = 800;
        int ticks_last_time_did_mine_ = 0;
        int ticks_start_jumping_ = 0;
        int jump_duration_ = 1000;
        int ticks_last_stamina_regeneration_ = 0;
        float max_jump_height_ = 1.5f;

        friend class PlayScene;
        friend class EditorScene;
    };
}