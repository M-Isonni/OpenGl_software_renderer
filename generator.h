#include <SDL.h>
#include <glad.h>
#include "parser.h"
#include "libfbxc.h"
#include <stb_image.h>

typedef struct mesh{
    int vertices_len;
    GLuint vao;
    GLint x_uniform;
    GLint y_uniform;
    GLint scale_uniform;
    float position_x;
    float position_y;
    float scale;
    int texture_num;
    GLint tex_uniform;
}mesh_t;
int meshes_num;
mesh_t *meshes_to_draw;

void GenerateMesh(char *fbx_name,char *texture_name,GLuint *program,float scale);
GLuint CompileMesh(fbxc_scene_t *obj,char* texture_name);
GLuint attach_vertex_fragment_to_program();
void GenerateTexture(char* texture_name);
GLuint compile_shader(GLenum shader_type, const char *filename);
void AddTexture(char* texture_name,mesh_t *activemesh);