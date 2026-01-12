#pragma once

#include "Cell.h"
#include "src/Vec2.h"
#include <vector>

class Grid
{
private:
	std::vector<std::vector<Cell*>> cells;
	float cellSize;
	int cols;
	int rows;

public:
	Grid(int cellSize);
	~Grid();
	// Getters
	std::vector<std::vector<Cell*>> GetCells() const { return cells; }
	int GetCellSize() const;
	int GetColsCount() const;
	int GetRowsCount() const;
	Vec2 GetCellWorldPosition(Vec2 coord) const;
	Vec2 GetCellLogicalPosition(Vec2 coord) const;

};

