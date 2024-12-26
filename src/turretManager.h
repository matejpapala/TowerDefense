#ifndef TURRET_MANAGER_H
#define TURRET_MANAGER_H

#include <SDL2/SDL.h>
#include "enemyManager.h"

typedef struct {
    SDL_Rect position; 
    int range; 
    int damage; 
    double shootCooldown;
    double lastShotTime; 
} Turret;

void shootEnemies(Turret* turret, EnemyManager* enemyManager, SDL_Renderer* renderer, double elapsedTime);

#endif
