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
        return data_.movement_.position;
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
        data_.movement_.facing_angle = Camera::Get()->horizontal_angle_;
    }

    void
    Player::TurnRight()
    /*///////////////*/
    {
        data_.movement_.facing_angle = Camera::Get()->horizontal_angle_ - 90.0f;
    }

    void
    Player::TurnLeft()
    /*//////////////*/
    {
        data_.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 90.0f;
    }

    void
    Player::TurnBack()
    /*//////////////*/
    {
        data_.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 180.0f;
    }

    void
    Player::TurnRightForward()
    /*//////////////////////*/
    {
        data_.movement_.facing_angle = Camera::Get()->horizontal_angle_ - 45.0f;
    }

    void
    Player::TurnLeftForward()
    /*/////////////////////*/
    {
        data_.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 45.0f;
    }

    void
    Player::TurnRightBack()
    /*///////////////////*/
    {
        data_.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 225.0f;
    }

    void
    Player::TurnLeftBack()
    /*//////////////////*/
    {
        data_.movement_.facing_angle = Camera::Get()->horizontal_angle_ + 135.0f;
    }

    Point3F
    Player::GetSpaceCoord()
    /*///////////////////*/
    {
        auto position = GetPosition();
        Point3F space_coordinate = {position.x * kTileSize, 0.0f, position.y * kTileSize};
        space_coordinate.y -= Player::Get()->data_.movement_.jump_height;
        return space_coordinate;
    }

    Point3
    Player::GetWorldAreaPos()
    /*/////////////////////*/
    {
        return data_.movement_.world_area_position;
    }

    float
    Player::GetFacingAngle()
    /*////////////////////*/
    {
        return data_.movement_.facing_angle;
    }

    void
    Player::SetFacingAngle(float newFacingAngle)
    /*////////////////////////////////////////*/
    {
        data_.movement_.facing_angle = newFacingAngle;
    }

    void
    Player::ClaimTile(Point2 Tile)
    /*//////////////////////////*/
    {
        claimed_tiles_.push_back(Tile);
    }

    bool
    Player::HasClaimedTile(Point2 Tile)
    /*///////////////////////////////*/
    {
        return std::count(claimed_tiles_.begin(), claimed_tiles_.end(), Tile) != 0;
    }

    void
    Player::ClearClaimedTiles()
    /*///////////////////////*/
    {
        claimed_tiles_.clear();
    }

    void
    Player::Jump()
    /*//////////*/
    {
        if (SDL_GetTicks() > ticks_start_jumping_ + jump_duration_)
            ticks_start_jumping_ = SDL_GetTicks();
    }

    void
    Player::MoveAtAngle(float angleOffset)
    /*//////////////////////////////////*/
    {
        if (!data_.movement_.is_moving)
            data_.ticks_last_update_ = SDL_GetTicks();
        auto used_angle = data_.movement_.facing_angle + angleOffset - 90;
        auto dx = CosDegrees(used_angle) * data_.movement_.step_size * data_.delta_t_ /
                  data_.movement_.movement_speed * skill_set_.skills_.at("MovementSpeed").level;
        auto dy = SinDegrees(used_angle) * data_.movement_.step_size * data_.delta_t_ /
                  data_.movement_.movement_speed * skill_set_.skills_.at("MovementSpeed").level;
        if (SceneManager::Get()->GetCurrentScene() == SceneNames::Editor)
        /*******************************************************/
        {
            dx *= 2;
            dy *= 2;
        }
        auto map_area = World::Get()->GetMapAreaAtZLevel(data_.movement_.world_area_position.z);
        auto tile_before_move = map_area->GetTile(data_.movement_.position.ToIntPoint());
        auto new_x = data_.movement_.position.x - dx;
        auto new_y = data_.movement_.position.y + dy;
        auto tile_new = map_area->GetTile({static_cast<int>(new_x), static_cast<int>(new_y)});
        if (SceneManager::Get()->GetCurrentScene() != SceneNames::Editor)
        /*******************************************************/
        {
            for (auto object_entry : tile_before_move->GetObjects().list_)
            /********************************************************/
            {
                if (ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()))
                /********************************************************************/
                {
                    if ((ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockN) != 0 &&
                        (int)new_y == (int)data_.movement_.position.y - 1)
                    /**********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockS) != 0 &&
                        (int)new_y == (int)data_.movement_.position.y + 1)
                    /**********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockE) != 0 &&
                        (int)new_x == (int)data_.movement_.position.x + 1)
                    /**********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockW) != 0 &&
                        (int)new_x == (int)data_.movement_.position.x - 1)
                    /**********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                }
            }
            for (auto object_entry : tile_new->GetObjects().list_)
            /*************************************************/
            {
                if (object_entry->GetObjectType() == Hash("ObjectCaveBlock"))
                /**********************************************************/
                {
                    if (SDL_GetTicks() < ticks_last_time_did_mine_ + mine_speed_)
                    /***************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                }
                if (ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()))
                /********************************************************************/
                {
                    if (ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                        (int)ObjectBehaviourFlags::MovementBlock != 0)
                    /*********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockN) != 0 &&
                        (int)new_y == (int)data_.movement_.position.y + 1)
                    /**********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockS) != 0 &&
                        (int)new_y == (int)data_.movement_.position.y - 1)
                    /**********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockE) != 0 &&
                        (int)new_x == (int)data_.movement_.position.x - 1)
                    /**********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get()->GetFlags(object_entry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockW) != 0 &&
                        (int)new_x == (int)data_.movement_.position.x + 1)
                    /**********************************************************************/
                    {
                        data_.delta_t_ = 0;
                        return;
                    }
                }
            }
        }
        if (new_x != data_.movement_.position.x || new_y != data_.movement_.position.y)
        /***********************************************************************/
        {
            data_.movement_.position.x = new_x;
            data_.movement_.position.y = new_y;
            Audio::Get()->PlaySound("Footsteps");
            data_.delta_t_ = 0;
        }
        auto tile = map_area->GetTile(data_.movement_.position.ToIntPoint());
        for (auto object_entry : tile->GetObjects().list_)
        /**********************************************/
        {
            if (object_entry->GetObjectType() == Hash("ObjectMineEntrance") &&
                false == tile_before_move->GetObjects().Contains(Hash("ObjectMineEntrance")))
            /*****************************************************************************/
            {
                data_.movement_.world_area_position.z--;
                auto map_area = World::Get()->GetMapAreaAtZLevel(data_.movement_.world_area_position.z);
                tile = map_area->GetTile(data_.movement_.position.ToIntPoint());
                if (false == tile->GetObjects().Contains(Hash("ObjectMineExit")))
                /***************************************************************/
                {
                    tile->ClearObjects();
                    auto mine_exit = std::make_shared<Object>("ObjectMineExit");
                    mine_exit->SetModelScaling(0.7f);
                    tile->AddObject(mine_exit);
                }
                ticks_last_time_did_mine_ = SDL_GetTicks();
            }
            else if (
                object_entry->GetObjectType() == Hash("ObjectMineExit") &&
                false == tile_before_move->GetObjects().Contains(Hash("ObjectMineExit")))
            /*************************************************************************/
            {
                data_.movement_.world_area_position.z++;
            }
        }
        tile = map_area->GetTile(data_.movement_.position.ToIntPoint());
        for (auto object_entry : tile->GetObjects().list_)
        /**********************************************/
        {
            if (object_entry->GetObjectType() == Hash("ObjectCaveBlock"))
            /**********************************************************/
            {
                if (false == tile->GetObjects().Contains(Hash("ObjectMineExit")))
                /***************************************************************/
                {
                    tile->ReplaceObject(object_entry, std::make_shared<Object>("ObjectStone"));
                    object_entry->SetModelScaling(
                        0.5f + rand() / static_cast<float>(RAND_MAX) * 0.7f);
                    ticks_last_time_did_mine_ = SDL_GetTicks();
                }
            }
        }
    }

    void
    Player::Update()
    /*////////////*/
    {
        data_.Update();
        if (!data_.movement_.is_moving)
            Audio::Get()->StopPlaySound();

        if (SDL_GetTicks() > ticks_last_stamina_regeneration_ + kStaminaRegenerationSpeed)
        /****************************************************************************/
        {
            data_.status_.stamina = std::min(data_.status_.max_stamina, data_.status_.stamina + 1);
            ticks_last_stamina_regeneration_ = SDL_GetTicks();
        }

        if (SDL_GetTicks() < ticks_start_jumping_ + jump_duration_)
        /****************************************************/
        {
            auto jump_progress =
                (SDL_GetTicks() - ticks_start_jumping_) * 2 / static_cast<float>(jump_duration_);
            auto jump_height = -std::pow(jump_progress - 1.0f, 2) + 1.0f;
            data_.movement_.jump_height = -jump_height * max_jump_height_ * kTileSize;
        }
        else
        /**/
        {
            data_.movement_.jump_height = 0.0f;
        }
    }
}