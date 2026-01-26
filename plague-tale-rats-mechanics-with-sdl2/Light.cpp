#include "Light.h"
#include "src/Graphics.h"
#include "SDL.h"
#include "Cell.h"
#include "src/World.h"
#include "Grid.h"
#include "src/Constants.h"

using namespace std;

Light::Light(Vec2 pos, float r, float inten)
{
	position = pos;
	radius = r;
	percentageDeadRadius = inten;
}

void Light::Update(float dt)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	Vec2 mousePos = Vec2(x, y);
	if (Graphics::IsInsideScreen(mousePos))
		position = mousePos;

	for(Cell* c : affectedCellsPositions)
	{
		c->_lightLevel = 0.0f;
	}
	affectedCellsPositions.clear();

	Grid* grid = World::instance->grid.get();

	vector<vector<unique_ptr<Cell>>>& cells = grid->GetCells();
	float deadRadius = radius * percentageDeadRadius;
	for(auto& row : cells)
	{
		for(auto& cell : row)
		{
			if (cell->isObstacle())
				continue;
			float dist = position.Dist(cell->GetWorldPosition());
			if(dist <= radius)
			{
				affectedCellsPositions.push_back(cell.get());
				if (dist <= deadRadius)
				{
					cell->_lightLevel = 1.0f;
					continue;
				}
				float lightLevel = 1.0f - ((dist - deadRadius) / (radius - deadRadius));
			}
		}
	}
}

void Light::Render()
{
	if (DISPLAY_LIGHT_CELLS)
	{
		for(Cell* c : affectedCellsPositions)
		{
			Uint32 color = 0x44FF00FF;
			if (c->_lightLevel == 1.0f)
			{
				color = 0x44FFFF00;
			}

			Graphics::DrawFillRect(
				static_cast<int>(c->GetWorldPosition().x),
				static_cast<int>(c->GetWorldPosition().y),
				World::instance->grid->GetCellSize(),
				World::instance->grid->GetCellSize(),
				color
			);
		}
	}

	Graphics::DrawCircle(
		static_cast<int>(position.x), 
		static_cast<int>(position.y), 
		static_cast<int>(radius), 
		0,
		0xFFFFFFFF
	);
	float deadRadius = radius * percentageDeadRadius;
	Graphics::DrawCircle(
		static_cast<int>(position.x), 
		static_cast<int>(position.y), 
		static_cast<int>(deadRadius), 
		0,
		0xFF0000FF
	);
}
