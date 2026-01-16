#include "Player.h"

Player::Player(Body* body, const char* textureFileName): Entity(body, textureFileName)
{
	type = PLAYER;
	std::cout << "Creating Player" << std::endl;

}

Player::~Player()
{
	std::cout << "Deleting Player" << std::endl;
}

void Player::SetNewMove(const Vec2& pos)
{
	newPos = pos;
	isMoving = true;
}

void Player::Update(float dt)
{
	Entity::Update(dt);
}
