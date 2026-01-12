#include "Grid.h"
#include "src/Graphics.h"


Grid::Grid(int cellSize)
{
	this->cellSize = cellSize;
	cols = 1920 / cellSize;
	rows = 1200 / cellSize;	
	for (int x = 0; x < cols; x++) {
		cells.push_back(std::vector<Cell*>());
		for (int y = 0; y < rows; y++) {
			cells[x].push_back(new Cell(Vec2(x * cellSize + cellSize/2, y * cellSize + cellSize / 2), Vec2(x, y)));
		}
	}
}

Grid::~Grid()
{
	for (auto col : cells) {
		for(auto cell : col) {
			delete cell;
		}
	}
}

int Grid::GetCellSize() const
{
	return cellSize;
}

int Grid::GetColsCount() const
{
	return cols;
}

int Grid::GetRowsCount() const
{
	return rows;
}

Vec2 Grid::GetCellWorldPosition(Vec2 logicalCoord) const
{
	if (logicalCoord.x < 0 || logicalCoord.x >= cols || logicalCoord.y < 0 || logicalCoord.y >= rows) {
		return Vec2(-1, -1); // Return an invalid position if out of bounds
	}
	Cell* c = cells[logicalCoord.x][logicalCoord.y];

	if (c == nullptr) {
		return Vec2(-1, -1); // Return an invalid position if cell is null
	}
	return c->GetWorldPosition();
}

Vec2 Grid::GetCellLogicalPosition(Vec2 worldCoord) const
{
	if (worldCoord.x < 0 || worldCoord.x >= Graphics::Width() || worldCoord.y < 0 || worldCoord.y >= Graphics::Height()) {
		return Vec2(-1, -1); // Return an invalid position if out of bounds
	}
	Vec2 cellCoord = Vec2(static_cast<int>(worldCoord.x) / cellSize, static_cast<int>(worldCoord.y) / cellSize);

	if (cellCoord.x < 0 || cellCoord.x >= cols || cellCoord.y < 0 || cellCoord.y >= rows) {
		return Vec2(-1, -1); // Return an invalid position if out of bounds
	}
	Cell* c = cells[worldCoord.x][worldCoord.y];
		
	return cellCoord;
}
