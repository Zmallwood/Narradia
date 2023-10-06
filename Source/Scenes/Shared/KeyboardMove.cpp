//////////////////////////////////////////////////////////////////////
#include "KeyboardMove.hpp"
#include "Engine/Core/Input/KeyboardInput.hpp"
#include "Scenes/PlayScene/Modules/DestinationMove.hpp"
#include "World/Player.hpp"
//////////////////////////////////////////////////////////////////////
namespace Narradia
{
    void
    KeyboardMove::Update()
    /*///////////////////////*/
    {
        auto player = Player::GetPointer();
        if (DestinationMove::Get().GetDestination() == Point2{-1, -1})
            player->data.movement.isMoving = false;
        if (KeyboardInput::Get().KeyIsPressed(SDLK_a) && KeyboardInput::Get().KeyIsPressed(SDLK_w))
            player->TurnLeftForward();
        else if (
            KeyboardInput::Get().KeyIsPressed(SDLK_d) && KeyboardInput::Get().KeyIsPressed(SDLK_w))
            player->TurnRightForward();
        else if (
            KeyboardInput::Get().KeyIsPressed(SDLK_a) && KeyboardInput::Get().KeyIsPressed(SDLK_s))
            player->TurnLeftBack();
        else if (
            KeyboardInput::Get().KeyIsPressed(SDLK_d) && KeyboardInput::Get().KeyIsPressed(SDLK_s))
            player->TurnRightBack();
        else if (KeyboardInput::Get().KeyIsPressed(SDLK_a))
            player->TurnLeft();
        else if (KeyboardInput::Get().KeyIsPressed(SDLK_d))
            player->TurnRight();
        else if (KeyboardInput::Get().KeyIsPressed(SDLK_s))
            player->TurnBack();
        else if (KeyboardInput::Get().KeyIsPressed(SDLK_w))
            player->TurnForward();
        if ((KeyboardInput::Get().KeyIsPressed(SDLK_w) ||
             KeyboardInput::Get().KeyIsPressed(SDLK_a) ||
             KeyboardInput::Get().KeyIsPressed(SDLK_s) ||
             KeyboardInput::Get().KeyIsPressed(SDLK_d)))
        /***********************************************/
        {
            DestinationMove::Get().ResetDestination();
            player->MoveForward();
            player->data.movement.isMoving = true;
        }
    }
}
//////////////////////////////////////////////////////////////////////