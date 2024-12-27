#include "enemyManager.h"
#include <stdlib.h>
#include <stdbool.h>
#include "textEffects.h"
// manager init
EnemyManager* createEnemyManager(int maxEnemies) {
    EnemyManager* manager = (EnemyManager*)malloc(sizeof(EnemyManager));
    manager->enemies = (Enemy**)malloc(sizeof(Enemy*) * maxEnemies);
    manager->maxEnemies = maxEnemies;
    manager->activeEnemies = 0;
    manager->nextSpawnTime = 0;
    for (int i = 0; i < maxEnemies; i++) {
        manager->enemies[i] = NULL;
    }
    return manager;
}


//freeing the manager
void freeEnemyManager(EnemyManager* manager) {
    for (int i = 0; i < manager->maxEnemies; i++) {
        if (manager->enemies[i] != NULL) {
            SDL_DestroyTexture(manager->enemies[i]->image);
            free(manager->enemies[i]);
        }
    }
    free(manager->enemies);
    free(manager);
}

// spawning enemies
void spawnEnemies(EnemyManager* manager, SDL_Renderer* renderer, SDL_Point* path, int pathLength, int wave, double elapsedTime) {
    int totalEnemiesToSpawn = 5 + wave * 2;
    if (manager->waveActive && manager->activeEnemies <= totalEnemiesToSpawn && elapsedTime >= manager->nextSpawnTime) {
        // difficulty
        int enemyHP = 100 + (wave * 5);
        int enemySpeed = 2 + (wave * 0.3);
        const char* enemyImagePath = "../../src/Assets/characterEnemyGoblin.png";

        // creating new enemy
        Enemy* enemy = spawnEnemy(renderer, path, pathLength, enemyImagePath);
        if (enemy) {
            enemy->speed = enemySpeed;
            enemy->hp = enemyHP;
            manager->enemies[manager->activeEnemies++] = enemy;
        }

        // timing for waves spawn
        manager->nextSpawnTime = elapsedTime + 1000;
        
    }
    if (manager->activeEnemies == totalEnemiesToSpawn) {
        manager->waveActive = false;
        manager->waveCooldown = elapsedTime + 5000;
    }
}


void updateEnemies(EnemyManager* manager, double deltaTime, int* playerMoney, double elapsedTime) {
    for (int i = 0; i < manager->activeEnemies; i++) {
        Enemy* enemy = manager->enemies[i];
        if (enemy != NULL) {
            moveEnemy(enemy, 50 * deltaTime);

            //end of path
            if (enemy->pathIndex >= enemy->pathLength - 1) {
                SDL_DestroyTexture(enemy->image);
                free(enemy);
                manager->enemies[i] = NULL;
                continue;
            }
        }
        //out of HP
        if (enemy->hp <= 0) {
            if (numTextEffects < 10) {
                sprintf(textEffects[numTextEffects].text, "+10");
                textEffects[numTextEffects].position = (SDL_Rect){enemy->x, enemy->y - 20, 30, 20};
                textEffects[numTextEffects].color = (SDL_Color){0, 0, 0, 255}; 
                textEffects[numTextEffects].startTime = elapsedTime;
                textEffects[numTextEffects].duration = 1000; 
                numTextEffects++;
            }

            SDL_DestroyTexture(enemy->image);
            free(enemy);
            manager->enemies[i] = NULL;
            *playerMoney += 10;
            continue;
        }

    }
    

    //NULLing the enemies array
    int index = 0;
    for (int i = 0; i < manager->activeEnemies; i++) {
        if (manager->enemies[i] != NULL) {
            manager->enemies[index++] = manager->enemies[i];
        }
    }
    manager->activeEnemies = index;
}

// Rendering the enemies
void renderEnemies(EnemyManager* manager, SDL_Renderer* renderer) {
    for (int i = 0; i < manager->activeEnemies; i++) {
        if (manager->enemies[i] != NULL) {
            renderEnemy(renderer, manager->enemies[i]);
        }
    }
}
