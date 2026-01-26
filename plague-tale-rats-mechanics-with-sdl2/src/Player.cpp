#include "Player.h"
#include "World.h"
#include "Body.h"
#include "../Rat.h"
#include "Constants.h"
#include "Graphics.h"

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
	_movementDirection = _movementDirection.Normalize();
	body->velocity = _movementDirection * body->maxVelocity;
    Entity::Update(dt);
	
	for(auto& rat : World::instance->rats) {
		Vec2 toRat = rat->body->position - body->position;
		float distToRat = toRat.Magnitude();
		if (distToRat < _attackRadius) {
			rat->_target = this;
		}
		else {
			if (rat->_target == this) {
				rat->_target = nullptr;
			}
		}
	}
}

void Player::Render()
{
	if (DISPLAY_PLAYER_TRIGGER_RADIUS)
	{
		Graphics::DrawCircle(
			static_cast<int>(body->position.x),
			static_cast<int>(body->position.y),
			static_cast<int>(_attackRadius),
			0.0f,
			0xFFFF0000
		);
	}
	Entity::Render();
}

void Player::GoDirection(const Vec2& direction)
{
	body->velocity = direction * body->maxVelocity;
}
