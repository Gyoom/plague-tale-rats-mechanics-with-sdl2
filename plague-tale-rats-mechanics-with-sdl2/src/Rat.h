#pragma once


#include "Entity.h"

struct Rat : public Entity  
{  
	float detectionRadius = 80.0f;
	float separationDistance = 30.0f; // Distance to maintain from other rats
	float maxForce = 800.0f; // limit de reaction

	float alignmentWeight = 1.0f;
	float separationWeight = 2.0f;
	float cohesionWeight = 0.6f;

	float multiplier = 10.0f;

	std::vector<Rat*> neighbors;

	Rat() = default;  
	Rat(Body* body, const char* textureFileName);  
	~Rat();

	void Update(float dt) override;
};

