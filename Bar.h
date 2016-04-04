#ifndef BAR_H
#define BAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Bar
{
    public:
        SDL_Renderer* renderer;
        SDL_Rect rect;
        SDL_Texture* texture;
        int val;
        
        int x,y;
        Bar(SDL_Renderer* renderer,int x,int val);
        void move(int pos);
        void bling();
        void revert();
        void ok();
        virtual ~Bar();
    protected:
    private:
};

#endif // BAR_H
