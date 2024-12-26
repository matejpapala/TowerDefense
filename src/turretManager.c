#include "turretManager.h"

void shootEnemies(Turret* turret, EnemyManager* enemyManager, SDL_Renderer* renderer, double elapsedTime) {
    if (elapsedTime - turret->lastShotTime < turret->shootCooldown) {
        return; 
    }

    // enemy check
    for (int i = 0; i < enemyManager->activeEnemies; i++) {
        Enemy* enemy = enemyManager->enemies[i];
        if (enemy != NULL) {
            double dx = enemy->x - turret->position.x;
            double dy = enemy->y - turret->position.y;
            double distance = sqrt(dx * dx + dy * dy);

            if (distance <= turret->range) {
                //shoot
                enemy->hp -= turret->damage;
                turret->lastShotTime = elapsedTime;

                // projectile anim
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black color for projectile
                SDL_RenderDrawLine(renderer, turret->position.x + 25, turret->position.y + 25, enemy->x, enemy->y);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Reset to white

                printf("Turret shot enemy at (%d, %d), HP left: %d\n", enemy->x, enemy->y, enemy->hp);
                break;
            }
        }
    }
}
