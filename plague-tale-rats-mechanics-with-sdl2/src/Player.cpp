#include "Player.h"

Player::Player(Body* body, const char* textureFileName): Entity(body, textureFileName)
{
	type = PLAYER;
}

Player::~Player()
{
	
}

void Player::SetNewMove(const Vec2& pos)
{
	newPos = pos;
	isMoving = true;
}

void Player::Update(float dt)
{
	if (isMoving)
	{
		Vec2 direction = newPos - body->position;
		float distance = direction.Magnitude();
		if (distance < 5.0f)
		{
			body->velocity = Vec2(0.0f, 0.0f);
			isMoving = false;
		}
		else
		{
			direction.Normalize();
			float speed = 300.0f; // pixels per second
			body->velocity = direction * speed;
		}
	}
	else
	{
		body->velocity = Vec2(0.0f, 0.0f);
	}
	Entity::Update(dt);
}
