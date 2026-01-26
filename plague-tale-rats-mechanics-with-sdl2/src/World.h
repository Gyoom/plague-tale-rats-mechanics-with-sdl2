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
class Cell;

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
        std::vector<std::unique_ptr<Swarm>> swarms;
		std::vector<std::unique_ptr<Rat>> rats;

		// Cell attractiveness parameters
        float _w_occupancy = 1.0f;
        float _attractiveness_sharpness = 1.0f;
        float _w_attractiveness = 0.5f;
		// pursue target parameters
        float _w_pursue = 10.0f;
		// separation parameters
		float _w_separation = 1000.0f;
		// obstacle avoidance parameters
        float _obstacleSlowingFactor = 2.0f;
        float _w_obstacleAvoidance = 1.0f;

        // Debug cells attractiveness
        Rat* ratDebug = nullptr;
        Vec2 dirDebug = Vec2(0, 0);
        std::vector<Cell*> neigDebug;
        
        float sppedAdjustementRadius = 0.0f;

		// Constructor and Destructor

        World(float gravity);
        ~World();

        void AddBody(Body* body);
        std::vector<Body*>& GetBodies();

        void AddForce(const Vec2& force);
        void AddTorque(float torque);

		void RatsUpdate(float dt);
		void bodiesUpdate(float dt);
        void Update(float dt);

		void VerletCollisionsEffects(Body* a, Body* b);
        void CheckCollisions();

		void Render();

        Vec2 GetDirToBestNeighborCell(const Rat* r, const std::vector<Cell*>& neighbors);
        float GetCellAttractiveness(const Cell* c) const;

};
