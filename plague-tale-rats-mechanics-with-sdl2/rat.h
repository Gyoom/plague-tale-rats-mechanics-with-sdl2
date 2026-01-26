#pragma once

#include "src/Entity.h"

class Cell;
class GridBody;
class Player;

class Rat : public Entity
{
public:
	bool _displayDebugTools = false;

	float _maxForce = 800.0f; // limit de reaction
	float _maxForceAttackMultiplier = 2.0f;
	
	int _forwardDetectionRays = 0;

	float _w_neighbor = 1.0f;

	float _restVelocity = 100.0f;
	float _attackVelocity = 350.0f;

	float _w_cohesion_target = 1.0f;
	float _w_cohesion_target_min = 0.2f;

	Player* _target = nullptr;

	Rat() = default;
	Rat(std::unique_ptr<GridBody> body, const char* textureFileName, bool displayDebugTools);
	~Rat() = default;

	std::vector<Cell*> GetDetectedCells() const;
	float GetForwardObstacleFactor() const;

	void Update(float dt) override;
	void Render() override;
};

