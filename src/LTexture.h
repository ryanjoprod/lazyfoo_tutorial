#ifndef LTEXTURE_LAZYFOO
#define LTEXTURE_LAZYFOO

#include <string>
#include <SDL3/SDL.h>

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

    // Deallocates Texture
    void free();

    // Renders texture at given point
    void render(int x, int y, SDL_Renderer* gRenderer);

    // Gets image dimensions
    int getWidth();
    int getHeight();

  private:
    // The actual hardware texture
    SDL_Texture* mTexture;

    // Image dimensions
    int mWidth;
    int mHeight;
};
#endif
