#include "World.h"
#include "Constants.h"
#include "Graphics.h"
#include <iostream>
#include <vector>

#include "Body.h"
#include "Player.h"
#include "../Grid.h"
#include "../Swarm.h"
#include "../Rat.h"
#include "Boid.h"    
#include "../Cell.h"
#include "../Light.h"
#include "../Common.h"

using namespace std;

World* World::instance = nullptr;

World::World(float gravity) {
    G = -gravity;
    std::cout << "Creating World" << std::endl;

	// Singleton pattern
    if (World::instance == nullptr) {
        World::instance = this;
    } else {
        std::cerr << "Warning: Multiple instances of World detected!" << std::endl;
	}
	
}

World::~World() {
	
    std::cout << "\nDeleting World" << std::endl;
}

void World::AddBody(Body* body) {
    entities.push_back(body);
}

std::vector<Body*>& World::GetBodies() {
    return entities;
}

void World::AddForce(const Vec2& force) {
    forces.push_back(force);
}

void World::AddTorque(float torque) {
    torques.push_back(torque);
}

void World::RatsUpdate(float dt)
{
	for (auto& rat : rats) {
		if (rat->_displayDebugTools)
		{
			std::cout << "rat Velocity: " << rat->body->velocity.Magnitude() << "\n";
		}

		// Attraction to neighbor cells
		Vec2 cellLogicalPos = World::instance->grid->GetCellLogicalPosition(rat->body->position);
		vector<Cell*> neighbors = rat->GetDetectedCells();

		Vec2 dirBestCell = GetDirToBestNeighborCell(rat.get(), neighbors);
		Vec2 targetCellCenter = rat->body->position + dirBestCell * static_cast<float>(World::instance->grid->GetCellSize());

		Vec2 desiredVelocity = (targetCellCenter - rat->body->position).Normalize() * rat->body->maxVelocity;
		Vec2 steering = desiredVelocity - rat->body->velocity;
		steering = Vec2::ClampMag(steering, rat->_maxForce);
		steering *= _w_attractiveness;
		rat->body->AddForce(steering);

		if (rat->_displayDebugTools)
		{
			std::cout << "Neigbors force applied: (" << steering.Magnitude() << ")\n";
		}

		// Debug tools for cell attractiveness
		if (rat->_displayDebugTools && DISPLAY_CELLS_ATTRACTIVENESS) {
			ratDebug = rat.get();
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
				rat->body->maxVelocity;

			Vec2 pursueForce = desiredVel - rat->body->velocity;
			pursueForce = Vec2::ClampMag(pursueForce, rat->_maxForce * rat->_maxForceAttackMultiplier);
			pursueForce *= _w_pursue;
			rat->body->AddForce(pursueForce);

			// if target is set, reduce the weight of cohesion
			float closeness = Common::clamp(dist / rat->_target->_attackRadius, 0.0f, 1.0f);

			// more closeness => less cohesion, more speed
			rat->_w_cohesion_target = Common::lerp(rat->_w_cohesion_target_min, 1.0f, closeness);
			rat->body->_desiredVelocity = Common::lerp(rat->_attackVelocity, rat->_restVelocity, closeness);

			if (rat->_displayDebugTools)
			{
				std::cout << "Pursue force applied: (" << pursueForce.Magnitude() << ")\n";
			}
		}
		else
		{
			rat->_w_cohesion_target = 1.0f; // reset to default cohesion weight
			rat->body->_desiredVelocity = rat->_restVelocity; // reset to rest velocity
		}

		// Separation from nearby rats
		Vec2 separation = Vec2(0, 0);
		for (auto& other : rats)
		{
			Vec2 toOther = rat->body->position - other->body->position;
			float OtherLength = toOther.MagnitudeSquared();

			float separationRadius = static_cast<float>(World::instance->grid->GetCellSize()) * 0.5f;

			if (OtherLength > 0.0001f && OtherLength < separationRadius * separationRadius)
			{
				separation += toOther / OtherLength;
			}
		}
		separation = separation.ClampMag(separation, rat->_maxForce);
		separation *= _w_separation;
		rat->body->AddForce(separation);

		if (rat->_displayDebugTools)
		{
			std::cout << "Separation force applied: (" << separation.Magnitude() << ")\n";
		}

		// speed limit by obstacles sighting
		float obstacleFactor = rat->GetForwardObstacleFactor();
		obstacleFactor = std::powf(obstacleFactor, _obstacleSlowingFactor);
		rat->body->_desiredVelocity *= obstacleFactor;
	}
}

/**
* Update all bodies in the world by applying global forces and torques
*/
void World::bodiesUpdate(float dt) {
    //this->AddTorque(20.f);

   // Loop all bodies of the world applying forces
    for (auto e : entities) {
        // Apply the weight force to all bodies
        Vec2 weight = Vec2(0.0, e->mass * G * PIXELS_PER_METER);
        e->AddForce(weight);

        // Apply global forces (wind etc.) to all bodies
        for (auto force : forces) {
            e->AddForce(force);
        }

        // Apply global torque (wind etc.) to all bodies
        for (auto torque : torques) {
            e->AddTorque(torque);
        }
    }
    
}

void World::Update(float dt) 
{
    for (auto& e : boids) {
        e->Update(dt);
    }

    if (grid)
		grid->Update(dt);

	if (light)
		light->Update(dt);

    if (player)
        player->Update(dt);

	RatsUpdate(dt);
    for (auto& e : swarms) {
        e->Update(dt);
    }

    for (auto& e : rats) {
        e->Update(dt);
    }

    
	//World::bodiesUpdate(dt);
}

void World::VerletCollisionsEffects(Body* a, Body* b)
{
	
}

void World::CheckCollisions() { // Narrow type only
    
}

void World::Render() {
	if (DISPLAY_CELLS_ATTRACTIVENESS && ratDebug)
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


    // boids
    for (auto& e : boids) {
        e->Render();
    }
    // rats
    if (grid) {
        grid->Render();
    }

    if (light)
		light->Render();

    if (player)
        player->Render();
        
    for (auto& r : rats) {
        r->Render();
    }

    for (auto& s : swarms) {
        s->Render();
    }
}

float World::GetCellAttractiveness(const Cell* c) const
{
	float distanceToObstacleMax = static_cast<float>(World::instance->grid->GetCellSize()) * 3.0f;

	float cost = c->_occupancy * _w_occupancy +
		std::exp(-c->GetDistToObstacle() / distanceToObstacleMax) * _w_obstacleAvoidance;
	float attractiveness = std::exp(-cost * _attractiveness_sharpness);
	return attractiveness;
}

Vec2 World::GetDirToBestNeighborCell(const Rat* r, const vector<Cell*>& neighbors)
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
