#include <SDL.h>
void handleinput(SDL_bool KEYS[],SDL_Rect *rect, int *running, int speed );
void KeysInit(SDL_bool KEYS[]);
void manage_inputs(SDL_Event *event, SDL_Rect *rect,int speed);