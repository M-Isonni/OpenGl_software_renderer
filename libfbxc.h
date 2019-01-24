#include <stdlib.h>

#ifdef _WIN32
#define FBXC_API __declspec(dllexport)
#else
#define FBXC_API
#endif

typedef struct fbxc_scene
{
    int major;
    int minor;
    int revision;

    float *vertices;
    size_t vertices_len;
    float *normals;
    size_t normals_len;
    float *colors;
    size_t colors_len;
    float *uvs;
    size_t uvs_len;
} fbxc_scene_t;

#ifdef __cplusplus
extern "C"
{
#endif

FBXC_API fbxc_scene_t *fbxc_parse_file(const char *filename);
FBXC_API void fbxc_scene_free(fbxc_scene_t *scene);

#ifdef __cplusplus
}
#endif