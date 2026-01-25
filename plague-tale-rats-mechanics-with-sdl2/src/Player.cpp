#include "Player.h"
#include "World.h"
#include "Body.h"

Player::Player(std::unique_ptr<Body> body, const char* textureFileName): Entity(std::move(body), textureFileName)
{
	type = PLAYER;
	std::cout << "Creating Player" << std::endl;
}

Player::~Player()
{
	std::cout << "Deleting Player" << std::endl;
}

void Player::Update(float dt)
{
    Entity::Update(dt);
}
