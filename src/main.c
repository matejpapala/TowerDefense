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
#include "enemyManager.h"
#include "turretManager.h"
#include <stdbool.h>

SDL_Rect turretSpots[] = {
    {200, 200, 50, 50}, 
    {400, 300, 50, 50}, 
    {600, 500, 50, 50}, 
};
int numClickableSpots = sizeof(turretSpots) / sizeof(turretSpots[0]);

SDL_Point pathOne[] = {
    {360, -500},   // Start
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
    {770, 950}  // Konec 
};


int pathLengthOne = sizeof(pathOne) / sizeof(pathOne[0]);//length of the path

SDL_Point pathTwo[] = {
    {360, -500},   // Start 
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
    {770, 950}  // Konec 
};

int pathLengthTwo = sizeof(pathTwo) / sizeof(pathTwo[0]);

SDL_Point pathThree[] = {
    {360, -500},   // Start
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
    {770, 950}  // Konec
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
    int wave = 1;
    double elapsedTime = 0;
    SDL_Event event;
    //Enemy *enemyTest = NULL;
    EnemyManager* enemyManager = createEnemyManager(50);
    SDL_Rect turrets[5];
    int numTurrets = 0;
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
        double now = SDL_GetPerformanceCounter();
        double elapsed = deltaTime(now, last);
        last = now;
        elapsedTime += elapsed * 1000;   

        SDL_RenderClear(renderer);
        SDL_Texture* backgroundImage = IMG_LoadTexture(renderer, "../../src/Assets/backgroundVersTwo.png");
        SDL_Rect backgroundRect = {0, 0, windowWidth, windowHeight};
        SDL_RenderCopy(renderer, backgroundImage, NULL, &backgroundRect);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        for (int i = 0; i < numClickableSpots; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128); 
            SDL_RenderFillRect(renderer, &turretSpots[i]);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
            SDL_RenderDrawRect(renderer, &turretSpots[i]);
        }

        for (int i = 0; i < numTurrets; i++) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
            SDL_RenderFillRect(renderer, &turrets[i]);
        }



        spawnEnemies(enemyManager, renderer, pathOne, pathLengthOne, wave, elapsedTime);
        updateEnemies(enemyManager, elapsed);
        renderEnemies(enemyManager, renderer);

        for (int i = 0; i < numTurrets; i++) {
            Turret turret = {
                .position = turrets[i],
                .range = 200,             // Example range
                .damage = 5,             // Example damage
                .shootCooldown = 1000,    // 1 second cooldown
                .lastShotTime = 0,        // Initialize last shot time
            };

            shootEnemies(&turret, enemyManager, renderer, elapsedTime);
        }


        SDL_RenderPresent(renderer);

        // end of waves
        if (!enemyManager->waveActive && enemyManager->activeEnemies == 0 && elapsedTime > 10000) {
            wave++;     
            enemyManager->waveActive = true;
            enemyManager->nextSpawnTime = 0;
            elapsedTime = 0; 
        }

        // event catcher
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN: {
                    // Enclose this block with curly braces
                    int x = event.button.x;
                    int y = event.button.y;
                    for (int i = 0; i < numClickableSpots; i++) {
                        if (SDL_PointInRect(&(SDL_Point){x, y}, &turretSpots[i])) {
                            if (numTurrets < 5) {
                                turrets[numTurrets++] = turretSpots[i];
                                printf("Turret placed at (%d, %d)\n", turretSpots[i].x, turretSpots[i].y);
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    freeEnemyManager(enemyManager);
    SDL_Quit();

    return 0;
}
