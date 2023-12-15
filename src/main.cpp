// Using SDL and standard IO
#include <stdio.h>
#include <SDL3/SDL.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window wi'll be rendering to 
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The image we will load and show on the screen 
SDL_Surface* gHelloWorld = NULL;

// Starts up SDL and creates window
bool init()
{
  // Initiallization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    // Create window
    gWindow = SDL_CreateWindow("SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (gWindow == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

// Loads media
bool loadMedia()
{
  // Loading success flag
  bool success = true;

  // Load splash image
  gHelloWorld = SDL_LoadBMP("images/hello_world.bmp");
  if (gHelloWorld == NULL)
  {
    printf("Unable to load image %s SDL_Error: %s\n", "images/hello_world.bmp", SDL_GetError());
    success = false;
  }

  return success;
}

// Frees media and shuts down SDL
void close()
{
  // Deallocate surface
  // SDL2 Implamentation
  // SDL_FreeSurface(gHelloWorld);

  // SDL3 Implamentation
  SDL_DestroySurface(gHelloWorld);
  gHelloWorld = NULL;

  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}

int main(int argc, char* args[])
{
  // Start up SDL and create window
  if (!init())
  {
    printf("Failed to initialize!\n");
  }
  else
  {
    // Load media
    if (!loadMedia())
    {
      printf("Failed to load media!\n");
    }
    else
    {
      // Apply the image
      SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

      // Update the surface
      SDL_UpdateWindowSurface(gWindow);

      // Hack to get window to stay up 
      SDL_Event e;
      bool quit = false;
      while(quit == false)
      {
        while(SDL_PollEvent(&e))
        {
          if (e.type == SDL_EVENT_QUIT)
            quit = true;
        }
      }
    }
  }
  // Free resources and close SDL
  close();

  return 0;
}
