#include "Light.h"
#include "src/Graphics.h"
#include "SDL.h"
#include "Cell.h"
#include "src/World.h"
#include "Grid.h"
#include "src/Constants.h"
#include "Common.h"
#include "Rat.h"
#include "src/Body.h"

using namespace std;

Light::Light(Vec2 pos, float away, float mid, float close)
{
	position = pos;
	this->awayRadius = away;
	this->midRadius = mid;
	this->closeRadius = close;
}

void Light::Update(float dt)
{
	// Update light position to mouse position
	int x, y;
	SDL_GetMouseState(&x, &y);
	Vec2 mousePos = Vec2(x, y);
	if (Graphics::IsInsideScreen(mousePos))
		position = mousePos;


	for (auto& rat : World::instance->rats) {
		if (rat->_displayDebugTools)
		{
			std::cout << "drag: (" << rat->body->_effectiveLinearDrag << ")\n";
			std::cout << "drag is applied: (" << rat->body->_dragEnabled << ")\n";
			std::cout << "is in light: (" << rat->_isInLight << ")\n";
			std::cout << "vel: (" << rat->body->velocity.x << ";" << rat->body->velocity.y << ")\n";
			std::cout << "--------------------------------------\n";
			std::cout << std::endl;
		}


		Vec2 toLight = position - rat->body->position;
		float dist = toLight.Magnitude();

		if (dist > awayRadius)
			continue;

		// Apply repulsion force if the rat is within the light's influence radius
		float t = Common::saturate(1.0f - (dist  - midRadius) / (awayRadius - midRadius));

		Vec2 dirToRat = (rat->body->position - position).Normalize();

		Vec2 repel = dirToRat * t * rat->body->maxVelocity;
		repel -= rat->body->velocity;
		repel = Vec2::ClampMag(repel, rat->_maxForce);
		repel *= _w_lightRepulsion;
		rat->body->AddForce(repel);

		// Apply drag based on distance to light
		t = Common::saturate((dist - closeRadius) / (awayRadius - closeRadius));


		float localDrag = Common::lerp(_maxLightDrag, rat->body->_linearDrag, t);
		rat->body->_effectiveLinearDrag = max(rat->body->_effectiveLinearDrag, localDrag);

		


		// If the rat is very close to the light, apply a stronger impulse and lock its forward direction
		if (dist < closeRadius)
		{
			rat->body->velocity = Vec2(0, 0);
			float impulseStrength = rat->_target ? lightKickImpulse : lightKickImpulse / 2.0f;
			rat->body->AddImpulse(dirToRat * impulseStrength);
			rat->_lightSource = this;

			if (!rat->_isInLight)
			{
				rat->_isInLight = true;
				rat->_lightTimer = 0.0f;
			}
			rat->_repelTimer = 0.0f;
			rat->body->_dragEnabled = false;
			rat->body->fowardLocked = true;

		}
		else {
			rat->_isInLight = false;
		}
	}
}

void Light::Render()
{
	Graphics::DrawCircle(
		static_cast<int>(position.x), 
		static_cast<int>(position.y), 
		static_cast<int>(awayRadius), 
		0,
		0xFF000055
	);
	Graphics::DrawCircle(
		static_cast<int>(position.x), 
		static_cast<int>(position.y), 
		static_cast<int>(midRadius), 
		0,
		0xFF0000AA
	);

	Graphics::DrawCircle(
		static_cast<int>(position.x), 
		static_cast<int>(position.y), 
		static_cast<int>(closeRadius), 
		0,
		0xFF0000FF
	);
}
