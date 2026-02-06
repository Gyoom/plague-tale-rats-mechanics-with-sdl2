#include "swarm.h"
#include "src/World.h"
#include "Common.h"
#include "rat.h"
#include "Cell.h"
#include "src/Graphics.h"
#include "src/Vec2.h"
#include "src/Body.h"
#include "Grid.h"
#include "src/Constants.h"
#include "src/Player.h"
#include "src/Shape.h"
#include <set>
#include <map>


using namespace std;

Swarm::Swarm(int ratCount, Vec2 pos, bool fixed, float cohesionRadiusFactor, bool debugActive)
{
	position = pos;
	this->fixed = fixed;
	_w_cohesion_radius_factor = cohesionRadiusFactor;
	

	cohesionRadius = static_cast<float>(ratCount) * _w_cohesion_radius_factor;
	int cohesionRadiusInt = static_cast<float>(floor(cohesionRadius));
	
	World* world = World::instance;
	unique_ptr<Rat> rat;
	for (int i = 0; i < ratCount; i++) {
		rat = make_unique<Rat>(
			make_unique<GridBody>(
				PolygonShape({
					Vec2(25, 0),
					Vec2(-10, 10),
					Vec2(-10, -10),
					}),
					Vec2(
						pos.x - cohesionRadiusInt + (rand() % cohesionRadiusInt * 2),
						pos.y - cohesionRadiusInt + (rand() % cohesionRadiusInt * 2)
					),
					1.0f,
					200.0f,
					300.0f,
					0,
					false
					),
			"assets/rat.png",
			debugActive && i == 0 // display debug tools for the first rat only
		);

		if (debugActive && i == 0)
			rat->body->_color = 0xFFFFFF00; // yellow for the debug rat

		rat->body->velocity = Vec2(-1.0f, 0.0f).Rotate((float)(rand() % 360) * (M_PI / 180.0f)) * rat->body->maxVelocity; // random initial velocity
		world->rats.push_back(move(rat));
		rats.push_back(world->rats.back().get());
	}

	
}

void Swarm::Update(float dt)
{
	centerOfMass = Vec2(0.0f, 0.0f);
	for (auto rat : rats)
	{
		centerOfMass += rat->body->position;
	}
	centerOfMass /= (float) rats.size();

	for (auto& rat : rats) {

		// Swarm cohesion
		Vec2 toCOM = (fixed ? position : centerOfMass) - rat->body->position;
		float dist = toCOM.Magnitude();
		if (dist > 0.0001f)
		{
			// Cohesion with other rats
			Vec2 dir = toCOM / dist;

			// Cohesion radius based on swarm size
			
			float t = dist / cohesionRadius;

			// Cohésion exponentielle after a certain distance
			float strength = std::exp((t - 1.0f) * _cohesion_sharpness);

			Vec2 cohesionForce = dir * strength;

			cohesionForce = Vec2::ClampMag(cohesionForce, rat->_maxForce);
			cohesionForce *= _w_cohesion;
			cohesionForce *= rat->_w_cohesion_target;
			rat->body->AddForce(cohesionForce);
		}
	}

	

}

void Swarm::Render()
{
	

	if (DISPLAY_SWARM_COHESION_DEBUG) {
		// center of mass
		Vec2 COM = fixed ? position : centerOfMass;
		Graphics::DrawFillCircle(
			static_cast<int>(COM.x),
			static_cast<int>(COM.y),
			1,
			0xFF0000FF
		);
		// cohesion radius
		Graphics::DrawCircle(
			static_cast<int>(COM.x),
			static_cast<int>(COM.y),
			static_cast<int>(cohesionRadius),
			0,
			0xFF00FFFF
		);
	}
	
}

void Swarm::RemoveDeadRats()
{
	// Remove dead rats from the swarm
	rats.erase(
		std::remove_if(
			rats.begin(),
			rats.end(),
			[](Rat* r)
			{
				return r->_pendingKill;
			}
		),
		rats.end()
	);
}
