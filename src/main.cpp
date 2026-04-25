#define SDL_MAIN_USE_CALLBACKS 1 // use callbacks for app entrypoints instead of main()

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Log("Hello World!");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_Log("Spamming Logs");


    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_Log("Quit");

}