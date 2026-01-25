#pragma once

#include <vector>
#include "Entity.h"

class Body;

class Boid : public Entity  
{  
public:
	float detectionRadius = 250.0f;
	float separationDistance = 100.0f; // Distance to maintain from other rats
	float maxForce = 800.0f; // limit de reaction
	float detectionAngleRad = 130.0f * (3.14159265f / 180.0f); // convert degrees to radians

	float intervalAngleRad = 10.0f * (3.14159265f / 180.0f); // convert degrees to radians

	float alignmentWeight = 2.0f;
	float separationWeight = 2.5f;
	float cohesionWeight = 1.0f;
	float boundaryAvoidanceWeight = 3.0f;

	bool _displayDebugTools = false;

	std::vector<Boid*> neighbors; // TODO : use weak_ptr to avoid circular references?

	Boid() = default;  
	Boid(std::unique_ptr<Body> body, const char* textureFileName, bool DisplayDebugTools = false);  
	~Boid();

	void Update(float dt) override;

	void Render() override;
};

