#include "generator.h"

GLuint compile_shader(GLenum shader_type, const char *filename){

    SDL_RWops *rw=SDL_RWFromFile(filename,"rb");
    if(!rw){
        SDL_Log("unable to open file");
        exit(1);
    }

    size_t file_len = SDL_RWsize(rw);
    char *source=SDL_malloc(file_len+1);
    if(!source){
        SDL_Log("unable to allocate memory");
        exit(1);
    }
    if(SDL_RWread(rw,source,1,file_len)!=file_len){
        SDL_Log("unable to read file");
        exit(1);
    }

    source[file_len]=0;

    SDL_RWclose(rw);

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader,1,(const GLchar**)&source,(GLint *)&file_len);
    glCompileShader(shader);

    SDL_free(source);

    GLint compile_status;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&compile_status);
    if(compile_status==GL_FALSE){
        GLint log_size;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&log_size);
        char *error_log=SDL_malloc(log_size+1);
        if(!error_log){
            SDL_Log("unable to allocato memory for log");
            exit(1);
        }
        glGetShaderInfoLog(shader,log_size,&log_size,error_log);
        error_log[log_size]=0;
        SDL_Log("shader compile error: %s", error_log);
        SDL_free(error_log);
        exit(1);
    }

    return shader;
}

GLuint GenerateTexture(char* texture_name){

    int w,h,comp;
    unsigned char *pixels = stbi_load(texture_name,&w,&h,&comp,4);
    if(!pixels){
        SDL_Log("texture not loaded correctly");
        exit(1);
    }   

    GLuint tex;     
    glGenTextures(1,&tex);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    free(pixels);
    return tex;
}

void AddTexture(char* texture_name,mesh_t *activemesh){

    int w,h,comp;
    unsigned char *pixels = stbi_load(texture_name,&w,&h,&comp,4);
    if(!pixels){
        SDL_Log("texture not loaded correctly");
        exit(1);
    }   
    activemesh->texture_num++;
    GLuint tex;        
    glGenTextures(1,&tex);
    SDL_Log("Texture num: %d",activemesh->texture_num);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    free(pixels);
}

GLuint attach_vertex_fragment_to_program(char *vertex,char *geom, char *frag){

    GLuint program = glCreateProgram();
    glEnable(GL_DEPTH_TEST);    
    glEnable(GL_CULL_FACE|GL_FRONT);
    if(!geom){
        GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER,vertex);
        GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER,frag);

        glAttachShader(program,vertex_shader);
        glAttachShader(program,fragment_shader);

        glLinkProgram(program);

        glDetachShader(program,vertex_shader);
        glDetachShader(program,fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    else{
        GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER,vertex);
        GLuint geom_shader = compile_shader(GL_GEOMETRY_SHADER,geom);
        GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER,frag);

        glAttachShader(program,vertex_shader);
        glAttachShader(program,geom_shader);
        glAttachShader(program,fragment_shader);

        glLinkProgram(program);

        glDetachShader(program,vertex_shader);
        glDetachShader(program,geom_shader);
        glDetachShader(program,fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(geom_shader);
        glDeleteShader(fragment_shader);
    }    

    glUseProgram(program);
    return program;
}

GLuint CompileMesh(fbxc_scene_t *obj,char* texture_name){

    GLuint vao=meshes_num;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint vbo[3];
    glGenBuffers(3,vbo);  
    
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,obj->vertices_len * sizeof(float),obj->vertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,obj->normals_len * sizeof(float),obj->normals,GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
    glBufferData(GL_ARRAY_BUFFER,obj->uvs_len * sizeof(float),obj->uvs,GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,0);    

    GenerateTexture(texture_name);
    return vao;
}

void GenerateMesh(char *fbx_name,char *texture_name,GLuint *program,float scale){
    fbxc_scene_t *obj= fbxc_parse_file(fbx_name);
    if(!obj){
        SDL_Log("fbx not parsed");
    }
    
    GLuint vao = CompileMesh(obj,texture_name);
    SDL_Log("returned vao: %d",vao);
    GLuint tex=GenerateTexture(texture_name);
    mesh_t *new_mesh=malloc(sizeof(mesh_t));
    memset(new_mesh,0,sizeof(mesh_t));
    new_mesh->vertices_len = obj->vertices_len;
    new_mesh->vao=vao;
    new_mesh->x_uniform=glGetUniformLocation(*program,"x");
    new_mesh->y_uniform=glGetUniformLocation(*program,"y");
    new_mesh->scale_uniform=glGetUniformLocation(*program,"scale");
    new_mesh->tex_uniform=glGetUniformLocation(*program,"tex_base_color");
    new_mesh->scale=scale;
    new_mesh->texture_num=0; 
    new_mesh->tex = tex;
    glUniform1f(new_mesh->tex_uniform,0); 
    fbxc_scene_free(obj);  
    meshes_num++;
    mesh_t * new_alloc = realloc(meshes_to_draw,sizeof(mesh_t)*meshes_num);
    meshes_to_draw=new_alloc;
    meshes_to_draw[meshes_num-1]=*new_mesh;
    free(new_mesh);       
}