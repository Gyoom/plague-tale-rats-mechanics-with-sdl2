#pragma once

#include "Cell.h"
#include "src/Vec2.h"
#include <vector>
#include <memory>

class Grid
{
private:
	std::vector<std::vector<std::unique_ptr<Cell>>> cells;
	float cellSize;
	int cols;
	int rows;

public:
	Grid(int cellSize);
	~Grid() = default;
	// Getters
	std::vector<std::vector<std::unique_ptr<Cell>>>& GetCells();
	int GetCellSize() const;
	int GetColsCount() const;
	int GetRowsCount() const;
	Vec2 GetCellWorldPosition(Vec2 coord) const;
	Vec2 GetCellLogicalPosition(Vec2 coord) const;

};
