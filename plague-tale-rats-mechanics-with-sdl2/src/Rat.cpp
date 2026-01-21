
#include "Rat.h"
#include "World.h"

Rat::Rat(Body* body, const char* textureFileName) : Entity(body, textureFileName)
{
	type = RAT;
	std::cout << "Creating Rat" << std::endl;

	cohesionWeight *= multiplier;
	separationWeight *= multiplier;
	alignmentWeight *= multiplier;
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
        {
            Vec2 forward = body->forward;
            Vec2 toOther = r->body->position - body->position;
            float dist = toOther.Magnitude();

            Vec2 dirToOther = toOther / dist;

            float cosAngle = forward.Dot(dirToOther);

            float cosThreshold = cos(detectionAngleRad);

            if (cosAngle >= cosThreshold)
            {
                neighbors.push_back(r);
            }
        }
	}


	// Calculate steering forces
	Vec2 steeringForce = Vec2(0.0f, 0.0f);

    // Alignment force
    Vec2 alignmentForce = Vec2(0.0f, 0.0f);

    if (neighbors.size() > 0)
    {
		Vec2 avgFlockVelocity = Vec2(0.0f, 0.0f);
        for (auto rat : neighbors)
        {
            avgFlockVelocity += rat->body->velocity;
        }
        avgFlockVelocity /= (float) neighbors.size();

        alignmentForce = avgFlockVelocity.Normalize() * body->maxVelocity - body->velocity;

    }
	alignmentForce *= alignmentWeight; // Scale alignment force
	body->AddForce(Vec2::ClampMag(alignmentForce, maxForce));

    // Separation force
    Vec2 separationForce = Vec2(0.0f, 0.0f);
    if (neighbors.size() > 0)
    {
		Vec2 avgSeparation = Vec2(0.0f, 0.0f);
        for (auto rat : neighbors)
        {
            Vec2 toRat = body->position - rat->body->position;
            float dist = toRat.Magnitude();
            if (dist > 0.0f && dist < separationDistance)
            {
                avgSeparation += toRat.Normalize() / dist; // plus proche = plus fort
            }
        }
        avgSeparation /= (float)neighbors.size();

        separationForce = avgSeparation.Normalize() * body->maxVelocity - body->velocity;
    }
    separationForce *= separationWeight; // Scale separation force
	body->AddForce(Vec2::ClampMag(separationForce, maxForce));

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

		Vec2 toCOM = centerOfMass - body->position; // Vector pointing to the center of mass

        cohesionForce = toCOM.Normalize() * body->maxVelocity - body->velocity;
    }
	cohesionForce *= cohesionWeight; // Scale cohesion force
    body->AddForce(Vec2::ClampMag(cohesionForce, maxForce));
    
	Entity::Update(dt);
}
