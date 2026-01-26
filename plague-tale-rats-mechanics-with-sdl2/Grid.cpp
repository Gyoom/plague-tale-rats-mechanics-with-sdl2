#include "Grid.h"
#include "src/Graphics.h"
#include "src/World.h"
#include "src/Vec2.h"
#include "Cell.h"
#include "Rat.h"
#include "src/Body.h"
#include "common.h"

#include <iostream>
#include "src/Constants.h"

using namespace std;

Grid::Grid(int cellSize)
{
	this->cellSize = cellSize;
	cols = Graphics::Width() / cellSize;
	rows = Graphics::Height() / cellSize;
	for (int x = 0; x < cols; x++) {
		cells.push_back(vector<unique_ptr<Cell>>());
		for (int y = 0; y < rows; y++) {
			cells[x].push_back(make_unique<Cell>(Vec2(x * cellSize + cellSize/2, y * cellSize + cellSize / 2), Vec2(x, y)));

			if (x == 0 || y == 0 || y == rows - 1 || x == cols - 1)
				cells[x][y]->SetIsObstacle(true);
		}
	}

	// Calculate distance to nearest obstacle for each cell
	vector<Cell*> obstacleCells = GetObstacleCells();
	while (obstacleCells.size() > 0)
	{
		Cell* current = obstacleCells.back();
		obstacleCells.pop_back();
		Vec2 logicalPos = current->GetLogicalPosition();
		vector<Cell*> neighbors = GetNeighbors(logicalPos);
		for (Cell* neighbor : neighbors)
		{
			if (neighbor->isObstacle()) continue;
			float newDist = current->GetDistToObstacle() + static_cast<float>(cellSize);
			if (newDist < neighbor->GetDistToObstacle())
			{
				neighbor->SetDistToObstacle(newDist);
				obstacleCells.push_back(neighbor);
			}
		}
	}
}

vector<vector<unique_ptr<Cell>>>& Grid::GetCells() 
{
	return cells;
}

int Grid::GetCellSize() const
{
	return cellSize;
}

float Grid::GetGridWidth() const
{
	return cols * cellSize;
}

float Grid::GetGridHeight() const
{
	return rows * cellSize;
}

int Grid::GetColsCount() const
{
	return cols;
}

int Grid::GetRowsCount() const
{
	return rows;
}

Vec2 Grid::GetCellWorldPosition(const Vec2& logicalCoord) const
{
	if (logicalCoord.x < 0 || logicalCoord.x >= cols || logicalCoord.y < 0 || logicalCoord.y >= rows) {
		return Vec2(-1, -1); // Return an invalid position if out of bounds
	}
	Cell* c = cells[logicalCoord.x][logicalCoord.y].get();

	if (c == nullptr) {
		return Vec2(-1, -1); // Return an invalid position if cell is null
	}
	return c->GetWorldPosition();
}

Vec2 Grid::GetCellLogicalPosition(const Vec2& worldCoord) const
{
	if (worldCoord.x < 0 || worldCoord.x >= Graphics::Width() || worldCoord.y < 0 || worldCoord.y >= Graphics::Height()) {
		return Vec2(-1, -1); // Return an invalid position if out of bounds
	}
	Vec2 cellCoord = Vec2(static_cast<int>(worldCoord.x / cellSize), static_cast<int>(worldCoord.y / cellSize));

	if (cellCoord.x < 0 || cellCoord.x >= cols || cellCoord.y < 0 || cellCoord.y >= rows) {
		return Vec2(-1, -1); // Return an invalid position if out of bounds
	}
		

	return cellCoord;
}

Cell* Grid::GetCellAtLogicalPosition(const Vec2& logicalPos) const
{
	if (logicalPos.x < 0 || logicalPos.x >= cols || logicalPos.y < 0 || logicalPos.y >= rows) {
		return nullptr; // Return nullptr if out of bounds
	}
	return cells[static_cast<int>(logicalPos.x)][static_cast<int>(logicalPos.y)].get();
	
}

Cell* Grid::GetCellAtWorldPosition(const Vec2& worldPos) const
{
	Vec2 logicalPos = GetCellLogicalPosition(worldPos);
	if (logicalPos.x == -1 && logicalPos.y == -1) {
		return nullptr; // Return nullptr if out of bounds
	}
	return GetCellAtLogicalPosition(logicalPos);

	
}

vector<Cell*> Grid::GetNeighbors(const Vec2& logicalPos) const
{
	if (logicalPos.x < 0 || logicalPos.x >= cols || logicalPos.y < 0 || logicalPos.y >= rows) {
		return {}; // Return empty vector if out of bounds
	}
	vector<Cell*> neighbors;
	int x = static_cast<int>(logicalPos.x);
	int y = static_cast<int>(logicalPos.y);
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0) continue; // Skip the cell itself
			int nx = x + dx;
			int ny = y + dy;
			if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
				neighbors.push_back(cells[nx][ny].get());
			}
		}
	}
	return neighbors;
	
}

std::vector<Cell*> Grid::GetObstacleCells() const
{
	std::vector<Cell*> obstacleCells;
	for (const auto& column : cells) {
		for (const auto& cell : column) {
			if (cell->isObstacle()) {
				obstacleCells.push_back(cell.get());
			}
		}
	}
	return obstacleCells;
	
}

void Grid::Update(float dt)
{
	for (auto& column : cells) {
		for (auto& cell : column) {
			cell->ClearOccupancy();
		}
	}

	for(auto& cr : World::instance->rats) {
		Vec2 logicalPos = GetCellLogicalPosition(cr->body->position);
		if (logicalPos.x == -1 && logicalPos.y == -1)
		{
			std::cout << "Rat out of bounds at position: (" << cr->body->position.x << ", " << cr->body->position.y << ")\n";
			continue; // Skip if out of bounds
		}
		Cell* cell = cells[static_cast<int>(logicalPos.x)][static_cast<int>(logicalPos.y)].get();
		if (cell) {
			cell->SetOccupancy(cell->GetOccupancy() + 1.0f);
		}
	}	
}

void Grid::Render()
{
	// Draw obstacle cells
	for (auto& column : cells) {
		for (auto& cell : column) {
			Uint32 black = 0xFF000000; // black
			

			if (cell->isObstacle())
			{
				Vec2 worldPos = cell->GetWorldPosition();
				Graphics::DrawFillRect(
					static_cast<int>(worldPos.x),
					static_cast<int>(worldPos.y),
					cellSize,
					cellSize,
				black
				);
			}
		}
	}

	// Draw distance to obstacle preview
	if (DISPLAY_OBSTACLE_CELLS_GRADIENT)
	{
		for (auto& column : cells) {
			for (auto& cell : column) {
				if (cell->isObstacle()) continue;

				Uint32 black = 0xFF111111; // black
				Uint32 bg = 0xFF222222; // background color
				float distFactor = cell->GetDistToObstacle() / (GetCellSize() * 3.0f);

				if (distFactor < 0.0f) distFactor = 0.0f;
				if (distFactor > 1.0f) distFactor = 1.0f;
				Uint32 color = Common::lerpColor(black, bg, distFactor);

				Vec2 worldPos = cell->GetWorldPosition();
				Graphics::DrawFillRect(
					static_cast<int>(worldPos.x),
					static_cast<int>(worldPos.y),
					cellSize,
					cellSize,
					color
				);
			}
		}
	}

	// Draw grid lines
	for (int x = 0; x <= cols; x++) {
		Graphics::DrawLine(x * cellSize, 0, x * cellSize, rows * cellSize, 0xFF444444);
	}
	for (int y = 0; y <= rows; y++) {
		Graphics::DrawLine(0, y * cellSize, cols * cellSize, y * cellSize, 0xFF444444);
	}
	
}
