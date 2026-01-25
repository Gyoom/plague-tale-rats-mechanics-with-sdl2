#pragma once

#include <vector>
#include <memory>
#include "src/Vec2.h"

class Cell;

class Grid
{
private:
	std::vector<std::vector<std::unique_ptr<Cell>>> cells;
	float cellSize;
	int cols;
	int rows;

	bool _distanceToObstaclePreview = false;

public:
	Grid(int cellSize);
	~Grid() = default;
	// Getters
	std::vector<std::vector<std::unique_ptr<Cell>>>& GetCells();
	
	int GetCellSize() const;
	float GetGridWidth() const;
	float GetGridHeight() const;
	
	int GetColsCount() const;
	int GetRowsCount() const;
	
	Vec2 GetCellWorldPosition(const Vec2& coord) const;
	Vec2 GetCellLogicalPosition(const Vec2& coord) const;

	Cell* GetCellAtLogicalPosition(const Vec2& logicalPos) const;
	Cell* GetCellAtWorldPosition(const Vec2& worldPos) const;

	std::vector<Cell*> GetNeighbors(const Vec2& logicalPos) const;
	std::vector<Cell*> GetObstacleCells() const;

	void Update(float dt);
	void Render();

};
