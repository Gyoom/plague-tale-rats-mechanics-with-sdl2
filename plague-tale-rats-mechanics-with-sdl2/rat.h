#pragma once

#include "src/Entity.h"

class Cell;
class GridBody;

class Rat : public Entity
{
public:
	bool _displayDebugTools = false;
	float _maxForce = 800.0f; // limit de reaction
	int _forwardDetectionRays = 0;

	float _w_neighbor = 1.0f;

	float _minVelocity = 100.0f;

	Rat() = default;
	Rat(std::unique_ptr<GridBody> body, const char* textureFileName, bool displayDebugTools);
	~Rat() = default;

	std::vector<Cell*> GetDetectedCells() const;
	float GetForwardObstacleFactor() const;

	void Update(float dt) override;
	void Render() override;
};

