#include "rat.h"
#include "Cell.h"
#include "src/World.h"
#include <algorithm>
#include "src/Graphics.h"
#include "src/Vec2.h"
#include "src/Body.h"
#include "Grid.h"
#include "Swarm.h"
#include "src/Constants.h"
#include "Light.h"

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

void Rat::Update(float dt)
{
	// Handle light exposure effects
	if (_isInLight)
	{
		_lightTimer += dt;
		if (_lightTimer >= _lightResistanceDuration && body->_color != 0xFF0000FF)
		{
			if (DELETE_RAT_ON_LIGHT_EXPOSURE && !_displayDebugTools)
			{
				_pendingKill = true;
				
			}
				
			else
				body->_color = 0xFF0000FF;
		}
	}
	// Reset light state after processing
	if (_lightSource)
	{
		Vec2 toLight = (_lightSource->position - this->body->position).Normalize();
		float alignment = this->body->velocity.Dot(toLight);
		if (alignment > 0.0f)
			EndRepelFromLight();
	}


	Entity::Update(dt);
}

void Rat::Render()
{
	Entity::Render();
}

void Rat::EndRepelFromLight()
{
	body->fowardLocked = false;
	body->_dragEnabled = true;
	_lightSource = nullptr;
}
