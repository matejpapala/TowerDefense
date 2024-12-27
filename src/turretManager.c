#include "turretManager.h"

void shootEnemies(Turret* turret, EnemyManager* enemyManager, SDL_Renderer* renderer, double elapsedTime) {
    if (elapsedTime - turret->lastShotTime < turret->shootCooldown) {
        return;//cooldown
    }

    // check range
    for (int i = 0; i < enemyManager->activeEnemies; i++) {
        Enemy* enemy = enemyManager->enemies[i];
        if (enemy != NULL) {
            double dx = enemy->x - turret->position.x;
            double dy = enemy->y - turret->position.y;
            double distance = sqrt(dx * dx + dy * dy);
            if (distance <= turret->range) {
                enemy->hp -= turret->damage;
                turret->lastShotTime = elapsedTime;
                // animation of shooting
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  
                SDL_RenderDrawLine(renderer, turret->position.x + 25, turret->position.y + 25, enemy->x, enemy->y);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
                break; 
            }
        }
    }
}

