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

    for (auto e : entities) {
        delete e;
    }

    delete grid;
    
}

void World::AddEntity(Entity* body) {
    entities.push_back(body);
}

std::vector<Entity*>& World::GetEntities() {
    return entities;
}

void World::AddForce(const Vec2& force) {
    forces.push_back(force);
}

void World::AddTorque(float torque) {
    torques.push_back(torque);
}

void World::bodiesUpdate(float dt) {
    //this->AddTorque(20.f);

   // Loop all bodies of the world applying forces
    for (auto e : entities) {
        // Apply the weight force to all bodies
        Vec2 weight = Vec2(0.0, e->body->mass * G * PIXELS_PER_METER);
        e->body->AddForce(weight);

        // Apply global forces (wind etc.) to all bodies
        for (auto force : forces) {
            e->body->AddForce(force);
        }

        // Apply global torque (wind etc.) to all bodies
        for (auto torque : torques) {
            e->body->AddTorque(torque);
        }
    }
    
}

void World::Update(float dt) 
{
    for (auto e : entities) {
        e->Update(dt);
    }
	//World::bodiesUpdate(dt);
}

void World::VerletCollisionsEffects(Body* a, Body* b)
{
	
}

void World::CheckCollisions() { // Narrow type only
    
}
