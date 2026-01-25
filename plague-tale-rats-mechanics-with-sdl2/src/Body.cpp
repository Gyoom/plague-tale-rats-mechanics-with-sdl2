
#include "Body.h"

#include <iostream>
#include <math.h>
#include "Graphics.h"
#include "World.h"
#include "Vec2.h"
#include "Shape.h"
#include "../Grid.h"

// Base Body class implementation

Body::Body(const Shape& shape, Vec2 pos, float mass, bool canCollide) {
    this->shape = shape.Clone();
    this->position = pos;
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->sumForces = Vec2(0, 0);
    this->sumTorque = 0.0;
    this->restitution = 1.0;
    this->friction = 0.7;
    this->mass = mass;
	this->canCollide = canCollide;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;
    }
    I = shape.GetMomentOfInertia() * mass;
    if (I != 0.0) {
        this->invI = 1.0 / I;
    } else {
        this->invI = 0.0;
    }
	World::instance->AddBody(this);
}

Body::~Body() {
    SDL_DestroyTexture(texture);
}

void Body::SetTexture(const char* textureFileName) {
    SDL_Surface* surface = IMG_Load(textureFileName);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }
}

bool Body::IsStatic() const {
    const float epsilon = 0.005f;
    return fabs(invMass - 0.0) < epsilon;
}

void Body::AddForce(const Vec2& force) {
    sumForces = sumForces + force;
}

void Body::AddTorque(float torque) {
    sumTorque += torque;
}

void Body::ClearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Body::ClearTorque() {
    sumTorque = 0.0;
}

void Body::ApplyImpulse(const Vec2& j) {
    if (IsStatic()) {
        return;
    }
    velocity += j * invMass;
}

void Body::ApplyImpulse(const Vec2& j, const Vec2& r) {
    if (IsStatic()) {
        return; 
    }
    velocity += j * invMass;
    angularVelocity += r.Cross(j) * invI;
}

void Body::IntegrateLinear(float dt) {
    if (IsStatic()) {
        return;
    }

    acceleration = sumForces * invMass;

    velocity += acceleration * dt;

    // 🔑 DRAG
    velocity *= std::exp(-linearDrag * dt);

    // 🔑 SPEED LIMIT
    velocity = Vec2::ClampMag(velocity, maxVelocity, minVelocity);

    position += velocity * dt;

    if (velocity.MagnitudeSquared() > 0.0001f)
    {
        forward = velocity;
		forward.Normalize();
        rotation = atan2(forward.y, forward.x);
    }

    ClearForces();
}

void Body::IntegrateAngular(float dt) {
    if (IsStatic()) {
        return;
    }

    // Find the angular acceleration based on the torque that is being applied and the moment of inertia
    angularAcceleration = sumTorque * invI;

    // Integrate the angular acceleration to find the new angular velocity
    angularVelocity += angularAcceleration * dt;

    // Integrate the angular velocity to find the new rotation angle
    rotation += angularVelocity * dt;

    // Clear all the torque acting on the object before the next physics step
    ClearTorque();
}

void Body::CheckLimits()
{
    // Limit position to stay within window bounds
    if (position.x < 0) 
        position.x = 0;
    if (position.x > Graphics::Width())
        position.x = Graphics::Width();

    if (position.y < 0) 
        position.y = 0;
    if (position.y > Graphics::Height()) 
        position.y = Graphics::Height();
    // Limit rotation to be within 0-360 degrees
    /*if (rotation >= 360.0f) rotation -= 360.0f;
	if (rotation < 0.0f) rotation += 360.0f;*/
}

void Body::Update(float dt) {
    
    IntegrateLinear(dt);
    //IntegrateAngular(dt);

    CheckLimits();

    shape->UpdateVertices(rotation, position);
}

void Body::Render() {

    if (Graphics::debugMode || texture == nullptr)
	    shape->DebugRender(position);
}


// GridBody class implementation

GridBody::GridBody(const Shape& shape, Vec2 pos, float mass, bool canCollide) : Body(shape, pos, mass, canCollide)
{
}

void GridBody::IntegrateLinear(float dt) {
    if (IsStatic()) {
        return;
    }

    acceleration = sumForces * invMass;

    velocity += acceleration * dt;

    // 🔑 DRAG
    velocity *= std::exp(-linearDrag * dt);

    // 🔑 SPEED LIMIT
    velocity = Vec2::ClampMag(velocity, _effectiveMaxVelocity, _effectiveMinVelocity);

    position += velocity * dt;

    if (velocity.MagnitudeSquared() > 0.0001f)
    {
        forward = velocity;
        forward.Normalize();
        rotation = atan2(forward.y, forward.x);
    }

    ClearForces();
}

void GridBody::CheckLimits()
{
	Grid* grid = World::instance->grid.get();

	float gridWidth = grid->GetColsCount() * grid->GetCellSize() - (1 + grid->GetCellSize());
    float gridHeight = grid->GetRowsCount() * grid->GetCellSize() - (1 + grid->GetCellSize());

    if (position.x < grid->GetCellSize())
        position.x = grid->GetCellSize();
    if (position.x > gridWidth)
        position.x = gridWidth;
    if (position.y < grid->GetCellSize())
        position.y = grid->GetCellSize();
    if (position.y > gridHeight)
		position.y = gridHeight;

}
