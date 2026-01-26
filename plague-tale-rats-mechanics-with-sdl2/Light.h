#pragma once
#include "src/Vec2.h"
#include <vector>

class Cell;
class Light
{
public:
	Vec2 position;
	float awayRadius;
	float midRadius;
	float closeRadius;
	std::vector <Cell*> affectedCellsPositions;

	float _w_lightRepulsion = 1.0f;
	float _maxLightDrag = 100.0f;
	float lightKickImpulse = 700.0f;

	Light(Vec2 pos, float away, float mid, float close);
	~Light() = default;

	void Update(float dt);
	void Render();
};

