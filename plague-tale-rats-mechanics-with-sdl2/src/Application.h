#pragma once

#include <vector>
#include <SDL_ttf.h>
#include <memory>
#include "World.h"


class Application {
    private:
        bool debug = false;
        bool running = false;

        std::unique_ptr<World> world;


    public:
        bool reload = true;
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
		void RenderBodies();;
		void RenderText();
        void Render();
        void Destroy();
};