
#include "Boid.h"
#include "World.h"

Boid::Boid(std::unique_ptr<Body> body, const char* textureFileName, bool displayDebugTools) : Entity(std::move(body), textureFileName)
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
	for (auto& r : World::instance->rats)
	{
        if (r.get() != this && r->body->position.Dist(body->position) < detectionRadius)
        {
            Vec2 forward = body->forward;
            Vec2 toOther = r->body->position - body->position;
            float dist = toOther.Magnitude();

            Vec2 dirToOther = toOther / dist;

            float cosAngle = forward.Dot(dirToOther);

            float cosThreshold = cos(detectionAngleRad);

            if (cosAngle >= cosThreshold)
            {
                neighbors.push_back(r.get());
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
    alignmentForce = Vec2::ClampMag(alignmentForce, maxForce);
	alignmentForce *= alignmentWeight; // Scale alignment force
	body->AddForce(alignmentForce);

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
	separationForce = Vec2::ClampMag(separationForce, maxForce);
    separationForce *= separationWeight; // Scale separation force
	body->AddForce(separationForce);

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
	cohesionForce = Vec2::ClampMag(cohesionForce, maxForce);
	cohesionForce *= cohesionWeight; // Scale cohesion force
    body->AddForce(cohesionForce);


	// ciustom check world bounds
	bool goingOutOfBounds = false;
	Vec2 futurePosition = body->position + body->forward * detectionRadius;
	goingOutOfBounds = !Graphics::IsInsideScreen(futurePosition);
	
    if (goingOutOfBounds)
    {
		srand(time(NULL));
		int checkDir = rand() % 2 == 0 ? 1 : -1; // random start direction
        float angle = 0;
        int i = 0;
        while (goingOutOfBounds && angle < (M_PI * 2.0f))
        {
            
            if (i % 2 == 0)
                angle += intervalAngleRad;

            checkDir = -checkDir;
			futurePosition = body->position + Vec2(std::cos(body->rotation + angle * checkDir), std::sin(body->rotation + angle * checkDir)) * detectionRadius;
			    
            goingOutOfBounds = !Graphics::IsInsideScreen(futurePosition);
            i++;

            if (goingOutOfBounds && _displayDebugTools)
            {
                Graphics::DrawLine(
                    body->position.x,
                    body->position.y,
                    futurePosition.x,
                    futurePosition.y,
                    0xFFFF0000
				);
            }
        }

		if (_displayDebugTools)
        { 
            Graphics::DrawLine(
                body->position.x,
                body->position.y,
                futurePosition.x,
                futurePosition.y,
                0xFF00FFFF
            );
        }
        

        if (angle < (M_PI * 2.0f))
        {
            Vec2 desiredVelocity = (futurePosition - body->position).Normalize() * body->maxVelocity;
            Vec2 boundaryAvoidanceForce = desiredVelocity - body->velocity;
            boundaryAvoidanceForce = Vec2::ClampMag(boundaryAvoidanceForce, maxForce);
            boundaryAvoidanceForce *= boundaryAvoidanceWeight; // Scale boundary avoidance force
            body->AddForce(boundaryAvoidanceForce);
        }
	}

	// Alternative boundary avoidance (simple)
    Vec2 boundaryForce(0, 0);
	float margin = detectionRadius / 2.0f;

    // gauche
    if (body->position.x < margin)
        boundaryForce.x += (margin - body->position.x) / margin;
    // droite
    if (body->position.x > Graphics::Width() - margin)
        boundaryForce.x -= (body->position.x - (Graphics::Width() - margin)) / margin;
    // haut
    if (body->position.y < margin)
        boundaryForce.y += (margin - body->position.y) / margin;
    // bas
    if (body->position.y > Graphics::Height() - margin)
        boundaryForce.y -= (body->position.y - (Graphics::Height() - margin)) / margin;

    // Normalize & scale
    if (boundaryForce.MagnitudeSquared() > 0.0001f)
    {
        boundaryForce = boundaryForce.Normalize() * body->maxVelocity;
        boundaryForce -= body->velocity;
        boundaryForce = Vec2::ClampMag(boundaryForce, maxForce) * boundaryAvoidanceWeight;
        body->AddForce(boundaryForce);
    }
    
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
