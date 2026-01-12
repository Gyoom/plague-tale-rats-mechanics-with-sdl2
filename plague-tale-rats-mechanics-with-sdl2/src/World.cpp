#include "World.h"
#include "Constants.h"
#include "Graphics.h"
#include <iostream>

World::World(float gravity) {
    G = -gravity;
	grid = new Grid(50);
}

World::~World() {
	
    delete player;

    for (auto rat : rats) {
        delete rat;
    }
    delete grid;
}

void World::AddBody(Body* body) {
    bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies() {
    return bodies;
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
    for (auto body : bodies) {
        // Apply the weight force to all bodies
        Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
        body->AddForce(weight);

        // Apply global forces (wind etc.) to all bodies
        for (auto force : forces) {
            body->AddForce(force);
        }

        // Apply global torque (wind etc.) to all bodies
        for (auto torque : torques) {
            body->AddTorque(torque);
        }
    }

    // Update all the bodies in the world (integrating and transforming vertices)

    for (auto body : bodies) {
        body->Update(dt);
        body->ClearForces();
    }
    
}

void World::Update(float dt) 
{
	this->player->Update(dt);
	//World::bodiesUpdate(dt);
}

void World::VerletCollisionsEffects(Body* a, Body* b)
{
	
}

void World::CheckCollisions() { // Narrow type only
    
}
