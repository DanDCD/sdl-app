#include "app_state.h"
#include "SDL3/SDL_video.h"

void SDL_Window_Deleter::operator()(SDL_Window * window) const {
    SDL_DestroyWindow(window);
}

AppState::AppState(SDL_Window *window) : d_window(window) {

}
