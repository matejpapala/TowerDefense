#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <SDL2/SDL.h>
#include "enemyMovement.h"

typedef struct {
    Enemy** enemies; 
    int maxEnemies; 
    int activeEnemies; 
    int nextSpawnTime; 
    int enemiesSpawned;
    double waveCooldown;
    bool waveActive;
} EnemyManager;

EnemyManager* createEnemyManager(int maxEnemies);

void freeEnemyManager(EnemyManager* manager);


void spawnEnemies(EnemyManager* manager, SDL_Renderer* renderer, SDL_Point* path, int pathLength, int wave, double elapsedTime);

void updateEnemies(EnemyManager* manager, double deltaTime, int* playerMoney, double elapsedTime, int* playerHealth);

void renderEnemies(EnemyManager* manager, SDL_Renderer* renderer);

void removeEnemy(EnemyManager* manager, int index);

#endif
