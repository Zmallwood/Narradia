//////////////////////////////////////////////////////////////////////
#include "Player.hpp"
#include "Engine/Core/Audio.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "MapArea.hpp"
#include "Object.hpp"
#include "ObjectBehaviourList.hpp"
#include "ObjectsCollection.hpp"
#include "Scenes/Shared/WorldDrawModule/Camera.hpp"
#include "Scenes/Shared/WorldDrawModule/Configuration.hpp"
#include "Tile.hpp"
#include "World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Point2F
    Player::GetPosition()
    /*/////////////////////////*/
    {
        return data.movement.position;
    }

    void
    Player::MoveForward()
    /*//////////////////////*/
    {
        MoveAtAngle(0);
    }

    void
    Player::TurnForward()
    /*//////////////////////*/
    {
        data.movement.facingAngle = Camera::Get().horizontalAngle;
    }

    void
    Player::TurnRight()
    /*////////////////////*/
    {
        data.movement.facingAngle = Camera::Get().horizontalAngle - 90.0f;
    }

    void
    Player::TurnLeft()
    /*///////////////////*/
    {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 90.0f;
    }

    void
    Player::TurnBack()
    /*///////////////////*/
    {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 180.0f;
    }

    void
    Player::TurnRightForward()
    /*///////////////////////////*/
    {
        data.movement.facingAngle = Camera::Get().horizontalAngle - 45.0f;
    }

    void
    Player::TurnLeftForward()
    /*//////////////////////////*/
    {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 45.0f;
    }

    void
    Player::TurnRightBack()
    /*////////////////////////*/
    {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 225.0f;
    }

    void
    Player::TurnLeftBack()
    /*///////////////////////*/
    {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 135.0f;
    }

    Point3F
    Player::GetSpaceCoord()
    /*///////////////////////////*/
    {
        auto position = GetPosition();
        Point3F spaceCoord = {position.x * kTileSize, 0.0f, position.y * kTileSize};
        spaceCoord.y -= Player::Get().data.movement.jumpHeight;
        return spaceCoord;
    }

    Point3
    Player::GetWorldAreaPos()
    /*////////////////////////////*/
    {
        return data.movement.worldAreaPos;
    }

    float
    Player::GetFacingAngle()
    /*//////////////////////////*/
    {
        return data.movement.facingAngle;
    }

    void
    Player::SetFacingAngle(float newFacingAngle)
    /*/////////////////////////////////////////////*/
    {
        data.movement.facingAngle = newFacingAngle;
    }

    void
    Player::ClaimTile(Point2 Tile)
    /*///////////////////////////////*/
    {
        claimedTiles.push_back(Tile);
    }

    bool
    Player::HasClaimedTile(Point2 Tile)
    /*////////////////////////////////////*/
    {
        return std::count(claimedTiles.begin(), claimedTiles.end(), Tile) != 0;
    }

    void
    Player::ClearClaimedTiles()
    /*////////////////////////////*/
    {
        claimedTiles.clear();
    }

    void
    Player::Jump()
    /*///////////////*/
    {
        if (SDL_GetTicks() > ticksStartJumping + jumpDuration)
            ticksStartJumping = SDL_GetTicks();
    }

    void
    Player::MoveAtAngle(float angleOffset)
    /*///////////////////////////////////////*/
    {
        if (!data.movement.isMoving)
            data.ticksLastUpdate = SDL_GetTicks();
        auto usedAngle = data.movement.facingAngle + angleOffset - 90;
        auto dx = CosDegrees(usedAngle) * data.movement.stepSize * data.deltaT /
                  data.movement.moveSpeed * skillSet.skills.at("MovementSpeed").level;
        auto dy = SinDegrees(usedAngle) * data.movement.stepSize * data.deltaT /
                  data.movement.moveSpeed * skillSet.skills.at("MovementSpeed").level;
        if (SceneManager::Get().GetCurrentScene() == SceneNames::Editor)
        /*******************************************************/
        {
            dx *= 2;
            dy *= 2;
        }
        auto mapArea = World::Get().GetMapAreaAtZLevel(data.movement.worldAreaPos.z);
        auto tileBeforeMove = mapArea->GetTile(data.movement.position.ToIntPoint());
        auto newX = data.movement.position.x - dx;
        auto newY = data.movement.position.y + dy;
        auto tileNew = mapArea->GetTile({static_cast<int>(newX), static_cast<int>(newY)});
        if (SceneManager::Get().GetCurrentScene() != SceneNames::Editor)
        /*******************************************************/
        {
            for (auto objectEntry : tileBeforeMove->GetObjects().list)
            /********************************************************/
            {
                if (ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()))
                /********************************************************************/
                {
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockN) != 0 &&
                        (int)newY == (int)data.movement.position.y - 1)
                    /**********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockS) != 0 &&
                        (int)newY == (int)data.movement.position.y + 1)
                    /**********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockE) != 0 &&
                        (int)newX == (int)data.movement.position.x + 1)
                    /**********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockW) != 0 &&
                        (int)newX == (int)data.movement.position.x - 1)
                    /**********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                }
            }
            for (auto objectEntry : tileNew->GetObjects().list)
            /*************************************************/
            {
                if (objectEntry->GetObjectType() == Hash("ObjectCaveBlock"))
                /**********************************************************/
                {
                    if (SDL_GetTicks() < tickLastTimeDidMine + mineSpeed)
                    /***************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                }
                if (ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()))
                /********************************************************************/
                {
                    if (ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                        (int)ObjectBehaviourFlags::MovementBlock != 0)
                    /*********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockN) != 0 &&
                        (int)newY == (int)data.movement.position.y + 1)
                    /**********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockS) != 0 &&
                        (int)newY == (int)data.movement.position.y - 1)
                    /**********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockE) != 0 &&
                        (int)newX == (int)data.movement.position.x - 1)
                    /**********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockW) != 0 &&
                        (int)newX == (int)data.movement.position.x + 1)
                    /**********************************************************************/
                    {
                        data.deltaT = 0;
                        return;
                    }
                }
            }
        }
        if (newX != data.movement.position.x || newY != data.movement.position.y)
        /***********************************************************************/
        {
            data.movement.position.x = newX;
            data.movement.position.y = newY;
            Audio::Get().PlaySound("Footsteps");
            data.deltaT = 0;
        }
        auto tile = mapArea->GetTile(data.movement.position.ToIntPoint());
        for (auto objectEntry : tile->GetObjects().list)
        /**********************************************/
        {
            if (objectEntry->GetObjectType() == Hash("ObjectMineEntrance") &&
                false == tileBeforeMove->GetObjects().Contains(Hash("ObjectMineEntrance")))
            /*****************************************************************************/
            {
                data.movement.worldAreaPos.z--;
                auto mapArea = World::Get().GetMapAreaAtZLevel(data.movement.worldAreaPos.z);
                tile = mapArea->GetTile(data.movement.position.ToIntPoint());
                if (false == tile->GetObjects().Contains(Hash("ObjectMineExit")))
                /***************************************************************/
                {
                    tile->ClearObjects();
                    auto mineExit = std::make_shared<Object>("ObjectMineExit");
                    mineExit->SetModelScaling(0.7f);
                    tile->AddObject(mineExit);
                }
                tickLastTimeDidMine = SDL_GetTicks();
            }
            else if (
                objectEntry->GetObjectType() == Hash("ObjectMineExit") &&
                false == tileBeforeMove->GetObjects().Contains(Hash("ObjectMineExit")))
            /*************************************************************************/
            {
                data.movement.worldAreaPos.z++;
            }
        }
        tile = mapArea->GetTile(data.movement.position.ToIntPoint());
        for (auto objectEntry : tile->GetObjects().list)
        /**********************************************/
        {
            if (objectEntry->GetObjectType() == Hash("ObjectCaveBlock"))
            /**********************************************************/
            {
                if (false == tile->GetObjects().Contains(Hash("ObjectMineExit")))
                /***************************************************************/
                {
                    tile->ReplaceObject(objectEntry, std::make_shared<Object>("ObjectStone"));
                    objectEntry->SetModelScaling(
                        0.5f + rand() / static_cast<float>(RAND_MAX) * 0.7f);
                    tickLastTimeDidMine = SDL_GetTicks();
                }
            }
        }
    }

    void
    Player::Update()
    /*/////////////////*/
    {
        data.Update();
        if (!data.movement.isMoving)
            Audio::Get().StopPlaySound();

        if (SDL_GetTicks() < ticksStartJumping + jumpDuration)
        /****************************************************/
        {
            auto jumpProgress =
                (SDL_GetTicks() - ticksStartJumping) * 2 / static_cast<float>(jumpDuration);
            auto jumpHeight = -std::pow(jumpProgress - 1.0f, 2) + 1.0f;
            data.movement.jumpHeight = -jumpHeight * maxJumpHeight * kTileSize;
        }
        else
        /**/
        {
            data.movement.jumpHeight = 0.0f;
        }
    }
}
//////////////////////////////////////////////////////////////////////