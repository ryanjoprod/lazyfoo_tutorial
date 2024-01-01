#include "LButton.h"
#include "LTexture.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>

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

void LButton::render(SDL_Renderer* gRenderer, LTexture gButtonSpriteSheetTexture, SDL_FRect* currentButtonDisplay)
{
  // Show current button sprite
  gButtonSpriteSheetTexture.render(gRenderer, mPosition.x, mPosition.y, currentButtonDisplay);
}
