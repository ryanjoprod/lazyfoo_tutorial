// Using SDL and standard IO 
#include <cstddef>
#include <string>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window wi'll be rendering to 
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// Current displayed image
SDL_Surface* gCurrentSurface = NULL;

// The window renderer
SDL_Renderer* gRenderer= NULL;

// Current displayed texture 
SDL_Texture* gTexture = NULL;

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

  gTexture = loadTexture("images/viewport.png");
  if (gTexture == NULL)
  {
    printf("Failed to load viewport image!\n");
    success = false;
  }
  
  // Nothing to load
  return success;
}

// Frees media and shuts down SDL
void close()
{
  // Free loaded image
  SDL_DestroyTexture(gTexture);
  gTexture = NULL;

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
        
        // Top left corner viewport
        SDL_Rect topLeftViewport;
        topLeftViewport.x = 0;
        topLeftViewport.y = 0;
        topLeftViewport.w = SCREEN_WIDTH / 2;
        topLeftViewport.h = SCREEN_HEIGHT / 2;
        // SDL_RenderSetViewport(gRenderer, &topLeftViewport);  // SDL2 Implementation
        SDL_SetRenderViewport(gRenderer, &topLeftViewport);  // SDL3 Implementation

        // Render texture to screen
        // SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);  // SDL2 Implementation
        SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);  // SDL3 Implementation

        // Top right corner viewport
        SDL_Rect topRightViewport;
        topRightViewport.x = SCREEN_WIDTH / 2;
        topRightViewport.y = 0;
        topRightViewport.w = SCREEN_WIDTH / 2;
        topRightViewport.h = SCREEN_HEIGHT / 2;
        // SDL_RenderSetViewport(gRenderer, &topRightViewport);  // SDL2 Implementation
        SDL_SetRenderViewport(gRenderer, &topRightViewport);  // SDL3 Implementation

        // Render texture to screen
        // SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);  // SDL2 Implementation
        SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);  // SDL3 Implementation

        // Bottom viewport
        SDL_Rect bottomViewport;
        bottomViewport.x = 0;
        bottomViewport.y = SCREEN_HEIGHT / 2;
        bottomViewport.w = SCREEN_WIDTH;
        bottomViewport.h = SCREEN_HEIGHT / 2;
        // SDL_RenderSetViewport(gRenderer, &bottomViewport);  // SDL2 Implementation
        SDL_SetRenderViewport(gRenderer, &bottomViewport);  // SDL3 Implementation

        // Render texture to screen
        // SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);  // SDL2 Implementation
        SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);  // SDL3 Implementation

        // Update screen
        SDL_RenderPresent(gRenderer);
      }
    }
  }
  // Free resources and close SDL
  close();

  return 0;
}
