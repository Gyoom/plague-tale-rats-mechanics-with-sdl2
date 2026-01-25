#pragma once

#include <vector>

#include "src/Vec2.h"

class Rat;
class Cell;

class Swarm
{
public:

	std::vector<Rat*> rats;

	float _obstacleSlowingFactor = 2.0f;
	float _w_obstacleAvoidance = 3.0f;
	float _w_occupancy = 1.0f;
	float _w_cohesion = 0.5f;
	float _attractiveness_factor = 1.0f;
	float _radiusObstacleAvoidance;;

	bool _displayDebugTools = false;
	Rat* ratDebug = nullptr;
	Vec2 dirDebug = Vec2(0, 0);
	std::vector<Cell*> neigDebug;

	Swarm();
	~Swarm() = default;

	Vec2 GetDirToBestNeighborCell(const Rat* r, const std::vector<Cell*>& neighbors);

	void Update(float dt);
	void Render();

	float GetCellAttractiveness(const Cell* c) const;
};

