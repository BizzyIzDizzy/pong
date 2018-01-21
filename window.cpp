#include <string>
#include <glad/glad.h>

#include "utils/log.h"
#include "utils/common.h"
#include "gfx/common.h"
#include "window.h"

namespace pong
{
    /// Checks SDL for errors.
    /// @returns True if there were any SDL errors, false otherwise.
    bool check_sdl_error()
    {
        std::string error = SDL_GetError();

        if (!error.empty())
        {
            auto log = utils::get_default_logger();
            log->error("SLD Error: {}", error);
            SDL_ClearError();
            return true;
        }

        return false;
    }

    void framebuffer_resize(uint32_t width, uint32_t height)
    {
        glViewport(0, 0, width, height);
    }

    bool window::init()
    {
        log = utils::get_default_logger();

        // initialize SDL video
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            log->error("Failed to init SDL!");
            cleanup();
            return false;
        }

        // create a window compatible with OpenGL
        window = SDL_CreateWindow(
                "BD Pong",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                DEFAULT_WINDOW_WIDTH,
                DEFAULT_WINDOW_HEIGHT,
                SDL_WINDOW_OPENGL
        );

        if(check_sdl_error())
        {
            log->error("Unable to create a window!");
            cleanup();
            return false;
        }

        // Set our OpenGL version.
        // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // 3.3 is part of the modern versions of OpenGL, but most video cards whould be able to run it
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);

        // Turn on double buffering with a 24bit Z buffer.
        // You may need to change this to 16 or 32 for your system
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // create OpenGL context
        sdl_gl_context = SDL_GL_CreateContext(*window);

        if(check_sdl_error()){
            log->error("Could not create OpenGL context version {}.{}!", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
            cleanup();
            return false;
        }

        // This makes our buffer swap synchronized with the monitor's vertical refresh
        SDL_GL_SetSwapInterval(1);

        // initialize glad
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
        {
            log->error("Failed to initialize GLAD!");
            cleanup();
            return false;
        }

        // resize OpenGL viewport to match our window
        framebuffer_resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

        if(!init_resources())
        {
            log->error("Failed to initialize resources!");
            cleanup();
            return false;
        }

        return true;
    }

    void window::run()
    {
        auto loop = true;

        SDL_Event event{};

        while(loop)
        {
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    loop = false;
                }else if(event.type == SDL_WINDOWEVENT)
                {
                    if(event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        framebuffer_resize((uint32_t)event.window.data1, (uint32_t)event.window.data2);
                    }
                }
            }

            process_input();
            render();
            SDL_GL_SwapWindow(*window);
        }

        cleanup();
    }

    bool window::init_resources()
    {
        return true;
    }

    void window::cleanup()
    {
        // Delete our OpenGL context
        if(sdl_gl_context.has_value())
        {
            SDL_GL_DeleteContext(*sdl_gl_context);
        }

        // Destroy our window
        if(window.has_value())
        {
            SDL_DestroyWindow(*window);
        }

        // Shutdown SDL2
        SDL_Quit();
    }

    void window::process_input()
    {
    }

    void window::render()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}