#include "rat.h"
#include "Cell.h"
#include "src/World.h"
#include <algorithm>
#include "src/Graphics.h"
#include "src/Vec2.h"
#include "src/Body.h"
#include "Grid.h"

Rat::Rat(std::unique_ptr<GridBody> body, const char* textureFileName, bool displayDebugTools) : Entity(std::move(body), textureFileName)
{
	_displayDebugTools = displayDebugTools;
	type = RAT;

}

std::vector<Cell*> Rat::GetDetectedCells() const
{
	std::vector<Cell*> cells = std::vector<Cell*>();
	
	Vec2 baseCellLogicalPos = World::instance->grid->GetCellLogicalPosition(this->body->position);

	std::vector<Cell*> detectedCells = World::instance->grid->GetNeighbors(baseCellLogicalPos);
	cells.insert(cells.end(), detectedCells.begin(), detectedCells.end());

	for(int i = 1; i < _forwardDetectionRays; i++)
	{
		Vec2 checkedWorldPos = this->body->position + this->body->forward * (i * World::instance->grid->GetCellSize());
		Vec2 cellLogicalPos = World::instance->grid->GetCellLogicalPosition(checkedWorldPos);
		
		detectedCells = World::instance->grid->GetNeighbors(cellLogicalPos);

		for (Cell* c : detectedCells)
		{
			if (c != nullptr && !c->GetLogicalPosition().Equals(baseCellLogicalPos) && std::find(cells.begin(), cells.end(), c) == cells.end())
				cells.push_back(c);
		}
	}
	return cells;
}

float Rat::GetForwardObstacleFactor() const
{
	float minForwardDist = INFINITY;

	for (int i = 1; i <= _forwardDetectionRays; ++i)
	{
		Vec2 samplePos = body->position + body->forward * (i * World::instance->grid->GetCellSize());
		Cell* c = World::instance->grid->GetCellAtWorldPosition(samplePos);
		
		if (c && c->isObstacle())
		{
			minForwardDist = i * World::instance->grid->GetCellSize();
			break;
		}
	}

	float maxLookaheadDist = _forwardDetectionRays * World::instance->grid->GetCellSize();
	float obstacleDist = 1.0f;

	if (minForwardDist < maxLookaheadDist)
	{
		float t = minForwardDist / maxLookaheadDist;
		obstacleDist = std::clamp(t, 0.0f, 1.0f);
	}
	return obstacleDist;
}

float Rat::AttackSpeed(float dist, float attackRange) const
{
	if (dist > attackRange) return body->_effectiveMaxVelocity;
	return body->_effectiveMaxVelocity * (dist / attackRange);
}

void Rat::Update(float dt)
{
	Entity::Update(dt);
}

void Rat::Render()
{
	Entity::Render();
}
