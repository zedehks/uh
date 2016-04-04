#include "Block.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Block::Block(SDL_Renderer* renderer, int x, int y, int texture)
{
    this->renderer = renderer;
    this->x = x; this->y = y;

    this->isWall = false;
    this->isFloor = false;
    this->is_mino = false;
    this->is_exit = false;

    switch(texture)
    {
    case 0:
        this->texture = IMG_LoadTexture(renderer, "FLOOR.png");
        break;
    case 1:
        this->texture = IMG_LoadTexture(renderer, "WALL.png");
        break;
    }

    this->rect_block.x = x*32; this->rect_block.y = y*32;
    SDL_QueryTexture(this->texture,NULL,NULL,&rect_block.w,&rect_block.h);
}

Block::~Block()
{
    //dtor
}

void Block::swap()
{
    if(is_mino)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = IMG_LoadTexture(renderer, "TILE_ALT.png");
        this->isFloor = true;
        this->isWall = false;
        SDL_QueryTexture(this->texture,NULL,NULL,&rect_block.w,&rect_block.h);
    }
}

void Block::change_type(int tipo)
{

    SDL_DestroyTexture(this->texture);

    switch(tipo)
    {
    case 0:
        this->texture = IMG_LoadTexture(renderer, "FLOOR.png");
        this->isFloor = true;
        this->isWall = false;
        this->is_mino = false;
        break;
    case 1:
        this->texture = IMG_LoadTexture(renderer, "WALL.png");
        this->isFloor = false;
        this->isWall = true;
        this->is_mino = false;
        break;
    case 2:
        this->texture = IMG_LoadTexture(renderer, "TILE_STAR.png");
        this->isFloor = true;
        this->isWall = false;
        this->is_exit = true;
        this->is_mino = false;
        break;
    case 3:
        this->texture = IMG_LoadTexture(renderer, "FLOOR.png");
        this->isFloor = true;
        this->isWall = false;
        this->is_mino = true;
        break;

    }
    SDL_QueryTexture(this->texture,NULL,NULL,&rect_block.w,&rect_block.h);
}
