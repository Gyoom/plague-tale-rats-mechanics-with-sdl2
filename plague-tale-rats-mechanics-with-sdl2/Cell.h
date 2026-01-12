#pragma once

#include "src/Vec2.h"

class Cell
{
private:
	Vec2 logicalPos;
	Vec2 worldPos;

	public:
		Cell(Vec2 worldPos, Vec2 logicalPos);

		Vec2 GetWorldPosition() const { return worldPos; }
		Vec2 GetLogicalPosition() const { return logicalPos; }
};

