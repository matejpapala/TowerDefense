#include "enemyManager.h"

typedef struct {
    char text[10];
    SDL_Rect position;
    SDL_Color color;
    double startTime;
    double duration;
} TextEffect;

extern TextEffect textEffects[10];
extern int numTextEffects;
