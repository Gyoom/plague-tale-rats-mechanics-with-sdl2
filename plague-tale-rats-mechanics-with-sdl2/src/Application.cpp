#include "Application.h"
#include "Constants.h"
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <SDL_image.h>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
	
    world = new World(0);
    //world->grid = new Grid(50);

	//Player* player = new Player(
 //       new PlayerBody(
 //           PolygonShape({
 //               Vec2(25, 0),
 //               Vec2(-10, 10),
 //               Vec2(-10, -10),
	//			}),
 //           Vec2(Graphics::Width() / 2.0f, Graphics::Height() / 2.0f),
 //           1.0f,
 //           false
 //       ),
 //       "assets/player.png"
	//);
 //   world->player = player;
	//world->AddBody(player->body);
	//srand(time(NULL));
	//Vec2 dir = Vec2(-1.0f, 0.0f).Rotate((float)(rand() % 360) * (M_PI / 180.0f));
 //   world->player->body->velocity = dir * world->player->body->maxVelocity; // random initial velocity
	//std::cout << "Player created with dir (" << dir.x << ", " << dir.y << ")\n";
	//std::cout << "Player max velocity: " << world->player->body->maxVelocity << "\n";
	//std::cout << "Player created with vel (" << world->player->body->velocity.x << ", " << world->player->body->velocity.y << ")\n";


    srand(time(NULL));
    for (int i = 0; i < 50; i++) {
        Boid* rat = new Boid(
            new Body(
                PolygonShape({
                    Vec2(25, 0),
                    Vec2(-10, 10),
                    Vec2(-10, -10),
                }),
                Vec2(rand() % Graphics::Width(), rand() % Graphics::Height()),
                1.0f,
                false
            ),
            "assets/rat.png",
            i == 0
        );
		rat->body->velocity = Vec2(-1.0f, 0.0f).Rotate((float)(rand() % 360) * (M_PI / 180.0f)) * rat->body->maxVelocity; // random initial velocity
        world->rats.push_back(rat);
        world->AddEntity(rat);
	}
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
                if (event.key.keysym.sym == SDLK_d)
                    debug = !debug;
                if (event.key.keysym.sym == SDLK_r) {
                    reload = true;
                    running = false;
                }
                break;

            
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    /*int x, y;
                    SDL_GetMouseState(&x, &y);
                    Vec2 mousePos = Vec2(x, y);

                    Vec2 playerPos = world->player->body->position;

					Vec2 direction = mousePos - playerPos;
					direction.Normalize();
					world->player->body->AddForce(direction * 50000);*/
					
                }

                if (event.button.button == SDL_BUTTON_RIGHT) {
					world->player->body->AddTorque(20000);
                }

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
    for(auto e : world->GetEntities()) {
        e->Render();
	}

    /*if (world->grid != nullptr)
    {
        Graphics::DrawCircle(0, 0, 5, 0, 0xF90B00FF);
        float cellSize = world->grid->GetCellSize();
        for (auto cols : world->grid->GetCells()) {
            for (auto cell : cols) {
                Vec2 cellPos = cell->GetWorldPosition();
                Graphics::DrawRect(cellPos.x, cellPos.y, cellSize, cellSize, 0x55555555);
            }
        }
    }*/
    
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
    delete world;
}