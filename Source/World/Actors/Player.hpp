//////////////////////////////////////////////////////////////////////
#pragma once
#include "ActorsData.hpp"
#include "PlayerBuilds.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    class Player : public Singleton<Player>
    /*///////////////////////////////////*/
    {
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
        std::vector<Point2> claimedTiles;
        ActorData data;
        SkillSet skillSet;
        bool mounted = false;

        PlayerBuilds playerBuild = PlayerBuilds::None;

      private:
        void Update();
        void MoveAtAngle(float);
        int mineSpeed = 800;
        int tickLastTimeDidMine = 0;
        int ticksStartJumping = 0;
        int jumpDuration = 1000;
        float maxJumpHeight = 1.5f;
        friend class PlayScene;
        friend class EditorScene;
    };
}