#pragma once

#include "Entity.h"

class Body;

class Player : public Entity
{
public:
	Vec2 _movementDirection = Vec2(0.0f, 0.0f);
	float _attackRadius = 300.0f;

	Player() = default;
	Player(std::unique_ptr<Body> body, const char* textureFileName);
	~Player();

	void Update(float dt) override;	
	void Render() override;

	void GoDirection(const Vec2& direction);

};
