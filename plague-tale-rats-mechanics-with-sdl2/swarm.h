#pragma once

#include <vector>

#include "src/Vec2.h"

class Rat;
class Cell;

class Swarm
{
public:

	std::vector<Rat*> rats;

	float _attractiveness_factor = 1.0f;
	float _obstacleSlowingFactor = 2.0f;
	float _w_obstacleAvoidance = 3.0f;
	float _w_occupancy = 1.0f;
	float _w_cohesion = 0.5f;
	float _w_cohesion_target = 1.0f;
	float _w_cohesion_target_min = 0.2f;
	float _w_pursue = 4.0f;
	
	float _radiusObstacleAvoidance;


	// Debug cells attractiveness
	Rat* ratDebug = nullptr;
	Vec2 dirDebug = Vec2(0, 0);
	std::vector<Cell*> neigDebug;
	// Debug tools swarm cohesion
	Vec2 centerOfMass = Vec2(0, 0);
	float sppedAdjustementRadius = 0.0f;
	float cohesionRadius = 0.0f;

	Swarm();
	~Swarm() = default;

	Vec2 GetDirToBestNeighborCell(const Rat* r, const std::vector<Cell*>& neighbors);

	void Update(float dt);
	void Render();

	float GetCellAttractiveness(const Cell* c) const;
};

