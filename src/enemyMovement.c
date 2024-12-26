//
//  enemyMovement.c
//  ProjectTowerDefense
//
//  Created by Matěj Papala on 05.12.2024.
//

#include "enemyMovement.h"

Enemy* spawnEnemy(SDL_Renderer *renderer, SDL_Point *path, int pathLength, const char *imagePath) {
    Enemy *enemy = (Enemy *)malloc(sizeof(Enemy));
    enemy->x = path[0].x;
    enemy->y = path[0].y;
    enemy->speed = 2;
    enemy->hp = 10;
    enemy->pathIndex = 0;
    enemy->path = path;
    enemy->pathLength = pathLength;
    //error cathing
    SDL_Surface *tempSurface = IMG_Load(imagePath);
        if (!tempSurface) {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            free(enemy);
            return NULL;
        }
    enemy->image = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!enemy->image) {
        fprintf(stderr, "Texture creat fail: %s\n", SDL_GetError());
        free(enemy);
        return NULL;
    }

    return enemy;
}

void moveEnemy(Enemy *enemy, double deltaTime) {
    if (!enemy || !enemy->path) {
        fprintf(stderr, "Invalid enemy or path\n");
        return;
    }

    if (enemy->pathIndex >= enemy->pathLength - 1) {
        // reach end
        return;
    }

    SDL_Point target = enemy->path[enemy->pathIndex + 1];
    double dx = target.x - enemy->x;
    double dy = target.y - enemy->y;
    float distance = sqrt(dx * dx + dy * dy);
    float step = enemy->speed * deltaTime;

    if (distance < step) {
        // enemy reached point
        enemy->x = target.x;
        enemy->y = target.y;
        enemy->pathIndex++;
    } else if (distance > 0) { // Ensure distance is not zero
        // moving enemy
        enemy->x += (int)round(step * dx / distance);
        enemy->y += (int)round(step * dy / distance);
    }

    printf("Enemy Position: (%d, %d), Target: (%d, %d), PathIndex: %d\n",
       enemy->x, enemy->y, target.x, target.y, enemy->pathIndex);

}


void renderEnemy(SDL_Renderer *renderer, Enemy *enemy) {
    if (!enemy) {
        fprintf(stderr, "Invalid enemy pointer\n");
        return;
    }

    if (!enemy->image) {
        fprintf(stderr, "Invalid enemy image\n");
        return;
    }

    SDL_Rect dstRect = {(int)enemy->x - 35, (int)enemy->y - 35, 70, 70}; // numbers for adjusting enemy size
    SDL_RenderCopy(renderer, enemy->image, NULL, &dstRect);
}
