#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Vec2.h"
#include "Shape.h"

class Entity;

struct Body {
    bool isColliding = false;
	bool canCollide = true;

	Vec2 forward = Vec2(-1.0f, 0.0f);
	float maxVelocity = 5000.0f;
	float linearDrag = 2;
    float slowingRadius = 50.0f;

    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;
    
    // Forces and torque
    Vec2 sumForces;
    float sumTorque;

    // Mass and Moment of Inertia
    float mass;
    float invMass; // 1 * mass
    float I;
    float invI;

    // Coefficient of restitution (elasticity)
    float restitution;

    // Coefficient of friction
    float friction;

    // Pointer to the shape/geometry of this rigid body
    Shape* shape = nullptr;

    // Pointer to an SDL texture
    SDL_Texture* texture = nullptr;

    Body() = default;
    Body(const Shape& shape, Vec2 pos, float mass, bool canCollide = true);
    ~Body();

    bool IsStatic() const;

    void AddForce(const Vec2& force);
    void AddTorque(float torque);
    void ClearForces();
    void ClearTorque();

    void SetTexture(const char* textureFileName);

    void ApplyImpulse(const Vec2& j);
    void ApplyImpulse(const Vec2& j, const Vec2& r);

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

	void CheckLimits();
    void ApplyBoidsForces();

    virtual void Update(float dt);
};
