#pragma once


#include "Entity.h"

struct Rat : public Entity  
{  
	float detectionRadius = 250.0f;
	float separationDistance = 100.0f; // Distance to maintain from other rats
	float maxForce = 800.0f; // limit de reaction
	float detectionAngleRad = 130.0f * (3.14159265f / 180.0f); // convert degrees to radians

	float alignmentWeight = 2.0f;
	float separationWeight = 2.5f;
	float cohesionWeight = 1.0f;

	float multiplier = 1.0f;	

	std::vector<Rat*> neighbors;

	Rat() = default;  
	Rat(Body* body, const char* textureFileName);  
	~Rat();

	void Update(float dt) override;
};

