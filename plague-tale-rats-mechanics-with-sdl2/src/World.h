#pragma once

#include "../Grid.h"
#include "Body.h"
#include "Player.h"
#include "Rat.h"
#include "Vec2.h"
#include <vector>

class World {
    private:
        float G = 9.8;

		std::vector<Body*> bodies;
		// Global Forces and Torque
        std::vector<Vec2> forces;
        std::vector<float> torques;


    public:
		static World* instance;
        SDL_Texture* bgTexture = nullptr;
		Player* player = nullptr;
        std::vector<Rat*> rats;
		Grid* grid = nullptr;
        std::vector<Rat*> playersRats;

        World(float gravity);
        ~World();

        void AddBody(Body* body);
        std::vector<Body*>& GetBodies();

        void AddForce(const Vec2& force);
        void AddTorque(float torque);

		void bodiesUpdate(float dt);
        void Update(float dt);

		void VerletCollisionsEffects(Body* a, Body* b);
        void CheckCollisions();
};
