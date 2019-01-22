#include "rasterizer.h"
#include <stdio.h>

vertex_t* vertex_array;
size_t vertex_array_size;

void parse_obj(char* data,size_t* file_size);
void parse_obj_faces(char* data,size_t* file_size);
char* read_file(const char* filename, size_t *file_size);
char* save_faces(char* data, int *index,size_t* max_size);
char* save_vertex(char* data, int *index);
