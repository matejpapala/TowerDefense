//
//  main.c
//  ProjectTowerDefense
//
//  Created by Matěj Papala on 21.11.2024.
//
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "enemyMovement.h"
#include <stdbool.h>

SDL_Point pathOne[] = {
    {50, 50}, //start point
    {300, 50},
    {300, 500},
    {800, 500},
    {800, 800} //end point
};

int pathLengthOne = sizeof(pathOne) / sizeof(pathOne[0]);//length of the path

SDL_Point pathTwo[] = {
    {50, 50},
    {150, 50},
    {150, 150},
    {300, 150},
    {300, 300}
};

int pathLengthTwo = sizeof(pathTwo) / sizeof(pathTwo[0]);

SDL_Point pathThree[] = {
    {50, 50},
    {150, 50},
    {150, 150},
    {300, 150},
    {300, 300}
};

int pathLengthThree = sizeof(pathThree) / sizeof(pathThree[0]);

SDL_Point pathFour[] = {
    {50, 50},
    {150, 50},
    {150, 150},
    {300, 150},
    {300, 300}
};

int pathLengthFour = sizeof(pathFour) / sizeof(pathFour[0]);

double deltaTime(double now, double last){
    return ((now - last) / (double)SDL_GetPerformanceFrequency());
}

char keyPressed(SDL_KeyboardEvent key){
    if(key.keysym.scancode == SDL_SCANCODE_L) {
        return 'l';
    }
    return 0;
}
 
int main( int argc, char* args[] )
{
    
    int windowWidth = 1004, windowHeight = 870;
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        printf("Error initialazing: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Placeholder name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    int run = 1;
    SDL_Event event;
    //** promena pro deltaTime
    double last = SDL_GetPerformanceCounter();
    //**
    //
    //
    //
    //**Game loop start**
    //
    //
    //
    while(run) {
        double now = SDL_GetPerformanceCounter();//druha promena pro deltaTime
        SDL_RenderClear(renderer);
        SDL_Texture* backgroundImage = IMG_LoadTexture(renderer, "../../src/Assets/backgroundVersTwo.png");//nacitani background obrazku
        //rect for background image
        SDL_Rect backgroundRect = {
            .x = 0,
            .y = 0,
            .w = windowWidth,
            .h = windowHeight
        };
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        SDL_RenderCopy(renderer, backgroundImage, NULL, &backgroundRect);
        //event catcher
        while(SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    run = 0;
                    break;
                    
                case SDL_KEYDOWN:
                    //keyDown = keyPressed(event.key);
                    break;
                    
                default:
                    break;
            }
        }
        
        //spawn enemy
        bool spawned = true;
        Enemy *enemyTest = NULL;
        if(spawned){
            enemyTest = spawnEnemy(renderer, pathOne, pathLengthOne, "../../src/Assets/characterMage.png");
            spawned = false;
        }
        moveEnemy(enemyTest, deltaTime(now, last));
        renderEnemy(renderer, enemyTest);
        last = now;
        //-----------------render------------
        SDL_RenderPresent(renderer);
        //---------------------------------
    }
    SDL_Quit();

    return 0;
}
