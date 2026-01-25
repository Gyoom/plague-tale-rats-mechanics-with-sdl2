#include "swarm.h"
#include "src/World.h"
#include "Common.h"
#include "rat.h"
#include "Cell.h"
#include "src/Graphics.h"
#include "src/Vec2.h"
#include "src/Body.h"
#include "Grid.h"

using namespace std;

Swarm::Swarm()
{
	_radiusObstacleAvoidance = static_cast<float>(World::instance->grid->GetCellSize()) * 3.0f;
}

Vec2 Swarm::GetDirToBestNeighborCell(const Rat* r, const vector<Cell*>& neighbors)
{
	Grid* grid = World::instance->grid.get();
	Vec2 dir = Vec2(0, 0);
	for (Cell* c : neighbors)
	{
		if (c->isObstacle()) continue;

		float attractiveness = GetCellAttractiveness(c);

		Vec2 dirToCell = (c->GetWorldPosition() - r->body->position).Normalize();
		dir += dirToCell * attractiveness;
	}

	if (dir.MagnitudeSquared() > 0.0001f)
		dir = dir.Normalize();

	return dir;
}

void Swarm::Update(float dt)
{
	Vec2 centerOfMass = Vec2(0.0f, 0.0f);
	for (auto rat : rats)
	{
		centerOfMass += rat->body->position;
	}
	centerOfMass /= (float) rats.size();

	for (auto& rat : rats) {
		// Attraction to neighbor cells
		Vec2 cellLogicalPos = World::instance->grid->GetCellLogicalPosition(rat->body->position);
		vector<Cell*> neighbors = rat->GetDetectedCells();
		
		Vec2 dirBestCell = GetDirToBestNeighborCell(rat, neighbors);
		Vec2 targetCellCenter = rat->body->position + dirBestCell * static_cast<float>(World::instance->grid->GetCellSize());
		
		Vec2 desiredVelocity = (targetCellCenter - rat->body->position).Normalize() * rat->body->maxVelocity;
		Vec2 steering = desiredVelocity - rat->body->velocity;
		steering = Vec2::ClampMag(steering, rat->_maxForce);
		rat->body->AddForce(steering * rat->_w_neighbor);

		// Adjust min velocity based on local density
		float localDensity = 0;
		for( Cell* c : neighbors) {
			localDensity += c->_occupancy;
		}
		float maxDensity = static_cast<float>(neighbors.size() / 2);
		//rat->body->_effectiveMinVelocity = Common::lerp(200.0f, 100.0f, localDensity / maxDensity);
		//rat->body->_effectiveMinVelocity = 100.0f;


		// Debug tools
		if (rat->_displayDebugTools) {
			_displayDebugTools = true;
			ratDebug = rat;
			dirDebug = dirBestCell;
			neigDebug.clear();
			for (Cell* c : neighbors) {
				neigDebug.push_back(c);
			}
		}

		// Cohesion with other rats
		Vec2 toCOM = centerOfMass - rat->body->position;
		float dist = toCOM.Magnitude();
		if (dist > 0)
		{
			Vec2 dir= toCOM / dist;

			// Rayon “acceptable” du groupe
			float baseRadius = static_cast<float>(World::instance->grid->GetCellSize()) * 0.1f;
			float groupRadius = baseRadius * std::sqrt(rats.size());

			float t = dist / groupRadius;

			// Cohésion exponentielle
			float cohesionGain = _w_cohesion;
			float cohesionSharpness = 4.0f;

			float strength = std::exp((t - 1.0f) * cohesionSharpness);

			Vec2 cohesionForce = dir * strength * cohesionGain;

			cohesionForce = Vec2::ClampMag(cohesionForce, rat->_maxForce);
			rat->body->AddForce(cohesionForce);

			// Adjust min velocity based on distance to center of mass
			baseRadius = static_cast<float>(World::instance->grid->GetCellSize()) * 0.5f;
			groupRadius = baseRadius * std::sqrt(rats.size());
			t = dist / groupRadius;

			float a = (t - 1.0f);
			if (a > 1.0f) a = 1.0f;
			if (a < 0.0f) a = 0.0f;
			// first rat
			if (rat == rats.front()) {
				Graphics::DrawFillRect(
					static_cast<int>(rat->body->position.x),
					static_cast<int>(rat->body->position.y),
					10,
					10,
					0xFFFFFF00
				);
				Graphics::DrawCircle(
					static_cast<int>(centerOfMass.x),
					static_cast<int>(centerOfMass.y),
					static_cast<int>(groupRadius),
					0,
					0xFFFFFF00
				);
			}

			rat->body->_effectiveMinVelocity = Common::lerp(100.0f, 200.0f, a);

		}

		// speed limit by obstacles sighting
		float obstacleFactor = rat->GetForwardObstacleFactor();
		obstacleFactor = std::powf(obstacleFactor, _obstacleSlowingFactor); // cubic easing
		rat->body->_effectiveMaxVelocity = Common::lerp(rat->body->minVelocity, rat->body->maxVelocity, obstacleFactor);

		
	}

}

void Swarm::Render()
{
	if (!_displayDebugTools || ratDebug == nullptr)
		return;
	Graphics::DrawLine(
		static_cast<int>(ratDebug->body->position.x),
		static_cast<int>(ratDebug->body->position.y),
		static_cast<int>(ratDebug->body->position.x + dirDebug.x * 50),
		static_cast<int>(ratDebug->body->position.y + dirDebug.y * 50),
		0xFFFF00FF
	);

	for (auto& cell : neigDebug) {
		
		Uint32 colorDown = 0xFF0000FF; // red
		Uint32 colorUp = 0xFF00FF00; // green

		Uint32 color = Common::lerpColor(colorDown, colorUp, std::powf(GetCellAttractiveness(cell), 1.0f));
		
		Graphics::DrawFillRect(
			static_cast<int>(cell->GetWorldPosition().x),
			static_cast<int>(cell->GetWorldPosition().y),
			World::instance->grid->GetCellSize(),
			World::instance->grid->GetCellSize(),
			color
		);
	}
}

float Swarm::GetCellAttractiveness(const Cell* c) const
{
	float cost = c->_occupancy * _w_occupancy +
	std::exp(-c->GetDistToObstacle() / _radiusObstacleAvoidance) * _w_obstacleAvoidance;
	float attractiveness = std::exp(-cost * _attractiveness_factor);
	return attractiveness;
	
}
