#ifndef LBUTTON_LAZYFOO
#define LBUTTON_LAZYFOO

#include "LTexture.h"
#include <SDL3/SDL.h>

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
    void render(SDL_Renderer* gRenderer, LTexture gButtonSpriteSheetTexture, SDL_FRect* currentButtonDisplay);

  private:
    // Top left position
    SDL_Point mPosition;

    // Currently used global sprite
    LButtonSprite mCurrentSprite;
};

#endif
