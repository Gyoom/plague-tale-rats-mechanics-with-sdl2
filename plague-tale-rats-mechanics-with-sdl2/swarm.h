#pragma once

#include <vector>

#include "src/Vec2.h"

class Rat;
class Cell;

class Swarm
{
public:
	Vec2 position;
	bool fixed = false;
	float cohesionRadius;
	Vec2 centerOfMass;
	std::vector<Rat*> rats;
	
	// Cohesion parameters
	float _w_cohesion_radius_factor = 2.5f;
	float _w_cohesion = 0.5f;

	Swarm(int ratCount, Vec2 pos, bool fixed);
	~Swarm() = default;

	void Update(float dt);
	void Render();
};

