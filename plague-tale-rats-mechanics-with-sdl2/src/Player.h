#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player() = default;
	Player(Body* body, const char* textureFileName);
	~Player();

	void Update(float dt) override;	

};
