#include "Bar.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Bar::Bar(SDL_Renderer* renderer, int x, int val)
{
    this->renderer = renderer;
    this->x = x;
    this->y = x;
    this ->val = val;

    this->rect.x = x*16; 
    this->rect.y = 1024-(val*12);

    this->texture = IMG_LoadTexture(renderer, "BACK.png");
    this->rect.h = val*64;
 
    SDL_QueryTexture(this->texture,NULL,NULL,&this->rect.w,&this->rect.h);
}

void Bar::move(int pos)
{
    this->x = pos;
    //this->val = pos+1;
    this->rect.x = x*16; 
    SDL_QueryTexture(this->texture,NULL,NULL,&this->rect.w,&this->rect.h);
}

void Bar::bling()
{
    this->texture = IMG_LoadTexture(renderer, "BACK_CHECK.png");
    SDL_QueryTexture(this->texture,NULL,NULL,&this->rect.w,&this->rect.h);
}
void Bar::revert()
{
    this->texture = IMG_LoadTexture(renderer, "BACK.png");
    SDL_QueryTexture(this->texture,NULL,NULL,&this->rect.w,&this->rect.h);
}
void Bar::ok()
{
    this->texture = IMG_LoadTexture(renderer, "BACK_DONE.png");
    SDL_QueryTexture(this->texture,NULL,NULL,&this->rect.w,&this->rect.h);
}


Bar::~Bar()
{
    //dtor
}
