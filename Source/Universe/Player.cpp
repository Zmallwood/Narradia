//////////////////////////////////////////////////////////////////////
#include "Player.hpp"
#include "Scenes/Shared/WorldDrawModule/Camera.hpp"
#include "Scenes/Shared/WorldDrawModule/Configuration.hpp"
#include "World.hpp"
#include "MapArea.hpp"
#include "Tile.hpp"
#include "ObjectsCollection.hpp"
#include "ObjectBehaviourList.hpp"
#include "Object.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    Point2F Player::GetPosition()
    /*/////////////////////////*/ { return data.movement.position; } // Function

    void Player::MoveForward()
    /*//////////////////////*/ { MoveAtAngle(0); } // Function

    void Player::TurnForward()
    /*//////////////////////*/ {
        data.movement.facingAngle = Camera::Get().horizontalAngle;
    } // Function

    void Player::TurnRight()
    /*////////////////////*/ {
        data.movement.facingAngle = Camera::Get().horizontalAngle - 90.0f;
    } // Function

    void Player::TurnLeft()
    /*///////////////////*/ {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 90.0f;
    } // Function

    void Player::TurnBack()
    /*///////////////////*/ {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 180.0f;
    } // Function

    void Player::TurnRightForward()
    /*///////////////////////////*/ {
        data.movement.facingAngle = Camera::Get().horizontalAngle - 45.0f;
    } // Function

    void Player::TurnLeftForward()
    /*//////////////////////////*/ {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 45.0f;
    } // Function

    void Player::TurnRightBack()
    /*////////////////////////*/ {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 225.0f;
    } // Function

    void Player::TurnLeftBack()
    /*///////////////////////*/ {
        data.movement.facingAngle = Camera::Get().horizontalAngle + 135.0f;
    } // Function

    Point3F Player::GetSpaceCoord()
    /*///////////////////////////*/ {
        auto position = GetPosition();
        Point3F spaceCoord = {position.x * kTileSize, 0.0f, position.y * kTileSize};
        spaceCoord.y -= Player::Get().data.movement.jumpHeight;
        return spaceCoord;
    } // Function

    Point3 Player::GetWorldAreaPos()
    /*////////////////////////////*/ { return data.movement.worldAreaPos; } // Function

    float Player::GetFacingAngle()
    /*//////////////////////////*/ { return data.movement.facingAngle; } // Function

    void Player::SetFacingAngle(float newFacingAngle)
    /*/////////////////////////////////////////////*/ {
        data.movement.facingAngle = newFacingAngle;
    } // Function

    void Player::ClaimTile(Point2 Tile)
    /*///////////////////////////////*/ { claimedTiles.push_back(Tile); } // Function

    bool Player::HasClaimedTile(Point2 Tile)
    /*////////////////////////////////////*/ {
        return std::count(claimedTiles.begin(), claimedTiles.end(), Tile) != 0;
    } // Function

    void Player::ClearClaimedTiles()
    /*////////////////////////////*/ { claimedTiles.clear(); } // Function

    void Player::Jump()
    /*///////////////*/ {
        if (SDL_GetTicks() > ticksStartJumping + jumpDuration) ticksStartJumping = SDL_GetTicks();
    } // Function

    void Player::MoveAtAngle(float angleOffset)
    /*///////////////////////////////////////*/ {
        if (!data.movement.isMoving) data.ticksLastUpdate = SDL_GetTicks();
        auto usedAngle = data.movement.facingAngle + angleOffset - 90;
        auto dx = CosDegrees(usedAngle) * data.movement.stepSize * data.deltaT /
                  data.movement.moveSpeed * skillSet.skills.at("MovementSpeed").level;
        auto dy = SinDegrees(usedAngle) * data.movement.stepSize * data.deltaT /
                  data.movement.moveSpeed * skillSet.skills.at("MovementSpeed").level;
        if (SceneManager::Get().GetCurrentView() == Scenes::Editor)
        /*******************************************************/ {
            dx *= 2;
            dy *= 2;
        }
        auto mapArea = World::Get().GetMapAreaAtZLevel(data.movement.worldAreaPos.z);
        auto tileBeforeMove = mapArea->GetTile(data.movement.position.ToIntPoint());
        auto newX = data.movement.position.x - dx;
        auto newY = data.movement.position.y + dy;
        auto tileNew = mapArea->GetTile({static_cast<int>(newX), static_cast<int>(newY)});
        if (SceneManager::Get().GetCurrentView() != Scenes::Editor)
        /*******************************************************/ {
            for (auto objectEntry : tileBeforeMove->GetObjects().list)
            /********************************************************/ {
                if (ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()))
                /********************************************************************/ {
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockN) != 0 &&
                        (int)newY == (int)data.movement.position.y - 1)
                    /**********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockS) != 0 &&
                        (int)newY == (int)data.movement.position.y + 1)
                    /**********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockE) != 0 &&
                        (int)newX == (int)data.movement.position.x + 1)
                    /**********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockW) != 0 &&
                        (int)newX == (int)data.movement.position.x - 1)
                    /**********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                }
            }
            for (auto objectEntry : tileNew->GetObjects().list)
            /*************************************************/ {
                if (objectEntry->GetObjectType() == Hash("ObjectCaveBlock"))
                /**********************************************************/ {
                    if (SDL_GetTicks() < tickLastTimeDidMine + mineSpeed)
                    /***************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                }
                if (ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()))
                /********************************************************************/ {
                    if (ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                        (int)ObjectBehaviourFlags::MovementBlock != 0)
                    /*********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockN) != 0 &&
                        (int)newY == (int)data.movement.position.y + 1)
                    /**********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockS) != 0 &&
                        (int)newY == (int)data.movement.position.y - 1)
                    /**********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockE) != 0 &&
                        (int)newX == (int)data.movement.position.x - 1)
                    /**********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                    if ((ObjectBehaviourList::Get().GetFlags(objectEntry->GetObjectType()) &
                         (int)ObjectBehaviourFlags::MovementBlockW) != 0 &&
                        (int)newX == (int)data.movement.position.x + 1)
                    /**********************************************************************/ {
                        data.deltaT = 0;
                        return;
                    }
                }
            }
        }
        if (newX != data.movement.position.x || newY != data.movement.position.y)
        /***********************************************************************/ {
            data.movement.position.x = newX;
            data.movement.position.y = newY;
            Audio::Get().PlaySound("Footsteps");
            data.deltaT = 0;
        }
        auto tile = mapArea->GetTile(data.movement.position.ToIntPoint());
        for (auto objectEntry : tile->GetObjects().list)
        /**********************************************/ {
            if (objectEntry->GetObjectType() == Hash("ObjectMineEntrance") &&
                false == tileBeforeMove->GetObjects().Contains(Hash("ObjectMineEntrance")))
            /*****************************************************************************/ {
                data.movement.worldAreaPos.z--;
                auto mapArea = World::Get().GetMapAreaAtZLevel(data.movement.worldAreaPos.z);
                tile = mapArea->GetTile(data.movement.position.ToIntPoint());
                if (false == tile->GetObjects().Contains(Hash("ObjectMineExit")))
                /***************************************************************/ {
                    tile->ClearObjects();
                    auto mineExit = std::make_shared<Object>("ObjectMineExit");
                    mineExit->SetModelScaling(0.7f);
                    tile->AddObject(mineExit);
                }
                tickLastTimeDidMine = SDL_GetTicks();
            } else if (
                objectEntry->GetObjectType() == Hash("ObjectMineExit") &&
                false == tileBeforeMove->GetObjects().Contains(Hash("ObjectMineExit")))
            /*************************************************************************/ {
                data.movement.worldAreaPos.z++;
            }
        }
        tile = mapArea->GetTile(data.movement.position.ToIntPoint());
        for (auto objectEntry : tile->GetObjects().list)
        /**********************************************/ {
            if (objectEntry->GetObjectType() == Hash("ObjectCaveBlock"))
            /**********************************************************/ {
                if (false == tile->GetObjects().Contains(Hash("ObjectMineExit")))
                /***************************************************************/ {
                    tile->ReplaceObject(objectEntry, std::make_shared<Object>("ObjectStone"));
                    objectEntry->SetModelScaling(
                        0.5f + rand() / static_cast<float>(RAND_MAX) * 0.7f);
                    tickLastTimeDidMine = SDL_GetTicks();
                }
            }
        }
    } // Function

    void Player::Update()
    /*/////////////////*/ {
        data.Update();
        if (!data.movement.isMoving) Audio::Get().StopPlaySound();

        if (SDL_GetTicks() < ticksStartJumping + jumpDuration)
        /****************************************************/ {
            auto jumpProgress =
                (SDL_GetTicks() - ticksStartJumping) * 2 / static_cast<float>(jumpDuration);
            auto jumpHeight = -std::pow(jumpProgress - 1.0f, 2) + 1.0f;
            data.movement.jumpHeight = -jumpHeight * maxJumpHeight * kTileSize;
        } else
        /****/ {
            data.movement.jumpHeight = 0.0f;
        }
    } // Function
}
//////////////////////////////////////////////////////////////////////