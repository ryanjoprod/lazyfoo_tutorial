#include "LTexture.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cstddef>

LTexture::LTexture()
{
  // Initialize
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

LTexture::~LTexture()
{
  // Deallocate
  free();
}

bool LTexture::loadFromFile(std::string path, SDL_Renderer* gRenderer)
{
  // Get rid of pre-existing texture 
  free();

  // The final texture
  SDL_Texture* newTexture = NULL;

  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL)
  {
    printf("Unable to load image %s! SDL_Image Error: %s\n", path.c_str(), IMG_GetError());
  }
  else
  {
    // Color key image
    // SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));  // SDL2 Implementation
    SDL_SetSurfaceColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));  // SDL3 Implementation

    // Create texture from surface pixels 
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL)
    {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    else
    {
      // Get image dimensions
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }

    // Get rid of old loaded surface 
    // SDL_FreeSurface(loadedSurface);  // SDL2 Implementation
    SDL_DestroySurface(loadedSurface);
  }

  // Return success
  mTexture = newTexture;
  return mTexture != NULL;
}

void LTexture::free()
{
  // Free texture if it exists
  if (mTexture != NULL)
  {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

// void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip)  // SDL2 Implementation
void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_FRect* clip)
{
  // Set rendering space and render to screen
  // SDL_Rect renderQuad = {x, y, mWidth, mHeight};  // SDL2 Implementation
  SDL_FRect renderQuad = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(mWidth), static_cast<float>(mHeight)};  // SDL3 Implementation

  // Set clip rendering dimensions
  if (clip != NULL)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  // SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);  // SDL2 Implementation
  SDL_RenderTexture(gRenderer, mTexture, clip, &renderQuad);  // SDL3 Implementation
}

int LTexture::getWidth()
{
  return mWidth;
}

int LTexture::getHeight()
{
  return mHeight;
}
