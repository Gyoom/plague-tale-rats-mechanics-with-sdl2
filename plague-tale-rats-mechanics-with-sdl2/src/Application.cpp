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
    running = Graphics::OpenWindow(1920, 1200);
	
    world = new World(0);

	Player* player = new Player(
        new Body(
            CircleShape(30),
            Vec2(Graphics::Width() / 2.0f, Graphics::Height() / 2.0f),
            1.0f,
            false
        ),
        "assets/player.png"
	);
    world->player = player;
	world->AddBody(player->body);

    srand(time(NULL));
   /* for (int i = 0; i < 30; i++) {
        Rat* rat = new Rat(
            new Body(
                CircleShape(10),
                Vec2(rand() % Graphics::Width(), rand() % Graphics::Height()),
                1.0f,
                false
            ),
            "assets/rat.png"
        );
        world->rats.push_back(rat);
        world->AddBody(rat->body);
	}*/
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
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Vec2 mousePos = Vec2(x, y);
					world->player->SetNewMove(mousePos);
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {

                }
                if (event.button.button == SDL_BUTTON_MIDDLE) {

                }
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
    
	/*Body* player = world->player->body;
	CircleShape* circleShape = (CircleShape*) player->shape;
    Graphics::DrawCircle(player->position.x, player->position.y, circleShape->radius, player->rotation, player->isColliding ? 0xF90B00FF : 0xFF00FF00);

    for (auto rat : world->rats) {
        Body* body = rat->body;
		CircleShape* circleShape = (CircleShape*)body->shape;
		Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, body->isColliding ? 0xF90B00FF : 0xFF0000FF);
    }*/

    Graphics::DrawCircle(0, 0, 25, 0, 0xF90B00FF);
	float cellSize = world->grid->GetCellSize();
    for (auto cols : world->grid->GetCells()) {
        for (auto cell : cols) {
			Vec2 cellPos = cell->GetWorldPosition();
            Graphics::DrawRect(cellPos.x, cellPos.y, cellSize, cellSize, 0x55555555);
        }
	}
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