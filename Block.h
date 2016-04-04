#ifndef BLOCK_H
#define BLOCK_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Block
{
    public:
        int x,y;
        SDL_Renderer* renderer;
        SDL_Rect rect_block;
        SDL_Texture* texture;
        bool isWall;
        bool isFloor;
        bool is_mino;
        bool is_exit;
        Block(SDL_Renderer* renderer,int x, int y, int texture);
        void swap();
        void change_type(int tipo);
        virtual ~Block();
    protected:
    private:
};

#endif // BLOCK_H
