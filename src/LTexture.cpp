#include "LTexture.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <cstddef>
#include <cstdio>
#include <sys/types.h>

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

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont, SDL_Renderer* gRenderer)
{
  // Gid rid of pre-existing texture
  free();

  // Render text surface
  SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
  if (textSurface == NULL)
  {
    printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
  }
  else
  {
    // Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (mTexture == NULL)
    {
      printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
      // Get image dimenstions
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }

    // Get rid of old surface
    // SDL_FreeSurface(textSurface);  // SDL2 Implementation
    SDL_DestroySurface(textSurface);
  }

  // Return surface
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

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
  // Modulate texture
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

// void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)  // SDL2 Implementation
void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_FRect* clip, double angle, SDL_FPoint* center, SDL_RendererFlip flip)
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
  // SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);  // SDL2 Implementation
  SDL_RenderTextureRotated(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);  // SDL3 Implementation
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
  // Set blending function
  SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
  // Modulate texture alpha
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

int LTexture::getWidth()
{
  return mWidth;
}

int LTexture::getHeight()
{
  return mHeight;
}
