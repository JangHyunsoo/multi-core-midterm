#include "Player.h"

DEFINITION_SINGLE(Player)

Player::Player()
{
}

Player::~Player()
{
	DeleteObject(m_pBrush);
	DeleteObject(m_pWhite);
}
