//
//  enemyMovement.h
//  ProjectTowerDefense
//
//  Created by Matěj Papala on 05.12.2024.
//

#ifndef enemyMovement_h
#define enemyMovement_h

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int x, y;
    int speed;
    int hp;
    SDL_Texture *image;
    int pathIndex;//index of the point he goes to
    SDL_Point *path;//array of points
    int pathLength;//length of hi sw way
} Enemy;

Enemy* spawnEnemy(SDL_Renderer *renderer, SDL_Point *path, int pathLength, const char *imagePath);
void moveEnemy(Enemy *enemy, double deltaTime);
void renderEnemy(SDL_Renderer *renderer, Enemy *enemy);

#endif
