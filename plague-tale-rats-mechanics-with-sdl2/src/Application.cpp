#include "Application.h"

#include "Constants.h"
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "../Common.h"
#include "Graphics.h"
#include "World.h"
#include "Boid.h"
#include "../Rat.h"
#include "Body.h"
#include "Vec2.h"
#include "../Grid.h"
#include "../Swarm.h"
#include "../Cell.h"
#include "Shape.h"
#include "Player.h"
#include "../Light.h"


using namespace std;

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
	
    world = make_unique<World>(0);
    srand(time(NULL));

	// Create boids
	/*unique_ptr<Boid> boid;
    for (int i = 0; i < 100; i++) {
        boid = make_unique<Boid>(
            make_unique<Body>(
                PolygonShape({
                    Vec2(25, 0),
                    Vec2(-10, 10),
                    Vec2(-10, -10),
                }),
                Vec2(rand() % Graphics::Width(), rand() % Graphics::Height()),
                1.0f,
				200.0f,
				300.0f,
                3,
                false
            ),
            "assets/rat.png",
            i == 0
        );
		boid->body->velocity = Vec2(-1.0f, 0.0f).Rotate((float)(rand() % 360) * (M_PI / 180.0f)) * boid->body->maxVelocity; // random initial velocity
        world->boids.push_back(move(boid));
	}*/

    
    // Rats
	world->grid = make_unique<Grid>(40);

    // Create player
    world->player = make_unique<Player>(
        make_unique<Body>(
            PolygonShape({
                Vec2(30, 0),
                Vec2(-15, 15),
                Vec2(-15, -15),
                }),
                Vec2(Graphics::Width() / 2.0f, Graphics::Height() * 0.8f),
                1.0f,
                0.0f,
                300.0f,
                3,
                false
                ),
        "assets/player_ship.png"
    );
    world->player->body->_color = 0xFF00FF00;
    world->player->body->forward = Vec2(0.0f, -1.0f);
    world->player->body->rotation = -M_PI / 2.0f;

    // Create light
    world->light = make_unique<Light>(world->player->body->position, 300.0f, 250.0f, 200.0f);

	// Create swarms
    world->swarms.push_back(
        make_unique<Swarm>(
            100,
            Vec2(Graphics::Width() * 0.12f, Graphics::Height() * 0.78f),
            true,
            1.0f,
            false
        )
    );

    world->swarms.push_back(
        make_unique<Swarm>(
            200,
            Vec2(Graphics::Width() * 0.75f, Graphics::Height() * 0.25f),
            true,
            1.0f,
            false
        )
    );

    world->swarms.push_back(
        make_unique<Swarm>(
            50,
            Vec2(Graphics::Width() * 0.4f, Graphics::Height() * 0.25f),
            false,
            1.0f,
            true
        )
    );
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {


        switch (event.type) {
           case SDL_QUIT:
                running = false;
                break;
            
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_F1)
                    debug = !debug;
                if (event.key.keysym.sym == SDLK_r) {
                    reload = true;
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_q) {
					world->player->_movementDirection.x = -1.0f;
				}
				if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
					world->player->_movementDirection.x = 1.0f;
				}
				if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
					world->player->_movementDirection.y = -1.0f;
				}
				if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
					world->player->_movementDirection.y = 1.0f;
				}

                break;

			case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_q) {
					if (world->player->_movementDirection.x < 0.0f)
                        world->player->_movementDirection.x = 0.0f;
                }
                if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
					if (world->player->_movementDirection.x > 0.0f)
                        world->player->_movementDirection.x = 0.0f;
                }
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
                   if (world->player->_movementDirection.y < 0.0f)
					   world->player->_movementDirection.y = 0.0f;
                }
                if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
					if (world->player->_movementDirection.y > 0.0f)
						world->player->_movementDirection.y = 0.0f;
                }
				break;

            
            case SDL_MOUSEBUTTONDOWN:
                /*if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Vec2 mousePos = Vec2(x, y);

                    Vec2 playerPos = world->player->body->position;

					Vec2 direction = mousePos - playerPos;
					direction.Normalize();
					world->player->body->AddForce(direction * 50000);
					
                }*/

               /* if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Vec2 mousePos = Vec2(x, y);
					world->player->SetNewMove(mousePos);
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {

                }
                if (event.button.button == SDL_BUTTON_MIDDLE) {

                }*/
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    Graphics::ClearScreen(0xFF222222); // background function (reset les pixels pas les objets)
    
    // Wait some time until the reach the target frame time in milliseconds
    // calcul du Delta Time, bloquant jusqu'à ce que la durée du DT soit passé.
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
        deltaTime = 0.016; // si on place un breakpoint, l'execution est suspendue mais le delta time continue d'augmenter

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    // Update world bodies (integration, collision detection, etc.)
    world->Update(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////

void Application::RenderBodies() {
	world->Render();
}

void Application::RenderText() {
    
	

}

void Application::Render() {
    
	RenderBodies();

	RenderText();

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
}