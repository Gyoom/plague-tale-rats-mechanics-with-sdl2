
#include "Rat.h"
#include "World.h"

Rat::Rat(Body* body, const char* textureFileName) : Entity(body, textureFileName)
{
	type = RAT;
	std::cout << "Creating Rat" << std::endl;

	maxForce *= multiplier;
	cohesionWeight *= multiplier;
	separationWeight *= multiplier;
	alignmentWeight *= multiplier;
	body->maxVelocity *= multiplier;
}

Rat::~Rat()
{
	std::cout << "Deleting Rat" << std::endl;
}

void Rat::Update(float dt)
{
	// Detect neighboring rats within detection radius
	neighbors.clear();
	for (auto r : World::instance->rats)
	{
		if (r != this && r->body->position.Dist(body->position) < detectionRadius)
			neighbors.push_back(r);
	}


	// Calculate steering forces
	Vec2 steeringForce = Vec2(0.0f, 0.0f);

    // Alignment force
    Vec2 alignmentForce = Vec2(0.0f, 0.0f);

    if (neighbors.size() > 0)
    {
        for (auto rat : neighbors)
        {
            alignmentForce += rat->body->velocity;
        }

        alignmentForce /= (float) neighbors.size();
        alignmentForce = alignmentForce.Normalize() * body->maxVelocity;
        alignmentForce -= body->velocity;

    }
    steeringForce += alignmentForce * alignmentWeight;

    // Separation force
    Vec2 separationForce = Vec2(0.0f, 0.0f);
    if (neighbors.size() > 0)
    {
        for (auto rat : neighbors)
        {
            Vec2 toRat = body->position - rat->body->position;
            float dist = toRat.Magnitude();
            if (dist > 0.0f && dist < separationDistance)
            {
                separationForce += toRat.Normalize() / dist; // plus proche = plus fort
            }
        }
        separationForce /= (float)neighbors.size();
        separationForce = separationForce.Normalize() * body->maxVelocity;
        separationForce -= body->velocity; 
    }
	steeringForce += separationForce * separationWeight; // Scale separation force

    //cohesion force
    Vec2 cohesionForce = Vec2(0.0f, 0.0f);
    if (neighbors.size() > 0)
    {
        Vec2 centerOfMass = Vec2(0.0f, 0.0f);
        for (auto rat : neighbors)
        {
            centerOfMass += rat->body->position;
        }
        centerOfMass /= (float) neighbors.size();
        Vec2 toCOM = centerOfMass - body->position;
        cohesionForce = toCOM.Normalize() * body->maxVelocity - body->velocity;
    }
    steeringForce += cohesionForce * cohesionWeight; // Scale cohesion force
    
	// Limit the steering force
    steeringForce = steeringForce.Clamp(steeringForce, maxForce);
    body->AddForce(steeringForce);

	Entity::Update(dt);
}
