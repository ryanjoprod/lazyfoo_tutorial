#ifndef LTEXTURE_LAZYFOO
#define LTEXTURE_LAZYFOO

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

// Texture wrapper class
class LTexture
{
  public:
    // Initialize variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

#if defined(SDL_TTF_MAJOR_VERSION)
    // Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont, SDL_Renderer* gRenderer);
#endif

    // Deallocates Texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    // void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);  // SDL2 Implementation
    void render(SDL_Renderer* gRenderer, float x, float y, SDL_FRect* clip = NULL, double angle = 0.0, SDL_FPoint* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);  // SDL3 Implementation

    // Gets image dimensions
    float getWidth();
    float getHeight();

  private:
    // The actual hardware texture
    SDL_Texture* mTexture;

    // Image dimensions
    float mWidth;
    float mHeight;
};
#endif
