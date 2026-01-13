#pragma once


#include "Entity.h"

struct Rat : public Entity  
{  
	float forwardForceMagnitude = 5.0f;

	Rat() = default;  
	Rat(Body* body, const char* textureFileName);  
	~Rat();

	void Update(float dt) override;
};

