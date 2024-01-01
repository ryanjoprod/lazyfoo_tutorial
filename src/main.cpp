// Using SDL and standard IO 
#include <string>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Button constants
const int BUTTON_WIDTH = 320;
const int BUTTON_HEIGHT = 240;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
  BUTTON_SPRITE_MOUSE_OUT = 0,
  BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
  BUTTON_SPRITE_MOUSE_DOWN = 2,
  BUTTON_SPRITE_MOUSE_UP = 3,
  BUTTON_SPRITE_TOTAL = 4
};

// Texture wrapper class
class LTexture
{
  public:
    // Initialize variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)
    // Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont);
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
    // void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);  // SDL2 Implementation
    void render(float x, float y, SDL_FRect* clip = NULL, double angle = 0.0, SDL_FPoint* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);  // SDL3 Implementation

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

// The mouse button
class LButton 
{
  public:
    // Initializes internal variables
    LButton();

    // Set top left position
    void setPosition(float x, float y);

    // Handles mouse event
    void handleEvent(SDL_Event* e);

    // Show button sprite
    void render();

  private:
    // Top left position
    SDL_Point mPosition;

    // Button texture
    LTexture mButtonTexture;

    // Currently used global sprite
    LButtonSprite mCurrentSprite;
};

// Starts up SDl and creates window
bool init();

// Load media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window wi'll be rendering to 
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer= NULL;

// Globally used font 
TTF_Font* gFont = NULL;

// The buttons
LButton gButtons[TOTAL_BUTTONS];

// Rendered texture
LTexture gButtonSpriteSheetTexture;
SDL_FRect gSpriteClips[BUTTON_SPRITE_TOTAL];

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

bool LTexture::loadFromFile(std::string path)
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

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont)
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
#endif

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
void LTexture::render(float x, float y, SDL_FRect* clip, double angle, SDL_FPoint* center, SDL_RendererFlip flip)
{
  // Set rendering space and render to screen
  // SDL_Rect renderQuad = {x, y, mWidth, mHeight};  // SDL2 Implementation
  SDL_FRect renderQuad = {x, y, mWidth, mHeight};  // SDL3 Implementation

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

float LTexture::getWidth()
{
  return mWidth;
}

float LTexture::getHeight()
{
  return mHeight;
}

LButton::LButton()
{
  mPosition.x = 0;
  mPosition.y = 0;

  mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(float x, float y)
{
  mPosition.x = x;
  mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
  // If mouse event happened
  // if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN ||  e->type == SDL_MOUSEBUTTONUP)  // SDL2 Implementation
  if (e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type == SDL_EVENT_MOUSE_BUTTON_UP)  // SDL3 Implementation
  {
    // Get mouse position 
    float x, y;
    SDL_GetMouseState(&x, &y);

    // Check if mouse is in button 
    bool inside = true;

    if (x < mPosition.x)  // Mouse is left of button 
    {
      inside = false;
    }
    else if (x > mPosition.x + BUTTON_WIDTH)  // Mouse is right of button
    {
      inside = false;
    }
    else if (y < mPosition.y)  // Mouse is above the button
    {
      inside = false;
    }
    else if (y > mPosition.y + BUTTON_HEIGHT)  // Mouse is below the button 
    {
      inside = false;
    }

    if (!inside)  // Mouse is outside button
    {
      mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    }
    else  // Mouse is inside button
    {
      // Set mouse over sprite 
      switch (e->type)
      {
        // case SDL_MOUSEMOTION:  // SDL2 Implementation
        case SDL_EVENT_MOUSE_MOTION:
          mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
          break;
        // case SDL_MOUSEBUTTONDOWN:  // SDL2 Implementation
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
          break;
        // case SDL_MOUSEBUTTONUP:  // SDL2 Implementation
        case SDL_EVENT_MOUSE_BUTTON_UP:
          mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
          break;
      }
    }
  }
}

void LButton::render()
{
  // Show current button sprite
  gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}

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
      // gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  // SDL2 Implementation
      gRenderer = SDL_CreateRenderer(gWindow, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  // SDL3 Implementation
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

        // Initialize SDL_ttf
        if (TTF_Init() == -1)
        {
          printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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
  if (!gButtonSpriteSheetTexture.loadFromFile("assets/images/buttons.png"))
  {
    printf("Failed to load button textures!\n");
    success = false;
  }
  else
  {
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = BUTTON_WIDTH;
    gSpriteClips[0].h = BUTTON_HEIGHT;

    gSpriteClips[1].x = BUTTON_WIDTH;
    gSpriteClips[1].y = 0;
    gSpriteClips[1].w = BUTTON_WIDTH;
    gSpriteClips[1].h = BUTTON_HEIGHT;

    gSpriteClips[2].x = 2 * BUTTON_WIDTH;;
    gSpriteClips[2].y = 0;
    gSpriteClips[2].w = BUTTON_WIDTH;
    gSpriteClips[2].h = BUTTON_HEIGHT;

    gSpriteClips[3].x = 3 * BUTTON_WIDTH;
    gSpriteClips[3].y = 0;
    gSpriteClips[3].w = BUTTON_WIDTH;
    gSpriteClips[3].h = BUTTON_HEIGHT;

    // Set buttons in corners
    gButtons[0].setPosition(0, 0);
    gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
    gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
    gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
  }

  // Nothing to load
  return success;
}

// Frees media and shuts down SDL
void close()
{
  // Free loaded image
  gButtonSpriteSheetTexture.free();

  // Destroy window
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;

  // Quit SDL subsystems
  TTF_Quit();
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

      // Angle of rotation
      double degrees = 0;

      // Flip type
      SDL_RendererFlip flipType = SDL_FLIP_NONE;
    
      // Set default current surface
      while(!quit)
      {
        while(SDL_PollEvent(&e) != 0)
        {
          if (e.type == SDL_EVENT_QUIT)
          {
            quit = true;
          }

          // Handle button events
          for (int i = 0; i < TOTAL_BUTTONS; ++i)
          {
            gButtons[i].handleEvent(&e);
          }
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // Render buttons
        for (int i = 0; i < TOTAL_BUTTONS; ++i)
        {
          SDL_FRect* currentButtonDisplay = &gSpriteClips[i];
          gButtons[i].render();
        }

        // Update screen
        SDL_RenderPresent(gRenderer);
      }
    }
  }
  // Free resources and close SDL
  close();

  return 0;
}
