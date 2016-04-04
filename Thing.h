#ifndef THING_H
#define THING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Thing
{
    public:
        SDL_Renderer* renderer;
        SDL_Rect rect;
        SDL_Texture* texture;
        char skin;
        
        int x,y;
        Thing(SDL_Renderer* renderer, char skin,int x, int y,int val);
        void move(int direction);
        virtual ~Thing();
    protected:
    private:
};

#endif // THING_H
