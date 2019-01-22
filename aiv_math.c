#include "aiv_math.h"

Vector3_t *Vector3_new(float x, float y,float z){
    Vector3_t *vector =malloc(sizeof(Vector3_t));
    vector->x=x;
    vector->y=y;
    vector->z=z;
    return vector;
}

Vector3_t Vector3_zero(){
    Vector3_t vec={0,0,0};
    return vec;
}

float lerp(float start,float end,float gradient){
    return start + (end-start)*gradient;
}

//SlopeX = (x1-x0)/(y1-y0)