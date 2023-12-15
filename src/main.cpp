// Using SDL and standard IO
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <cstddef>
#include <stdio.h>
#include <SDL3/SDL.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
  // The window we'll be rending to
  SDL_Window* window = NULL;

  // The surface contained by the window
  SDL_Surface* screenSurface = NULL;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    // Create window
    // SDL2 Implementation
    // window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOWEVENT_SHOWN);
    // SDL3 Implementation
    window = SDL_CreateWindow("SDL Tutorial", SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
      // Get window surface
      screenSurface = SDL_GetWindowSurface(window);

      // Fill the surface white
      // SDL2 Implementation
      // SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
      SDL_FillSurfaceRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

      // Update the surface
      SDL_UpdateWindowSurface(window);

      // Hack to get widow to stay up 
      // This hack is for MacOS
      SDL_Event e;
      bool quit = false;
      while(quit == false)
      {
        while(SDL_PollEvent(&e))
        {
          // SDL2 Implementation
          // if (e.type == SDL_QUIT)
          // SDL3 Implementation
          if (e.type == SDL_EVENT_QUIT)
            quit = true;
        }
      }
    }
  }

  // Destroy window
  SDL_DestroyWindow(window);

  // Quit SDL subsystems
  SDL_Quit();

  return 0;
}
