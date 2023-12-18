// Using SDL and standard IO 
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstddef>
#include <string>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// Key press surfaces constants
enum KeyPressTextures
{
  KEY_PRESS_TEXTURE_DEFAULT,
  KEY_PRESS_TEXTURE_UP,
  KEY_PRESS_TEXTURE_DOWN,
  KEY_PRESS_TEXTURE_LEFT,
  KEY_PRESS_TEXTURE_RIGHT,
  KEY_PRESS_TEXTURE_TOTAL
};

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window wi'll be rendering to 
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The image that correspons to a keypress 
SDL_Texture* gKeyPressTextures[KEY_PRESS_TEXTURE_TOTAL];

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
    // optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);  // SDL2 Implamentation
    // optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format);  // SDL3 Implamentation
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL)
    {
      printf("Unable to optimize image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
    }

    // Get rid of old loaded surface
    // SDL_FreeSurface(loadedSurface);  // SDL2 Implamentation
    SDL_DestroySurface(loadedSurface);  // SDl3 Implamentation
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

  // Load default surface
  gKeyPressTextures[KEY_PRESS_TEXTURE_DEFAULT] = loadTexture("images/texture.png");
  if (gKeyPressTextures[KEY_PRESS_TEXTURE_DEFAULT] == NULL)
  {
    printf("Failed to load default image!\n");
    success = false;
  }

  gKeyPressTextures[KEY_PRESS_TEXTURE_UP] = loadTexture("images/up.bmp");
  if (gKeyPressTextures[KEY_PRESS_TEXTURE_UP] == NULL)
  {
    printf("Failed to load up image!\n");
    success = false;
  }

  gKeyPressTextures[KEY_PRESS_TEXTURE_DOWN] = loadTexture("images/down.bmp");
  if (gKeyPressTextures[KEY_PRESS_TEXTURE_DOWN] == NULL)
  {
    printf("Failed to load down image!\n");
    success = false;
  }
  gKeyPressTextures[KEY_PRESS_TEXTURE_LEFT] = loadTexture("images/left.bmp");
  if (gKeyPressTextures[KEY_PRESS_TEXTURE_LEFT] == NULL)
  {
    printf("Failed to load left image!\n");
    success = false;
  }
  gKeyPressTextures[KEY_PRESS_TEXTURE_RIGHT] = loadTexture("images/right.bmp");
  if (gKeyPressTextures[KEY_PRESS_TEXTURE_RIGHT] == NULL)
  {
    printf("Failed to load right image!\n");
    success = false;
  }
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
      gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_DEFAULT];
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
                gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_UP];
                break;
              case SDLK_DOWN:
                gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_DOWN];
                break;
              case SDLK_LEFT:
                gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_LEFT];
                break;
              case SDLK_RIGHT:
                gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_RIGHT];
                break;
              default:
                gTexture = gKeyPressTextures[KEY_PRESS_TEXTURE_DEFAULT];
                break;
            }
          }
          // Clear screen
          SDL_RenderClear(gRenderer);

          // Render texture to screen
          // SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);  // SDL2 Implementation
          SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);  // SDL3 Implementation

          // Update screen
          SDL_RenderPresent(gRenderer);
        }

        // Apply the image stretched
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;
        // SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);  // SDL2 Implamentation
        SDL_BlitSurfaceScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);  // SDL3 Implamentation

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
      }
    }
  }
  // Free resources and close SDL
  close();

  return 0;
}
