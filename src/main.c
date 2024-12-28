//
//  main.c
//  ProjectTowerDefense
//
//  Created by Matěj Papala on 21.11.2024.
//
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "enemyMovement.h"
#include "enemyManager.h"
#include "turretManager.h"
#include "textEffects.h"
#include <stdbool.h>

SDL_Rect turretSpots[] = {
    {400, 200, 25, 25}, 
    {200, 440, 25, 25}, 
    {570, 430, 25, 25}, 
    {650, 710, 25, 25}
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

SDL_Rect startButton = {400, 300, 200, 80}; 
SDL_Rect exitButton = {400, 400, 200, 80};


 
void renderMenu(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color white = {255, 255, 255};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
    SDL_RenderFillRect(renderer, &startButton);
    SDL_Surface* startTextSurface = TTF_RenderText_Blended(font, "Start Game", white);
    SDL_Texture* startTextTexture = SDL_CreateTextureFromSurface(renderer, startTextSurface);
    SDL_Rect startTextRect = {startButton.x + 20, startButton.y + 20, startTextSurface->w, startTextSurface->h};
    SDL_RenderCopy(renderer, startTextTexture, NULL, &startTextRect);


    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    SDL_RenderFillRect(renderer, &exitButton);
    SDL_Surface* exitTextSurface = TTF_RenderText_Blended(font, "Exit", white);
    SDL_Texture* exitTextTexture = SDL_CreateTextureFromSurface(renderer, exitTextSurface);
    SDL_Rect exitTextRect = {exitButton.x + 50, exitButton.y + 20, exitTextSurface->w, exitTextSurface->h};
    SDL_RenderCopy(renderer, exitTextTexture, NULL, &exitTextRect);

    SDL_FreeSurface(startTextSurface);
    SDL_DestroyTexture(startTextTexture);
    SDL_FreeSurface(exitTextSurface);
    SDL_DestroyTexture(exitTextTexture);

    SDL_RenderPresent(renderer);
}

void renderGameOver(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    SDL_Color red = {255, 0, 0};
    SDL_Surface* gameOverSurface = TTF_RenderText_Blended(font, "GAME OVER", red);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Rect gameOverRect = {200, 300, 600, 200};
    SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);

    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);

    SDL_RenderPresent(renderer);
}

void renderVictoryScreen(SDL_Renderer* renderer, TTF_Font* font) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    SDL_Color green = {0, 255, 0}; 
    SDL_Surface* victorySurface = TTF_RenderText_Blended(font, "YOU WON!", green);
    SDL_Texture* victoryTexture = SDL_CreateTextureFromSurface(renderer, victorySurface);
    SDL_Rect victoryRect = {200, 300, 600, 200}; 
    SDL_RenderCopy(renderer, victoryTexture, NULL, &victoryRect);

    SDL_FreeSurface(victorySurface);
    SDL_DestroyTexture(victoryTexture);

    SDL_RenderPresent(renderer); 
}



int playerMoney = 100;
int turretCost = 50;
TextEffect textEffects[10];
int numTextEffects = 0;
bool inMenu = true;
bool gameOver = false;
double gameOverStartTime = 0;
bool gameWon = false; 
double gameWonStartTime = 0; 



int main( int argc, char* args[] )
{
    double last = SDL_GetPerformanceCounter();
    int windowWidth = 1004, windowHeight = 870;
    if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        printf("Error initialazing: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("../../src/Assets/PTF-NORDIC-Rnd.ttf", 24);
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Placeholder name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* heartTexture = IMG_LoadTexture(renderer, "../../src/Assets/playerHealthHeart.png");
    SDL_Texture* backgroundImage = IMG_LoadTexture(renderer, "../../src/Assets/backgroundVersTwo.png");
    SDL_Texture* turretTexture = IMG_LoadTexture(renderer, "../../src/Assets/towerTowerDefense.png");
    int run = 1;
    int wave = 0;
    int playerHealth = 100;
    double elapsedTime = 0;
    SDL_Event event;
    //Enemy *enemyTest = NULL;
    EnemyManager* enemyManager = createEnemyManager(50);
    Turret turrets[5];
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
        if (inMenu) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    run = 0;
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x = event.button.x;
                    int y = event.button.y;

                    
                    if (SDL_PointInRect(&(SDL_Point){x, y}, &startButton)) {
                        inMenu = false;
                    }

                    if (SDL_PointInRect(&(SDL_Point){x, y}, &exitButton)) {
                        run = 0;
                    }
                }
            }
            renderMenu(renderer, font);
        } else {

            if (gameWon) {
                renderVictoryScreen(renderer, font);
                double now = SDL_GetPerformanceCounter();
                if (((now - gameWonStartTime) / SDL_GetPerformanceFrequency()) > 3.0) {
                    run = 0;
                }
                continue;
            }

            if(gameOver){
                renderGameOver(renderer, font);
                double now = SDL_GetPerformanceCounter();
                if (((now - gameOverStartTime) / SDL_GetPerformanceFrequency()) > 3.0) {
                    run = 0; 
                }
                continue;
            }

        double now = SDL_GetPerformanceCounter();
        double elapsed = deltaTime(now, last);
        last = now;
        elapsedTime += elapsed * 1000;   

        SDL_RenderClear(renderer);
        SDL_Rect backgroundRect = {0, 0, windowWidth, windowHeight};
        SDL_RenderCopy(renderer, backgroundImage, NULL, &backgroundRect);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


        SDL_Color white = {255, 255, 255};
        char hpText[20];
        snprintf(hpText, sizeof(hpText), "%d", playerHealth);


        SDL_Surface* hpSurface = TTF_RenderText_Blended(font, hpText, white);
        SDL_Texture* hpTexture = SDL_CreateTextureFromSurface(renderer, hpSurface);
        SDL_Rect hpRect = {windowWidth - 150, 10, hpSurface->w, hpSurface->h}; 
        SDL_RenderCopy(renderer, hpTexture, NULL, &hpRect);

        SDL_Rect heartRect = {windowWidth - 100, 10, 40, 40}; 
        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);

        SDL_FreeSurface(hpSurface);
        SDL_DestroyTexture(hpTexture);

        
        for (int i = 0; i < numClickableSpots; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128); 
            SDL_RenderFillRect(renderer, &turretSpots[i]);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
            SDL_RenderDrawRect(renderer, &turretSpots[i]);
        }
        //render turrets
        for (int i = 0; i < numTurrets; i++) {
            SDL_RenderCopy(renderer, turrets[i].texture, NULL, &turrets[i].renderPosition);
        }



        //spawn a update enemy
        if(wave <= 2) {
            spawnEnemies(enemyManager, renderer, pathOne, pathLengthOne, wave, elapsedTime);
        }else if(wave == 3) {
            spawnEnemies(enemyManager, renderer, pathTwo, pathLengthTwo, wave, elapsedTime);
        } else {
            spawnEnemies(enemyManager, renderer, pathThree, pathLengthThree, wave, elapsedTime);
        }
        updateEnemies(enemyManager, elapsed, &playerMoney, elapsedTime, &playerHealth);
        renderEnemies(enemyManager, renderer);


        //render money
        char moneyText[20];
        snprintf(moneyText, sizeof(moneyText),"Money: %d", playerMoney);
        SDL_Surface* moneySurface = TTF_RenderText_Blended(font, moneyText, white);
        SDL_Texture* moneyTexture = SDL_CreateTextureFromSurface(renderer, moneySurface);
        SDL_Rect moneyRect = {10, 10, moneySurface->w, moneySurface->h};
        SDL_RenderCopy(renderer, moneyTexture, NULL, &moneyRect);
        SDL_FreeSurface(moneySurface);
        SDL_DestroyTexture(moneyTexture);
        //render wave
        char waveText[20];
        sprintf(waveText, "Wave: %d", wave);
        SDL_Surface* waveSurface = TTF_RenderText_Blended(font, waveText, white);
        SDL_Texture* waveTexture = SDL_CreateTextureFromSurface(renderer, waveSurface);
        SDL_Rect waveRect = {10, 40, waveSurface->w, waveSurface->h}; // Positioned below the money
        SDL_RenderCopy(renderer, waveTexture, NULL, &waveRect);
        for (int i = 0; i < numTextEffects; i++) {
            if (elapsedTime - textEffects[i].startTime < textEffects[i].duration) {
                SDL_Surface* effectSurface = TTF_RenderText_Blended(font, textEffects[i].text, textEffects[i].color);
                SDL_Texture* effectTexture = SDL_CreateTextureFromSurface(renderer, effectSurface);
                SDL_RenderCopy(renderer, effectTexture, NULL, &textEffects[i].position);
                SDL_FreeSurface(effectSurface);
                SDL_DestroyTexture(effectTexture);
            } else {
                for (int j = i; j < numTextEffects - 1; j++) {
                    textEffects[j] = textEffects[j + 1];
                }
                numTextEffects--;
                i--;
            }
        }


        for (int i = 0; i < numTurrets; i++) {
            shootEnemies(&turrets[i], enemyManager, renderer, elapsedTime);
        }

        SDL_RenderPresent(renderer);

        // end of waves
        if (!enemyManager->waveActive && enemyManager->activeEnemies == 0 && elapsedTime > 10000) {
            wave++;     
            enemyManager->waveActive = true;
            enemyManager->nextSpawnTime = 0;
            enemyManager->enemiesSpawned = 0;
            elapsedTime = 0;
            turrets->lastShotTime = 0;
            for (int i = 0; i < numTurrets; i++) {
                turrets[i].lastShotTime = turrets[i].shootCooldown;
            }
            for (int i = 0; i < numTextEffects; i++) {
                textEffects[i].duration = 0;
            }
        }

        // event catcher
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN: {
                    int x = event.button.x;
                    int y = event.button.y;
                    bool turretUpgraded = false;
                    for (int i = 0; i < numTurrets; i++) {
                        if (SDL_PointInRect(&(SDL_Point){x, y}, &turrets[i].position)) {
                        upgradeTurret(&turrets[i], &playerMoney, renderer);
                        turretUpgraded = true;
                        break;
                    }
                    }

                    if(!turretUpgraded){
                        for (int i = 0; i < numClickableSpots; i++) {
                            if (SDL_PointInRect(&(SDL_Point){x, y}, &turretSpots[i])) {
                                if (numTurrets < 50 && playerMoney >= turretCost) {
                                    turrets[numTurrets].position = turretSpots[i];
                                    turrets[numTurrets].renderPosition.x = turretSpots[i].x - 60;
                                    turrets[numTurrets].renderPosition.y = turretSpots[i].y - 70;
                                    turrets[numTurrets].renderPosition.w = 150; 
                                    turrets[numTurrets].renderPosition.h = 150;
                                    turrets[numTurrets].range = 150; 
                                    turrets[numTurrets].damage = 40;
                                    turrets[numTurrets].shootCooldown = 500;
                                    turrets[numTurrets].lastShotTime = 0;
                                    turrets[numTurrets].upgradeLevel = 0; 

                                    SDL_Surface* tempSurface = IMG_Load("../../src/Assets/towerBasic.png");
                                    if (tempSurface) {
                                        turrets[numTurrets].texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
                                        SDL_FreeSurface(tempSurface);
                                    }

                                    numTurrets++;
                                    playerMoney -= turretCost; 
                                }

                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
            if (playerHealth <= 0 && !gameOver) {
                gameOver = true;
                gameOverStartTime = SDL_GetPerformanceCounter(); 
            }
            if (wave > 5 && !gameWon) {
                gameWon = true; 
                gameWonStartTime = SDL_GetPerformanceCounter();
            }

        }

    }
    for (int i = 0; i < numTurrets; i++) {
        if (turrets[i].texture) {
            SDL_DestroyTexture(turrets[i].texture);
        }
    }

    SDL_DestroyTexture(backgroundImage);
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyTexture(turretTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    freeEnemyManager(enemyManager);
    SDL_Quit();

    return 0;
}
