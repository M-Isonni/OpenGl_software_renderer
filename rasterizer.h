#include "aiv_math.h"
#include <math.h>
#include <SDL.h>

typedef struct context{
    int width;
    int height;

    unsigned char *framebuffer;

}context_t;

typedef struct vertex{
    Vector3_t position;
    Vector3_t normal;
    Vector3_t color;
    Vector3_t view_position;

    int raster_x;
    int raster_y;
}vertex_t;

vertex_t *vertex_new(Vector3_t *vec);

typedef struct triangle{
    vertex_t a;
    vertex_t b;
    vertex_t c;
}triangle_t;

triangle_t *array_of_triangles;
size_t array_of_triangle_size;

triangle_t *triangle_new(vertex_t *a,vertex_t *b, vertex_t *c);
void rasterize(context_t *ctx,triangle_t *triangle,Vector3_t *camera);
void put_pixel(context_t *ctx,int x, int y, Vector3_t* color);
void clear_screen(context_t *ctx);
void bubble_sort(vertex_t *vertexes, int size);
void append_triangle(triangle_t* value);
void manage_camera(Vector3_t *camera,triangle_t *triangle);
float slope(float x0,float y0,float x1, float y1);
