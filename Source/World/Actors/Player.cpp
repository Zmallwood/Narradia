#include "Player.hpp"
#include "Engine/Core/Audio.hpp"
#include "Engine/Core/SceneManager.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/Camera.hpp"
#include "Scenes/SharedSceneModules/WorldDrawModule/ConfigWorldDraw.hpp"
#include "World/MapArea.hpp"
#include "World/Object.hpp"
#include "World/ObjectBehaviourList.hpp"
#include "World/ObjectsCollection.hpp"
#include "World/Tile.hpp"
#include "World/World.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Point2F
    Player::GetPosition()
    /*/////////////////*/
    {
        return data.movement_.position;
    }

    void
    Player::MoveForward()
    /*/////////////////*/
    {
        MoveAtAngle(0);
    }

    void
    Player::TurnForward()
    /*/////////////////*/
    {
        data.movement_.facing_angle = Camera::Get()->horizontal_angle_;
    }

    void
    Player::TurnRight()
    /*///////////////*/
    {
        data.movement_.facing_angle = Camera::Get()->horizontal_angle_ - 90.0f;
    }

    void
    Player::TurnLeft()
    /*//////////////*/
    {
        data.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 90.0f;
    }

    void
    Player::TurnBack()
    /*//////////////*/
    {
        data.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 180.0f;
    }

    void
    Player::TurnRightForward()
    /*//////////////////////*/
    {
        data.movement_.facing_angle = Camera::Get()->horizontal_angle_ - 45.0f;
    }

    void
    Player::TurnLeftForward()
    /*/////////////////////*/
    {
        data.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 45.0f;
    }

    void
    Player::TurnRightBack()
    /*///////////////////*/
    {
        data.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 225.0f;
    }

    void
    Player::TurnLeftBack()
    /*//////////////////*/
    {
        data.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 135.0f;
    }

    Point3F
    Player::GetSpaceCoord()
    /*///////////////////*/
    {
        auto position = GetPosition();
        Point3F spaceCoord = {position.x * kTileSize, 0.0f, position.y * kTileSize};
        spaceCoord.y -= Player::Get()->data.movement_.jump_height;
        return spaceCoord;
    }

    Point3
    Player::GetWorldAreaPos()
    /*/////////////////////*/
    {
        return data.movement_.world_area_position;
    }

    float
    Player::GetFacingAngle()
    /*////////////////////*/
    {
        return data.movement_.facing_angle;
    }

    void
    Player::SetFacingAngle(float newFacingAngle)
    /*////////////////////////////////////////*/
    {
        data.movement_.facing_angle = newFacingAngle;
    }

    void
    Player::ClaimTile(Point2 Tile)
    /*//////////////////////////*/
    {
        claimedTiles.push_back(Tile);
    }

    bool
    Player::HasClaimedTile(Point2 Tile)
    /*///////////////////////////////*/
    {
        return std::count(claimedTiles.begin(), claimedTiles.end(), Tile) != 0;
    }

    void
    Player::ClearClaimedTiles()
    /*///////////////////////*/
    {
        claimedTiles.clear();
    }

    void
    Player::Jump()
    /*//////////*/
    {
        if (SDL_GetTicks() > ticksStartJumping + jumpDuration)
            ticksStartJumping = SDL_GetTicks();
    }

    void
    Player::MoveAtAngle(float angleOffset)
    /*//////////////////////////////////*/
    {
        if (!data.movement_.is_moving)
            data.ticks_last_update_ = SDL_GetTicks();
        auto usedAngle = data.movement_.facing_angle + angleOffset - 90;
        auto dx = CosDegrees(usedAngle) * data.movement_.step_size * data.delta_t_ /
                  data.movement_.movement_speed * skillSet.skills_.at("MovementSpeed").level;
        auto dy = SinDegrees(usedAngle) * data.movement_.step_size * data.delta_t_ /
                  data.movement_.movement_speed * skillSet.skills_.at("MovementSpeed").level;
        if (SceneManager::Get()->GetCurrentScene() == SceneNames::Editor)
        /*******************************************************/
        {
            dx *= 2;
            dy *= 2;
        }
        auto mapArea = World::Get()->GetMapAreaAtZLevel(data.movement_.world_area_position.z);
        auto tileBeforeMove = mapArea->GetTile(data.movement_.position.ToIntPoint());
        auto newX = data.movement_.position.x - dx;
        auto newY = data.movement_.position.y + dy;
        auto tileNew = mapArea->GetTile({static_cast<int>(newX), static_cast<int>(newY)});
        if (SceneManager::Get()->GetCurrentScene() != SceneNames::Editor)
        /*******************************************************/
        {
            for (auto objectEntry : tileBeforeMove->GetObjects().list)
            /********************************************************/
            {
                if (ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()))
                /********************************************************************/
                {
                    if ((ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockN) != 0 &&
                        (int)newY == (int)data.movement_.position.y - 1)
                    /**********************************************************************/
                    {
                        data.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockS) != 0 &&
                        (int)newY == (int)data.movement_.position.y + 1)
                    /**********************************************************************/
                    {
                        data.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockE) != 0 &&
                        (int)newX == (int)data.movement_.position.x + 1)
                    /**********************************************************************/
                    {
                        data.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockW) != 0 &&
                        (int)newX == (int)data.movement_.position.x - 1)
                    /**********************************************************************/
                    {
                        data.delta_t_ = 0;
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
                        data.delta_t_ = 0;
                        return;
                    }
                }
                if (ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()))
                /********************************************************************/
                {
                    if (ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                        (int)ObjectBehaviourFlags::MovementBlock != 0)
                    /*********************************************************************/
                    {
                        data.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockN) != 0 &&
                        (int)newY == (int)data.movement_.position.y + 1)
                    /**********************************************************************/
                    {
                        data.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockS) != 0 &&
                        (int)newY == (int)data.movement_.position.y - 1)
                    /**********************************************************************/
                    {
                        data.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockE) != 0 &&
                        (int)newX == (int)data.movement_.position.x - 1)
                    /**********************************************************************/
                    {
                        data.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockW) != 0 &&
                        (int)newX == (int)data.movement_.position.x + 1)
                    /**********************************************************************/
                    {
                        data.delta_t_ = 0;
                        return;
                    }
                }
            }
        }
        if (newX != data.movement_.position.x || newY != data.movement_.position.y)
        /***********************************************************************/
        {
            data.movement_.position.x = newX;
            data.movement_.position.y = newY;
            Audio::Get()->PlaySound("Footsteps");
            data.delta_t_ = 0;
        }
        auto tile = mapArea->GetTile(data.movement_.position.ToIntPoint());
        for (auto objectEntry : tile->GetObjects().list)
        /**********************************************/
        {
            if (objectEntry->GetObjectType() == Hash("ObjectMineEntrance") &&
                false == tileBeforeMove->GetObjects().Contains(Hash("ObjectMineEntrance")))
            /*****************************************************************************/
            {
                data.movement_.world_area_position.z--;
                auto mapArea = World::Get()->GetMapAreaAtZLevel(data.movement_.world_area_position.z);
                tile = mapArea->GetTile(data.movement_.position.ToIntPoint());
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
                data.movement_.world_area_position.z++;
            }
        }
        tile = mapArea->GetTile(data.movement_.position.ToIntPoint());
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
    /*////////////*/
    {
        data.Update();
        if (!data.movement_.is_moving)
            Audio::Get()->StopPlaySound();

        if (SDL_GetTicks() > ticksLastStaminaRegeneration + kStaminaRegenerationSpeed)
        /****************************************************************************/
        {
            data.status_.stamina = std::min(data.status_.max_stamina, data.status_.stamina + 1);
            ticksLastStaminaRegeneration = SDL_GetTicks();
        }

        if (SDL_GetTicks() < ticksStartJumping + jumpDuration)
        /****************************************************/
        {
            auto jumpProgress =
                (SDL_GetTicks() - ticksStartJumping) * 2 / static_cast<float>(jumpDuration);
            auto jumpHeight = -std::pow(jumpProgress - 1.0f, 2) + 1.0f;
            data.movement_.jump_height = -jumpHeight * maxJumpHeight * kTileSize;
        }
        else
        /**/
        {
            data.movement_.jump_height = 0.0f;
        }
    }
}