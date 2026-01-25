#pragma once

#include "Entity.h"

class Body;

class Player : public Entity
{
public:
	Player() = default;
	Player(std::unique_ptr<Body> body, const char* textureFileName);
	~Player();

	void Update(float dt) override;	

};
