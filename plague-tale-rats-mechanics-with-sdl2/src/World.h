#pragma once

#include <vector>
#include <memory>
#include <SDL.h>

#include "Vec2.h"

class Swarm;
class Rat;
class Player;
class Boid;
class Body;
class Grid;
class Light;

class World {
    private:
        float G = 9.8f;

		std::vector<Body*> entities;
		// Global Forces and Torque
        std::vector<Vec2> forces;
        std::vector<float> torques;


    public:
		static World* instance;
        SDL_Texture* bgTexture = nullptr;

        std::unique_ptr<Player> player = nullptr;
		std::unique_ptr<Light> light = nullptr;
        
        // boids
		std::vector<std::unique_ptr<Boid>> boids;

		// Rats
        std::unique_ptr<Grid> grid = nullptr;
		std::unique_ptr<Swarm> swarm = nullptr;
		std::vector<std::unique_ptr<Rat>> rats;

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

		void Render();
};
