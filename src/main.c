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
    {360, -500},   // Start - vstup z vrchu obrazovky
    {360, -100}, 
    {360, 120}, 
    {520, 120}, 
    {520, 520}, 
    {350, 520}, 
    {350, 650}, 
    {530, 650},
    {530, 690},
    {550, 690},
    {580, 690},
    {580, 770},
    {770, 770},
    {770, 950}  // Konec - výstup z obrazovky
};


int pathLengthOne = sizeof(pathOne) / sizeof(pathOne[0]);//length of the path

SDL_Point pathTwo[] = {
    {360, -500},   // Start - vstup z vrchu obrazovky
    {360, -100}, 
    {360, 120}, 
    {520, 120},
    {520, 280},
    {360, 280},
    {360, 350},
    {150, 350},
    {150, 520},
    {350, 520},
    {350, 650}, 
    {530, 650},
    {530, 690},
    {550, 690},
    {580, 690},
    {580, 770},
    {770, 770},
    {770, 950}  // Konec - výstup z obrazovky
};

int pathLengthTwo = sizeof(pathTwo) / sizeof(pathTwo[0]);

SDL_Point pathThree[] = {
    {360, -500},   // Start - vstup z vrchu obrazovky
    {360, -100}, 
    {360, 120}, 
    {520, 120},
    {520, 280},
    {360, 280},
    {360, 350},
    {150, 350},
    {150, 520},
    {685, 520},
    {685, 570},
    {640, 620},
    {640, 690},
    {580, 690},
    {580, 770},
    {770, 770},
    {770, 950}  // Konec - výstup z obrazovky
};

int pathLengthThree = sizeof(pathThree) / sizeof(pathThree[0]);


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
    double last = SDL_GetPerformanceCounter();
    int windowWidth = 1004, windowHeight = 870;
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        printf("Error initialazing: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Placeholder name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    int run = 1;
    SDL_Event event;
    Enemy *enemyTest = NULL;
    //** promena pro deltaTime
    //**
    //
    //
    //
    //**Game loop start**
    //
    //
    //
    while(run) {
        double now = SDL_GetPerformanceCounter(); // Aktuální čas
        double elapsed = deltaTime(now, last);    // Výpočet deltaTime
        last = now;  
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
                default:
                    break;
            }
        }
        
        //spawn enemy
        static bool spawned = true;
        if(spawned){
            enemyTest = spawnEnemy(renderer, pathThree, pathLengthThree, "../../src/Assets/characterEnemyGoblin.png");
            spawned = false;
            renderEnemy(renderer, enemyTest);
        }
        moveEnemy(enemyTest, 50 * elapsed);
        renderEnemy(renderer, enemyTest);
        //-----------------render------------
        SDL_RenderPresent(renderer);
        //---------------------------------

    }
    SDL_Quit();

    return 0;
}
