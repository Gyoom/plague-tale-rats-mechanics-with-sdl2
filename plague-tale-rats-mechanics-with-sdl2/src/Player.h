#pragma once

#include "Entity.h"
#include "Rat.h"

struct Player : public Entity
{
	float detectionRadius = 80.0f;
	float separationDistance = 30.0f; // Distance to maintain from other rats
	float maxForce = 800.0f; // limit de reaction
	
	float alignmentWeight = 1.0f;
	float separationWeight = 2.0f; 
	float cohesionWeight = 0.6f;

	float multiplier = 40.0f;
	
	

	std::vector<Rat*> neighbors;

	Player() = default;
	Player(Body* body, const char* textureFileName);
	~Player();

	void Update(float dt) override;	

};
