// Using SDL and standard IO
#include <string>
#include <stdio.h>
#include <SDL3/SDL.h>

// Key press surfaces constants
enum KeyPressSurfaces
{
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window wi'll be rendering to 
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The image that correspons to a keypress 
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

// CUrrent displayed image
SDL_Surface* gCurrentSurface = NULL;

// Loads individual image
SDL_Surface* loadSurface(std::string path)
{
  // Load image at specified path
  SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
  if (loadedSurface == NULL)
  {
    printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
  }

  return loadedSurface;
}
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

  // Load default surface
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("images/press.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
  {
    printf("Failed to load default image!\n");
    success = false;
  }

  // Load up surface
  gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("images/up.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
  {
    printf("Failed to load up image!\n");
    success = false;
  }

  // Load down surface
  gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("images/down.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
  {
    printf("Failed to load down image!\n");
    success = false;
  }

  // Load left success
  gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("images/left.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
  {
    printf("Failed to load left image!\n");
    success = false;
  }

  gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("images/right.bmp");
  if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
  {
    printf("Failed to load right image!\n");
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
  SDL_DestroySurface(gCurrentSurface);
  gCurrentSurface = NULL;

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
      // Main loop flag
      bool quit = false;

      // Event handler
      SDL_Event e;

      // Set default current surface
      gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

      while(!quit)
      {
        while(SDL_PollEvent(&e) != 0)
        {
          if (e.type == SDL_EVENT_QUIT)
          {
            quit = true;
          }
          // User presses a key 
          // else if (e.type == SDL_KEYDOWN)  SDL2 Implamentation
          else if (e.type == SDL_EVENT_KEY_DOWN)  // SDL3 Implamentation
          {
            // Select surfaces based on key press
            switch (e.key.keysym.sym)
            {
              case SDLK_UP:
                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                break;
              case SDLK_DOWN:
                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                break;
              case SDLK_LEFT:
                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                break;
              case SDLK_RIGHT:
                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                break;
              default:
                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                break;
            }
          }
        }

        // Apply the image
        SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
      }
    }
  }
  // Free resources and close SDL
  close();

  return 0;
}
