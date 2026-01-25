#pragma once

#include "src/Vec2.h"
#include <limits>
#include <iostream>

class Cell
{
public:
	Vec2 _logicalPos;
	Vec2 _worldPos;

	float _occupancy = 0.0;

	bool _isObstacle = false;
	float _distToObstacle = FLT_MAX;

	float _lightLevel = 0.0f;

	Cell(Vec2 worldPos, Vec2 logicalPos);

	Vec2 GetWorldPosition() const { return _worldPos; }
	Vec2 GetLogicalPosition() const { return _logicalPos; }

	void ClearOccupancy();
	float GetOccupancy() const { return _occupancy; }
	void SetOccupancy(float occupancy) { _occupancy = occupancy; }

	bool isObstacle() const { return _isObstacle; }
	void SetIsObstacle(bool isObstacle) { 
		_isObstacle = isObstacle; 
		_distToObstacle = isObstacle ? 0.0f : FLT_MAX;
	}

	float GetDistToObstacle() const { return _distToObstacle; }
	void SetDistToObstacle(float dist) { _distToObstacle = dist; }
};

