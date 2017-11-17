#include "pch.h"

PlayerController::PlayerController()
{
    Clear();
}

PlayerController::~PlayerController()
{
}

void PlayerController::Clear()
{
    m_ButtonFlags = 0;
}

void PlayerController::SetButtonPressed(PlayerControllerButtons button)
{
    m_ButtonFlags |= button;
}

void PlayerController::SetButtonReleased(PlayerControllerButtons button)
{
    m_ButtonFlags &= ~button;
}

bool PlayerController::IsButtonPressed(PlayerControllerButtons button)
{
    if( m_ButtonFlags & button )
        return true;

    return false;
}
