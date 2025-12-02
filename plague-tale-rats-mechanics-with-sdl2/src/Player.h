#pragma once

#include "Entity.h"

struct Player : public Entity
{

private:
	Vec2 newPos;
	bool isMoving = false;
public:
	Player() = default;
	Player(Body* body, const char* textureFileName);
	~Player();

	void SetNewMove(const Vec2& pos);

	void Update(float dt) override;	

};
