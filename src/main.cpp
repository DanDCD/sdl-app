#include "SDL3/SDL_events.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_video.h"
#include <memory>
#define SDL_MAIN_USE_CALLBACKS 1 // use callbacks for app entrypoints instead of main()

#include "memory"

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "app/app_state.h"


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Log("Initializing App...");
    
    auto sdl3Booted = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Log("SDL3 boot %s", sdl3Booted ? "succeeded" : "failed");
    
    *appstate = std::make_unique<AppState>(SDL_CreateWindow("window", 100, 100, SDL_WINDOW_OPENGL)).release();
    return SDL_APP_CONTINUE;
}

// called repeatedly - no guaranteed timing/refresh
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // SDL_Log("Spamming Logs");
    
    
    return SDL_APP_CONTINUE;
}

// invoked whenever an event is created (can be from any thread and concurrent with app iterate)
SDL_AppResult SDL_AppEvent(void *appstate_vptr, SDL_Event *event)
{
    AppState* appState_ptr = static_cast<AppState*>(appstate_vptr);

    switch(event->type)
    {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            return SDL_APP_SUCCESS;
            break;
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    std::unique_ptr<AppState> terminatingAppState{static_cast<AppState*>(appstate)};
    SDL_Log("Quitting App");
}