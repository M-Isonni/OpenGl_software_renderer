#include <stdlib.h>

typedef struct Vector3{
    float x;
    float y;
    float z;
}Vector3_t;

Vector3_t *Vector3_new(float x, float y,float z);
Vector3_t Vector3_zero();
float lerp(float start,float end,float gradient);