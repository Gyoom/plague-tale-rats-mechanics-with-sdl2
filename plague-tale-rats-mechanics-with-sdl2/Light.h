#pragma once
#include "src/Vec2.h"
#include <vector>

class Cell;
class Light
{
public:
	Vec2 position;
	float radius;
	float percentageDeadRadius; // 0.0 to 1.0
	std::vector <Cell*> affectedCellsPositions;

	Light(Vec2 pos, float r, float deadRadius);
	~Light() = default;

	void Update(float dt);
	void Render();
};

