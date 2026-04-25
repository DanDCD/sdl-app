#pragma once

#include <memory>

#include <SDL3/SDL_video.h>

// helper struct because SDL_Window has a C-style destructor (cannot rely on `delete`)
struct SDL_Window_Deleter
{
    void operator()(SDL_Window* w) const;
};

class AppState
{
public:
AppState(SDL_Window* window);
~AppState() = default;

private:


public:
std::unique_ptr<SDL_Window, SDL_Window_Deleter> d_window;



};