#include "swarm.h"
#include "src/World.h"
#include "Common.h"
#include "rat.h"
#include "Cell.h"
#include "src/Graphics.h"
#include "src/Vec2.h"
#include "src/Body.h"
#include "Grid.h"
#include "src/Constants.h"
#include "src/Player.h""

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
	centerOfMass = Vec2(0.0f, 0.0f);
	for (auto rat : rats)
	{
		centerOfMass += rat->body->position;
	}
	centerOfMass /= (float) rats.size();

	for (auto& rat : rats) {
		if (rat->_displayDebugTools)
		{
			std::cout << "rat Velocity: " << rat->body->velocity.Magnitude() << "\n";
		}

		// Attraction to neighbor cells
		Vec2 cellLogicalPos = World::instance->grid->GetCellLogicalPosition(rat->body->position);
		vector<Cell*> neighbors = rat->GetDetectedCells();
		
		Vec2 dirBestCell = GetDirToBestNeighborCell(rat, neighbors);
		Vec2 targetCellCenter = rat->body->position + dirBestCell * static_cast<float>(World::instance->grid->GetCellSize());
		
		Vec2 desiredVelocity = (targetCellCenter - rat->body->position).Normalize() * rat->body->maxVelocity;
		Vec2 steering = desiredVelocity - rat->body->velocity;
		steering = Vec2::ClampMag(steering, rat->_maxForce);
		rat->body->AddForce(steering * rat->_w_neighbor);

		// Debug tools for cell attractiveness
		if (rat->_displayDebugTools && DISPLAY_CELLS_ATTRACTIVENESS) {
			ratDebug = rat;
			dirDebug = dirBestCell;
			neigDebug.clear();
			for (Cell* c : neighbors) {
				neigDebug.push_back(c);
			}
		}

		// Pursue target (player)
		if (rat->_target)
		{
			Vec2 toPlayer = rat->_target->body->position - rat->body->position;
			float dist = toPlayer.Magnitude();

			Vec2 desiredVel =
				toPlayer.Normalize() *
				rat->AttackSpeed(dist, rat->_target->_attackRadius);

			Vec2 pursueForce = desiredVel - rat->body->velocity;
			pursueForce = Vec2::ClampMag(pursueForce, rat->_maxForce);
			pursueForce *= _w_pursue;
			rat->body->AddForce(pursueForce);

			// if target is set, reduce the weight of cohesion
			float closeness = Common::clamp(dist / rat->_target->_attackRadius, 0.0f, 1.0f);
			_w_cohesion_target = Common::lerp(_w_cohesion_target_min, 1.0f, closeness);
			rat->body->_effectiveMinVelocity = Common::lerp(rat->body->_effectiveMaxVelocity, 100.0f, closeness);
			if (rat->_displayDebugTools)
			{
				std::cout << "Pursue force applied: (" << pursueForce.x << ", " << pursueForce.y << ")\n";
				std::cout << "Cohesion weight adjusted to: " << _w_cohesion_target << "\n";
				
			}
		}
		else
		{
			_w_cohesion_target = 1.0f;
			rat->body->_effectiveMinVelocity = 100.0f;
		}
			

		
		// Swarm cohesion
		Vec2 toCOM = centerOfMass - rat->body->position;
		float dist = toCOM.Magnitude();
		if (dist > 0.0001f)
		{
			// Cohesion with other rats
			Vec2 dir = toCOM / dist;

			// Cohesion radius based on swarm size
			cohesionRadius = static_cast<float>(World::instance->grid->GetCellSize()) * std::sqrt(rats.size()) * 0.1f;
			float t = dist / cohesionRadius;

			// Cohésion exponentielle after a certain distance
			float cohesionGain = _w_cohesion;
			float cohesionSharpness = 4.0f;

			float strength = std::exp((t - 1.0f) * cohesionSharpness);

			Vec2 cohesionForce = dir * strength * cohesionGain;

			cohesionForce = Vec2::ClampMag(cohesionForce, rat->_maxForce);
			cohesionForce *= _w_cohesion_target;
			rat->body->AddForce(cohesionForce);

			if (rat->_displayDebugTools)
			{
				std::cout << "Cohesion force applied: (" << cohesionForce.x << ", " << cohesionForce.y << ")\n";
				
			}

			// Adjust min velocity based on distance to center of mass
			
			/*sppedAdjustementRadius = static_cast<float>(World::instance->grid->GetCellSize()) * std::sqrt(rats.size()) * 0.5f;
			t = dist / sppedAdjustementRadius;

			float a = (t - 1.0f);
			a = Common::clamp(a, 0.0f, 1.0f);
			rat->body->_effectiveMinVelocity = Common::lerp(100.0f, 200.0f, a);*/
		}
		

			


		// speed limit by obstacles sighting
		float obstacleFactor = rat->GetForwardObstacleFactor();
		obstacleFactor = std::powf(obstacleFactor, _obstacleSlowingFactor);
		rat->body->_effectiveMaxVelocity = Common::lerp(rat->body->minVelocity, rat->body->maxVelocity, obstacleFactor);

		if (rat->_displayDebugTools)
		{
			std::cout << "\n";
			
		}
		
	}

}

void Swarm::Render()
{
	if (DISPLAY_CELLS_ATTRACTIVENESS)
	{
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

	if (DISPLAY_SWARM_COHESION_DEBUG) {
		// center of mass
		Graphics::DrawFillCircle(
			static_cast<int>(centerOfMass.x),
			static_cast<int>(centerOfMass.y),
			1,
			0xFF0000FF
		);
		// cohesion radius
		Graphics::DrawCircle(
			static_cast<int>(centerOfMass.x),
			static_cast<int>(centerOfMass.y),
			static_cast<int>(cohesionRadius),
			0,
			0xFF00FFFF
		);

		// speed adjustment radius
		Graphics::DrawCircle(
			static_cast<int>(centerOfMass.x),
			static_cast<int>(centerOfMass.y),
			static_cast<int>(sppedAdjustementRadius),
			0,
			0xFFFFFF00
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
