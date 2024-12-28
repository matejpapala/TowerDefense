#ifndef TURRET_MANAGER_H
#define TURRET_MANAGER_H

#include <SDL2/SDL.h>
#include "enemyManager.h"

typedef struct {
    SDL_Rect position; 
    SDL_Rect renderPosition;
    int range; 
    int damage; 
    double shootCooldown;
    double lastShotTime; 
    int upgradeLevel;
    SDL_Texture* texture;
} Turret;

void shootEnemies(Turret* turret, EnemyManager* enemyManager, SDL_Renderer* renderer, double elapsedTime);
void upgradeTurret(Turret* turret, int* playerMoney, SDL_Renderer* renderer);
#endif
