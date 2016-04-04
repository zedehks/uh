#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <string>
#include <random>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Block.h"
#include "Thing.h"
#include "Bar.h"

const int MAZE_SIZE = 32;
const int BARS = 64;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;

Block* blocks[MAZE_SIZE][MAZE_SIZE];

Thing* player;
Thing* mino;
std::vector<Thing*> ents;

Thing* title_card;
int current_screen;

Bar* bars[BARS];
Bar* temp[BARS];
bool render_temp = false;


bool game1_won = false;
bool show_mino_thought_process = false;
int exit_x = 0;
int exit_y = 0;



//const int MAX_FPS = 60;
//const int TICKS_PER_FRAME = 1000 / MAX_FPS;

int init_all();
void new_map();
void render();
int main_game();
void carve_maze();
int algorithm_game();
void player_move(int direction);
bool is_floor(int x, int y);
void game1_victory();
void game_loss();
void mino_find(int times);
int mino_find_aux(int x,int y, int counter);
void bubble_sort();
void merge_sort();
void real_merge_sort(int size, int min, int max);
void merge(int size, int min, int mid, int max);
void gnome_sort();
void quick_sort();
void real_quick_sort(int left,int right);
void ok();
void examine(int i);
void confirm_moves();

//main windows
int main(int argc, char *argv[])
{
    int result = init_all();
    if(result != 0)
        return result;

    current_screen = 0;
    title_card = new Thing(renderer,'u',0,132,0);
    //menu loop
    while(true)
    {      
        SDL_RenderClear(renderer);
        
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
                return 0;
            if(Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    goto m_exit;
                    break;
                case SDLK_1:
                    title_card = NULL;
                    current_screen = -2;
                    main_game();
                    current_screen = 0;
                    title_card = new Thing(renderer,'u',0,132,0);
                    break;
                case SDLK_2:
                    title_card = NULL;
                    current_screen = -1;
                    algorithm_game();
                    current_screen = 0;
                    title_card = new Thing(renderer,'u',0,132,0);
                    break;
                }
            }
        }
        //render
        render();
        SDL_RenderPresent(renderer);
    }
    //Clear everything
    m_exit:
    SDL_Quit();
    //printf("\n\n\n\n\n\n\nJUST v0.0.1 - Copyup (c)1984 SOME AMOUNT OF RIGHTS RESERVED, BITCH\n\"Fuck FLOSS, Fuck Liberty, and especially, Fuck You :^)\"\n\n");
    return 0;
}
int main_game()
{
    game1_won = false;
    //init array
    new_map();
    carve_maze();
    //Main Loop
    while(true)
    {
        if(game1_won)
            return 0;
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
                return 0;
            if(Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    return 0;
                    break;
                case SDLK_LEFT:
                    player_move(2);
                    if(player->x == exit_x && player->y == exit_y)
                    {
                        game1_victory();
                        if(game1_won)
                            return 0;
                    }
                    mino_find(0);
                    break;
                case SDLK_RIGHT:
                    player_move(0);
                    if(player->x == exit_x && player->y == exit_y)
                    {
                        game1_victory();
                        if(game1_won)
                            return 0;
                    }
                    mino_find(0);
                    break;
                case SDLK_UP:
                    player_move(1);
                    if(player->x == exit_x && player->y == exit_y)
                    {
                        game1_victory();
                        if(game1_won)
                            return 0;
                    }
                    mino_find(0);
                    break;
                case SDLK_DOWN:
                    player_move(3);
                    if(player->x == exit_x && player->y == exit_y)
                    {
                        game1_victory();
                        if(game1_won)
                            return 0;
                    }
                    mino_find(0);
                    break;
                case SDLK_DELETE:
                    break;
                case SDLK_RETURN:
                    mino_find(0);
                    break;
                case SDLK_SPACE:
                    if(show_mino_thought_process)
                    {
                        printf("paths hidden\n");
                        show_mino_thought_process = false;
                        for(int i = 0; i < MAZE_SIZE; i++)
                        {
                            for(int j = 0; j < MAZE_SIZE; j++)
                            {
                                if(blocks[i][j]->is_mino)
                                    blocks[i][j]->change_type(0);
                            }
                        }
                    }
                    else
                    {
                        printf("paths shown\n");
                       show_mino_thought_process = true; 
                       for(int i = 0; i < MAZE_SIZE; i++)
                        {
                            for(int j = 0; j < MAZE_SIZE; j++)
                            {
                                if(blocks[i][j]->is_mino)
                                    blocks[i][j]->swap();
                            }
                        }
                    }
                    break;

                }
            }
        }
        render();
        if(player->x == mino->x && player->y == mino->y)
        {
            game_loss();
        }
        if(player->x+1 == mino->x && player->y == mino->y)  
        {
            game_loss();
        }
        if(player->x-1 == mino->x && player->y == mino->y)  
        {
            game_loss();
        }
        if(player->x == mino->x && player->y+1 == mino->y)  
        {
            game_loss();
        }
        if(player->x == mino->x && player->y-1 == mino->y)  
        {
            game_loss();
        }
        //RENDERING
        
        //everything else
        render();
    }
}
void game1_victory()
{
    current_screen = 1;
    title_card = NULL;
    title_card = new Thing(renderer,'w',192,252,0);
    while(true)
    {
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_KEYDOWN)
            {        
                game1_won = true;
                return;
            }
        }
        render();
    }
}
void game_loss()
{
    current_screen = 2;
    title_card = NULL;
    title_card = new Thing(renderer,'l',192,252,0);
    while(true)
    {
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_KEYDOWN)
            {        
                game1_won = true;
                return;
            }
        }
        render();
    }
}
int algorithm_game()
{
    render_temp = false;
    SDL_RenderClear(renderer);
    for (int i = 0; i < BARS; ++i)
    {
        bars[i] = NULL;
        temp[i] = NULL;
    }
    for (int i = 0; i < BARS; ++i)
    {
        SDL_RenderClear(renderer);
        bars[i] = new Bar(renderer,i,i+1);
        render();
        SDL_Delay(30);
    }
    //Main Loop
    while(true)
    {
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
                return 0;
            if(Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    return 0;
                    break;
                case SDLK_1://bubble
                    bubble_sort();
                    break;
                case SDLK_2://merge
                    merge_sort();
                    break;
                case SDLK_3://quick
                    gnome_sort();
                    break;
                case SDLK_4:
                    quick_sort();
                    break;
                case SDLK_DELETE:
                    break;
                case SDLK_RETURN:
                    break;
                case SDLK_SPACE:
                    std::random_shuffle(std::begin(bars),std::end(bars));
                    confirm_moves();

                }
            }
        }
        render();
    }
}

//game tools
int init_all()
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("MINOTAUR_MAZE", 500, 20, 704/*WIDTH*/, 704/*HEIGHT*/,  SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL_SetWindowIcon(window, IMG_Load("BJ_STAND.png"));


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }
    //SDL_RenderSetLogicalSize(renderer,640,640);
    //double rScale = (4/3);
    SDL_RenderSetScale(renderer,0.6875,0.6875);
    //selected_block = 0;
    //printSelection(selected_block);
    //bucket = false;
    //SDL_ShowCursor(0);
    //srand(time(0));
    srandomdev();

    return 0;
}
void new_map()
{
    exit_x = 0;
    exit_y = 0;
    ents.clear();
    for(int i = 0;i<MAZE_SIZE;i++)
    {
        for(int j = 0;j<MAZE_SIZE;j++)
        {
            blocks[i][j] = NULL;
            blocks[i][j] = new Block(renderer,i,j,1);
        }
    }
}
void carve_maze()
{
    int x = random()%MAZE_SIZE; 
    int y = random()%MAZE_SIZE;
    if(x==0)
        x+=1;
    if(x==31)
        x-=1;
    if(y==0)
        y+=1;
    if(y==31)
        y-=1;
    blocks[x][y]->change_type(0);
    if(x < MAZE_SIZE-1)
        blocks[x+1][y]->change_type(1);
    if(x > 0)
        blocks[x-1][y]->change_type(1);
    if(y < MAZE_SIZE-1)
        blocks[x][y+1]->change_type(1);
    if(y > 0)
        blocks[x][y-1]->change_type(1);
    exit_x = x;
    exit_y = y;

    render();
    bool isDone = false;
    int is_it_done = 0;
    while(!isDone)
    {
        x = random()%MAZE_SIZE; 
        y = random()%MAZE_SIZE;
        if(blocks[x][y]->isWall)
        {
            bool dig_up=false,dig_down=false,dig_left=false,dig_right=false;
            int dig_count = 0;
            int diagonals = 0;
            if(x < MAZE_SIZE-1)
            {
                if(!blocks[x+1][y]->isFloor)
                {
                    dig_count++;
                    dig_right=true; 
                }
                if(y < MAZE_SIZE-1)
                {
                    if(blocks[x+1][y+1]->isFloor)
                    {
                        diagonals++;
                    }
                }                          
            }            
            if(x > 0)
            {
                if(!blocks[x-1][y]->isFloor)
                {
                    dig_count++;
                    dig_left=true;
                }
                if(y > 0)
                {
                    if(blocks[x-1][y-1]->isFloor)
                    {
                        diagonals++;
                    }
                }  
            }  
            if(y < MAZE_SIZE-1)
            {
                if(!blocks[x][y+1]->isFloor)
                {
                    dig_count++;
                    dig_down=true;
                }
                if(x > 0)
                {
                    if(blocks[x-1][y+1]->isFloor)
                    {
                        diagonals++;
                    }
                }  
            }  
            if(y > 0)
            {
                if(!blocks[x][y-1]->isFloor)
                {
                    dig_count++;
                    dig_up=true;
                }
                if(x < MAZE_SIZE-1)
                {
                    if(blocks[x+1][y-1]->isFloor)
                    {
                        diagonals++;
                    }                     
                }
            }  
            if(dig_count >= 3 && diagonals < 2)
            {
                blocks[x][y]->change_type(0);

                if(x < MAZE_SIZE-1 && !blocks[x+1][y]->isFloor)
                    blocks[x+1][y]->change_type(1);
                if(x > 0 && !blocks[x-1][y]->isFloor)
                    blocks[x-1][y]->change_type(1);
                if(y < MAZE_SIZE-1 && !blocks[x][y+1]->isFloor)
                    blocks[x][y+1]->change_type(1);
                if(y > 0 && !blocks[x][y-1]->isFloor)
                    blocks[x][y-1]->change_type(1);
            }
            else
            {
                is_it_done++;
            }
                

            render();
            //SDL_Delay(120);
        }

        
        if(is_it_done >= MAZE_SIZE*MAZE_SIZE*5)
            isDone = true; 
    }

    for(int i = 0;i<MAZE_SIZE;i++)
    {
        for(int j = 0;j<MAZE_SIZE;j++)
        {
            if(!blocks[i][j]->isWall && !blocks[i][j]->isFloor)
                blocks[i][j]->change_type(1);
            if(blocks[i][j]->isFloor)
                blocks[i][j]->change_type(0);
            if(blocks[i][j]->isWall)
                blocks[i][j]->change_type(1);
        }
    }

    //todo:needs more checks to work to my taste
    /*int passages = MAZE_SIZE;
    for (int i = 0; i < passages; ++i)
    {
        x = random()%MAZE_SIZE;
        y = random()%MAZE_SIZE;
        if(x==0)
            x+=1;
        if(x==31)
            x-=1;
        if(y==0)
            y+=1;
        if(y==31)
            y-=1;

        if(blocks[x][y]->isWall)
        {
            int i = 0;
            if(blocks[x+1][y]->isFloor)
                i++;
            if(blocks[x-1][y]->isFloor)
                i++;
            if(blocks[x][y+1]->isFloor)
                i++;
            if(blocks[x+1][y]->isFloor)
                i++;

            if(i==2)
            {
                blocks[x][y]->change_type(3);
                blocks[x][y]->swap();
                passages--;
            }
            else
            {
                passages++;
            }
        }
        else
        {
            passages++;
        }
    }*/

    //blocks[exit_x][exit_y]->change_type(2);

    
    while(true)
    {
        x = random()%MAZE_SIZE;
        y = random()%MAZE_SIZE;
        if(blocks[x][y]->isFloor)
        {
            int floor_count = 0;
            if(x < 31)
            {
                if(blocks[x+1][y]->isFloor)
                    floor_count++;
            }
            if(x > 0)
            {
                if(blocks[x-1][y]->isFloor)
                    floor_count++;
            }
            if(y < 31)
            {
                if(blocks[x][y+1]->isFloor)
                    floor_count++;
            }
            if(y > 0)
            {
                if(blocks[x][y-1]->isFloor)
                    floor_count++;
            }

            if(floor_count == 1)
            {
                blocks[x][y]->change_type(2);
                exit_x = x;
                exit_y = y;
                break; 
            }           
        }
    }  

    while(true)
    {
        x = random()%MAZE_SIZE;
        y = random()%MAZE_SIZE;

        if(x > 16 && exit_x > 16)
            x-=15;
        if(x < 16 && exit_x < 16)
            x+=15;
        if(y > 16 && exit_y > 16)
            y-=15;
        if(y < 16 && exit_y < 16)
            y+=15;

        if(blocks[x][y]->isFloor && !blocks[x][y]->isWall)
        {
            mino = new Thing(renderer,'m',x,y,0);
            ents.push_back(mino);
            break;    
        }
    }
    while(true)
    {
        x = random()%MAZE_SIZE;
        y = random()%MAZE_SIZE;

        if(x > 16 && exit_x > 16)
            x-=12;
        else if(x < 16 && exit_x < 16)
            x+=10;
        if(y > 16 && exit_y > 16)
            y-=12;
        else if(y < 16 && exit_y < 16)
            y+=12;

        //cant be too close to the mino, huh?
        if(x > 16 && (mino->x - x)<=5)
            x-=5;
        else if(x < 16 && (x - mino->x)<=5)
            x+=5;
        if(y > 16 && (mino->y - y)<=5)
            y-=5;
        else if(y < 16 && (y - mino->y)<=5)
            y+=5;

        if(blocks[x][y]->isFloor && !blocks[x][y]->isWall)
        {

            player = new Thing(renderer,'t',x,y,0);
            ents.push_back(player);
            break;    
        }
    }
    render();
    //SDL_Delay(120);
}
void player_move(int direction)
{
    switch(direction)
    {
        case 0:
            if(is_floor(player->x+1,player->y) )
                player->move(0);
            break;
        case 1:
            if(is_floor(player->x,player->y-1) )
                player->move(1);
            break;
        case 2:
            if(is_floor(player->x-1,player->y) )
                player->move(2);
            break;
        case 3:
            if(is_floor(player->x,player->y+1) )
                player->move(3);
            break;
    }
}
void mino_find(int times)
{
    for(int i = 0; i < MAZE_SIZE; i++)
    {
        for(int j = 0; j < MAZE_SIZE; j++)
        {
            if(blocks[i][j]->is_mino)
                blocks[i][j]->change_type(0);
        }
    }

    int x = mino->x;
    int y = mino->y;
    blocks[x][y]->change_type(3);
    if(show_mino_thought_process)
        blocks[x][y]->swap();
    render();

    if(is_floor(x+1,y) )
    {
        if(mino_find_aux(x+1,y,0) == -1)
        {
            mino->move(0);
            return;
        }
    }
    if(is_floor(x,y-1) )
    {
        if(mino_find_aux(x,y-1,0)==-1)
        {
            mino->move(1);
            return;
        }

    }
    if(is_floor(x-1,y) )
    {
        if(mino_find_aux(x-1,y,0)==-1)
        {
            mino->move(2);
            return;
        }
    }
    if(is_floor(x,y+1) )
    {
        if(mino_find_aux(x,y+1,0)==-1)
        {
            mino->move(3);
            return;
        }
    }

    int x_dis = player->x - mino->x;
    int y_dis = player->y - mino->y;

    if(x_dis <0)
        x_dis*=-1;
    if(y_dis <0)
        y_dis*=-1;

    if(x_dis >4 || y_dis > 4 )
    {   
        if(times < 1)
            mino_find(++times);
    }

}
int mino_find_aux(int x,int y, int counter)
{
    blocks[x][y]->change_type(3); 
    if(show_mino_thought_process)
        blocks[x][y]->swap();
    render();
    //SDL_Delay(60);
    counter++;
    if(x == player->x && y == player->y)
        return -1;

    if(is_floor(x+1,y))
    {
        if(!blocks[x+1][y]->is_mino && !blocks[x+1][y]->isWall && !blocks[x+1][y]->is_exit)
        {
            counter = mino_find_aux(x+1,y,counter); 
            if(counter == -1)
                return -1;
        }        
    }
    if(is_floor(x,y-1) )
    {
        if(!blocks[x][y-1]->is_mino&& !blocks[x][y-1]->isWall && !blocks[x][y-1]->is_exit)
        {
            counter = mino_find_aux(x,y-1,counter); 
            if(counter == -1)
                return -1;
        }        
    }
    if(is_floor(x-1,y))
    {
        if(!blocks[x-1][y]->is_mino && !blocks[x-1][y]->isWall && !blocks[x-1][y]->is_exit)
        {
           counter = mino_find_aux(x-1,y,counter); 
           if(counter == -1)
                return -1;
        }        
    }
    if(is_floor(x,y+1) )
    {
        if(!blocks[x][y+1]->is_mino&& !blocks[x][y+1]->isWall && !blocks[x][y+1]->is_exit)
        {
           counter = mino_find_aux(x,y+1,counter); 
           if(counter == -1)
                return -1;
        }        
    }

    return counter;
}
bool is_floor(int x, int y)
{
    if(x>=MAZE_SIZE||x<=0||y>=MAZE_SIZE||y<=0)
        return false;
    if(blocks[x][y]->isFloor && !blocks[x][y]->isWall)
        return true;
    return false;
}
void bubble_sort()
{
    Bar* temp = NULL;
    render_temp = false;

    bool swap = true;
    while(swap)
    {
        swap = false;
        for (int i = 0; i < BARS; ++i)
        {
            bars[i]->bling();
            render();
            SDL_RenderClear(renderer);
            if(i<BARS-1)
            {
                if(bars[i]->val > bars[i+1]->val)
                {
                    swap = true;
                    temp = bars[i+1];
                    bars[i+1] = bars[i];
                    bars[i] = temp;

                    bars[i]->move(i);
                    bars[i+1]->move(i+1);
                    temp = NULL;
                }
            }
            //SDL_Delay(16);
            render();
            bars[i]->revert();
            render();
        }
        
    }
    ok();
}
void merge_sort()
{
    Bar* temp = NULL;
    render_temp = false;
    real_merge_sort(BARS,0,BARS-1);
    confirm_moves();
    ok();
}
void real_merge_sort(int size, int min, int max)
{
    if(min < max)
    {
        int mid  = (min+max)/2;
        real_merge_sort(size,min,mid);
        real_merge_sort(size,mid+1,max);
        merge(size,min,mid,max);
    }
}
void merge(int size, int min, int mid, int max)
{
    //temp[size];
    for(int i = min;i<=max;++i)
    {
        temp[i] = NULL;
        temp[i] = bars[i]; 
        confirm_moves();
        examine(i);     
    }
    int a = min;
    int b = mid+1;
    int c = min;

    while(a<=mid && b <=max) 
    {
        if(temp[a]->val<= temp[b]->val)
        {
            bars[c] = temp[a];
            a++;
            confirm_moves();
            examine(c);
        }
        else
        {
            bars[c] = temp[b];
            b++;
            confirm_moves();
             examine(c);
        }
        ++c;
    }
    while(a<=mid)
    {
        bars[c++] = temp[a++];
        confirm_moves();
         examine(c);
    }
}

void quick_sort()
{
    real_quick_sort(0,BARS-1);
    confirm_moves();
    ok();
}

void real_quick_sort(int left,int right)
{
    int a = left;
    int b = right;
    Bar* r;
    Bar* pivot = bars[(left+right)/2];
    pivot->ok();
    render();

    while(a<=b)
    {
        printf("THINKING...\n");
        while(bars[a]->val < pivot->val)
        {
            a++;
        } 
        while(bars[b]->val > pivot->val)
        {
            b--;
        }
        if(a<=b)
        {
            r = bars[a];
            bars[a] = bars[b];
            bars[b] = r;
            confirm_moves();
            examine(a); 
            examine(b); 
            a++;
            b--;
        }
    }
    if(left < b)
    {
        pivot->revert();
        render();
        real_quick_sort(left,b);
    }
    if(a < right)
    {
        pivot->revert();
        render();
        real_quick_sort(a,right);
    }
}

void gnome_sort()
{
    bool teleport = false;
    int portal = 0;
    int pos = 0;
    while(pos<BARS)
    {
        examine(pos);
        //examine(pos-1);
        if(pos== 0 || bars[pos-1]->val <= bars[pos]->val)
        {   if(teleport)
            {
                teleport = false;
                pos=portal;
            }
            else
                pos +=1;
        }
        else
        {
            if(!teleport)
            {
                teleport = true;
                portal = pos;
            }
            Bar* temp;
            temp = bars[pos];
            bars[pos] = bars[pos-1];
            bars[pos-1] = temp;
            pos-=1;
            teleport = true;
            portal = pos;
            confirm_moves();
            examine(pos);
        }
    }
    ok();
}
void confirm_moves()
{
    for (int i = 0; i < BARS; ++i)
    {
        bars[i]->move(i);
    }
}


void examine(int i)
{
    bars[i]->bling();
    render();
    SDL_RenderClear(renderer);
    SDL_Delay(16);
    bars[i]->revert();
    render();
    SDL_RenderClear(renderer);
}
void ok()
{
    for (int i = 0; i < BARS; ++i)
    {
        bars[i]->ok();
        render();
        SDL_RenderClear(renderer);
        SDL_Delay(20);
    }
    SDL_Delay(600);
    for (int i = 0; i < BARS; ++i)
    {
        bars[i]->revert();
    }
    render();
}

//rendering
void render()
{
    if(current_screen == 0)
    {
        SDL_RenderCopy(renderer,title_card->texture, NULL, &title_card->rect);
        return;
    }
    if(current_screen == -1)
    {
        for(int j = 0; j < BARS; j++)
        {
            //if(!render_temp)
            {
                if(bars[j]!=NULL)
                {
                    SDL_RenderCopy(renderer,bars[j]->texture, NULL, &bars[j]->rect);
                }
            }
            //else
            {
                if(temp[j]!=NULL)
                {
                    SDL_RenderCopy(renderer,temp[j]->texture, NULL, &temp[j]->rect);
                }  
            }
            
                
        }
        SDL_RenderPresent(renderer);
        return;
    }

    for(int i = 0; i < MAZE_SIZE; i++)
    {
        for(int j = 0; j < MAZE_SIZE; j++)
        {
            SDL_RenderCopy(renderer,blocks[i][j]->texture, NULL, &blocks[i][j]->rect_block);
            
        }
    }

    std::vector<Thing*>::iterator it;
    for(it=ents.begin() ; it < ents.end(); it++) 
    {
        if(!ents.empty())
        {
             SDL_RenderCopy(renderer,(*it)->texture, NULL, &(*it)->rect);
        }
    }
    if(current_screen >= 1)
        SDL_RenderCopy(renderer,title_card->texture, NULL, &title_card->rect);

    SDL_RenderPresent(renderer);
}



