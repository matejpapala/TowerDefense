#include "turretManager.h"

void shootEnemies(Turret* turret, EnemyManager* enemyManager, SDL_Renderer* renderer, double elapsedTime) {
    if (elapsedTime - turret->lastShotTime < turret->shootCooldown) {
        return;
    }

    int turretCenterX = turret->position.x + turret->position.w / 2;
    int turretCenterY = turret->position.y + turret->position.h / 2;

    for (int i = 0; i < enemyManager->activeEnemies; i++) {
        Enemy* enemy = enemyManager->enemies[i];
        if (enemy != NULL) {
            double dx = enemy->x - turretCenterX;
            double dy = enemy->y - turretCenterY;
            double distanceSquared = dx * dx + dy * dy;

            if (distanceSquared <= turret->range * turret->range) {
                enemy->hp -= turret->damage;
                turret->lastShotTime = elapsedTime;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  
                SDL_RenderDrawLine(renderer, turretCenterX, turretCenterY, enemy->x, enemy->y);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                break;
            }
        }
    }
}


void upgradeTurret(Turret* turret, int* playerMoney, SDL_Renderer* renderer) {
    const int upgradeCost = 100; 
    const char* upgradeTextures[] = {
        "../../src/Assets/towerBasic.png",
        "../../src/Assets/towerLevelOne.png",
        "../../src/Assets/towerLevelTwo.png"
    };

    if (turret->upgradeLevel < 2 && *playerMoney >= upgradeCost) {
        *playerMoney -= upgradeCost; 
        turret->upgradeLevel++;   
        turret->damage += 10;  
        turret->range += 50;   
        SDL_DestroyTexture(turret->texture);
        SDL_Surface* tempSurface = IMG_Load(upgradeTextures[turret->upgradeLevel]);
        if (tempSurface) {
            turret->texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
        }
    }
}
