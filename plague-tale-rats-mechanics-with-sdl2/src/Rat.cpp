
#include "Rat.h"

Rat::Rat(Body* body, const char* textureFileName) : Entity(body, textureFileName)
{
	type = RAT;
	std::cout << "Creating Rat" << std::endl;
}

Rat::~Rat()
{
	std::cout << "Deleting Rat" << std::endl;
}

void Rat::Update(float dt)
{
	// Apply a forward force in the direction of the rat's current rotation
	float angleRad = body->rotation * (3.14159265f / 180.0f);
	Vec2 forwardDirection(cos(angleRad), sin(angleRad));
	forwardDirection.Normalize();
	Vec2 forwardForce = forwardDirection * forwardForceMagnitude;
	body->AddForce(forwardForce);
	body->Update(dt);
}
