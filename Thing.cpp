#include "Thing.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Thing::Thing(SDL_Renderer* renderer, char skin,int x, int y, int val)
{
    this->renderer = renderer;
    this->x = x;
    this->y = y;
    this ->skin = skin;

    this->rect.x = x*32; this->rect.y = y*32;
    switch(this->skin)
    {
    	case 't':
    		this->texture = IMG_LoadTexture(renderer, "THESEUS.png");
        	break;
        case 'm':
    		this->texture = IMG_LoadTexture(renderer, "MINO.png");
        	break;
        case 'b':
            this->texture = IMG_LoadTexture(renderer, "BACK.png");
            this->rect.x = x*32; this->rect.y = (y*32 + (32*val));
            break;
        //u win! sign;
        case 'w':
            this->texture = IMG_LoadTexture(renderer, "WIN.png");
            this->rect.x = x; this->rect.y = y;
            break;
        //u luse! sign;
        case 'l':
            this->texture = IMG_LoadTexture(renderer, "LOSE.png");
            this->rect.x = x; this->rect.y = y;
            break;
        //MENU sign;
        case 'u':
            this->texture = IMG_LoadTexture(renderer, "MENU.png");
            this->rect.x = x; this->rect.y = y;
            break;
    }

    
    SDL_QueryTexture(this->texture,NULL,NULL,&this->rect.w,&this->rect.h);
}

void Thing::move(int direction)
{
    switch(direction)
    {
        case 0:
            this->x+=1;
            break;
        case 1:
            this->y-=1;
            break;
        case 2:
            this->x-=1;
            break;
        case 3:
            this->y+=1;
            break;
    }
    this->rect.x = x*32; this->rect.y = y*32;
    SDL_QueryTexture(this->texture,NULL,NULL,&this->rect.w,&this->rect.h);
}

Thing::~Thing()
{
    //dtor
}
