
#include "Boid.h"
#include "World.h"

Boid::Boid(Body* body, const char* textureFileName, bool displayDebugTools) : Entity(body, textureFileName)
{
	type = RAT;
	_displayDebugTools = displayDebugTools;
}

Boid::~Boid()
{
}

void Boid::Update(float dt)
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

void Boid::Render()
{
    Entity::Render();
    if (_displayDebugTools)
    {
		// Draw detection radius
        Graphics::DrawCircle(body->position.x, body->position.y, detectionRadius, 0, 0xFFFFFFFF);
        float angle = atan2(body->forward.y, body->forward.x);
		// Draw forward line
        Vec2 limit = body->position + body->forward * detectionRadius;
        Graphics::DrawLine(
            body->position.x,
            body->position.y,
            limit.x,
            limit.y,
            0xFF00FF00
        );

		// Draw detection angle lines
        limit = body->position + Vec2(std::cos(angle - detectionAngleRad), std::sin(angle - detectionAngleRad)) * detectionRadius;
        Graphics::DrawLine(
            body->position.x,
            body->position.y,
            limit.x,
            limit.y,
            0xFFFFFFFF
        );

        limit = body->position + Vec2(std::cos(angle + detectionAngleRad), std::sin(angle + detectionAngleRad)) * detectionRadius;
        Graphics::DrawLine(
            body->position.x,
            body->position.y,
            limit.x,
            limit.y,
            0xFFFFFFFF
        );
		// Draw lines to neighbors
        for (auto n : neighbors)
        {
            Graphics::DrawLine(
                body->position.x,
                body->position.y,
                n->body->position.x,
                n->body->position.y,
                0xFF0000FF
            );
        }
	}
}
