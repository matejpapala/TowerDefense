#ifndef TURRET_MANAGER_H
#define TURRET_MANAGER_H

#include <SDL2/SDL.h>
#include "enemyManager.h"

typedef struct {
    SDL_Rect position;  // Position and size of the turret
    int range;          // Range of the turret
    int damage;         // Damage dealt per shot
    double shootCooldown; // Time between shots in milliseconds
    double lastShotTime;  // Time when the turret last shot
} Turret;

void shootEnemies(Turret* turret, EnemyManager* enemyManager, SDL_Renderer* renderer, double elapsedTime);

#endif
