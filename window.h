#pragma once

#include <SDL.h>
#include <optional>

#include "utils/log.h"

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

namespace pong
{
    class window
    {
    private:
        std::optional<SDL_Window*> window;
        std::optional<SDL_GLContext> sdl_gl_context;

    public:
        bool init();
        void run();

    protected:
        virtual bool init_resources();
        virtual void process_input();
        virtual void render();
        virtual void cleanup();

        std::shared_ptr<spd::logger> log;
    };
}