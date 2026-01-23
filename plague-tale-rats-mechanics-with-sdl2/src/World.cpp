#include "World.h"
#include "Constants.h"
#include "Graphics.h"
#include <iostream>

World* World::instance = nullptr;

World::World(float gravity) {
    G = -gravity;
    std::cout << "Creating World" << std::endl;

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
