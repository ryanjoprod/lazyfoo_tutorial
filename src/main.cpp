// Using SDL and standard IO 
#include <csignal>
#include <string>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "LTexture.h"

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window wi'll be rendering to 
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer= NULL;

// Scene textures 
LTexture gFooTexture;
LTexture gBackgroundTexture;

// Scene sprites
// SDL_Rect gSpriteClips[4]; // SDL2 Implementation
SDL_FRect gSpriteClips[4]; // SDL3 Implementation
LTexture gSpriteSheetTexture;

// Loads individual image
// Loads only one file format at a time.
SDL_Texture* loadTexture(std::string path)
{
  // The final optimized image
  // SDL_Surface* optimizedSurface = NULL;

  // The final texture
  SDL_Texture* newTexture = NULL;

  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL)
  {
    printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
  }
  else
  {
    // Convert surface to screen format
    // optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);  // SDL2 Implementation
    // optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format);  // SDL3 Implementation
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL)
    {
      printf("Unable to optimize image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
    }

    // Get rid of old loaded surface
    // SDL_FreeSurface(loadedSurface);  // SDL2 Implementation
    SDL_DestroySurface(loadedSurface);  // SDl3 Implementation
  }

  return newTexture;
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
      // Create renderer for window
      // gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);  // SDL2 Implementation

      gRenderer = SDL_CreateRenderer(gWindow, NULL, SDL_RENDERER_ACCELERATED);  // SDL3 Implementation
      if (gRenderer == NULL)
      {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialize PNG Loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags ))
        {
          printf("SDL3_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
          success = false;
        }
      }
    }
  }

  return success;
}

// Loads media
bool loadMedia()
{
  // Loading success flag
  bool success = true;
  
  // Load sprite sheet texture
  if (!gSpriteSheetTexture.loadFromFile("images/sprites.png", gRenderer))
  {
    printf("Failed to load sprite sheet texture.\n");
    success = false;
  }
  else
  {
    // Set top left sprite
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = 100;
    gSpriteClips[0].h = 100;

     // Set top right sprite
    gSpriteClips[1].x = 100;
    gSpriteClips[1].y = 0;
    gSpriteClips[1].w = 100;
    gSpriteClips[1].h = 100;

    // Set bottom left sprite
    gSpriteClips[2].x = 0;
    gSpriteClips[2].y = 100;
    gSpriteClips[2].w = 100;
    gSpriteClips[2].h = 100;

    // Set bottom right sprite
    gSpriteClips[3].x = 100;
    gSpriteClips[3].y = 100;
    gSpriteClips[3].w = 100;
    gSpriteClips[3].h = 100;
  }

  // Nothing to load
  return success;
}

// Frees media and shuts down SDL
void close()
{
  // Free loaded image
  gFooTexture.free();
  gBackgroundTexture.free();

  // Destroy window
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;

  // Quit SDL subsystems
  IMG_Quit();
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
      while(!quit)
      {
        while(SDL_PollEvent(&e) != 0)
        {
          if (e.type == SDL_EVENT_QUIT)
          {
            quit = true;
          }
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // Render top left sprite
        gSpriteSheetTexture.render(gRenderer, 0, 0, &gSpriteClips[0]);

        // Render top right sprite
        gSpriteSheetTexture.render(gRenderer, SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);

        // Render bottom left sprite
        gSpriteSheetTexture.render(gRenderer, 0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);

        // Render bottom right sprite
        gSpriteSheetTexture.render(gRenderer, SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);

        // Update screen
        SDL_RenderPresent(gRenderer);
      }
    }
  }
  // Free resources and close SDL
  close();

  return 0;
}
