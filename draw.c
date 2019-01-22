#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glad.h>
#include "parser.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

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

int main(int argc, char **argv){

    context_t ctx;
    ctx.width=600;
    ctx.height=600;    

    array_of_triangles=NULL;
    array_of_triangle_size=0;  
    vertex_array=NULL;
    vertex_array_size=0;

    ctx.framebuffer=NULL; 
    size_t size;
    char* data=read_file("Stormtrooper.obj",&size);
    parse_obj(data,&size);
    parse_obj_faces(data,&size);

    size_t vertex_positions_array_size=sizeof(float)*3*3*array_of_triangle_size;  
    float *vertex_positions_array=malloc(vertex_positions_array_size);
    SDL_Log("vertexes: %d",vertex_positions_array_size/sizeof(float));

    for(int i=0;i<array_of_triangle_size;i++){   
    vertex_positions_array[i*9]=array_of_triangles[i].a.position.x;
    vertex_positions_array[i*9+1]=array_of_triangles[i].a.position.y;
    vertex_positions_array[i*9+2]=array_of_triangles[i].a.position.z*-1;    
    vertex_positions_array[i*9+3]=array_of_triangles[i].b.position.x;
    vertex_positions_array[i*9+4]=array_of_triangles[i].b.position.y;
    vertex_positions_array[i*9+5]=array_of_triangles[i].b.position.z*-1;    
    vertex_positions_array[i*9+6]=array_of_triangles[i].c.position.x;
    vertex_positions_array[i*9+7]=array_of_triangles[i].c.position.y;
    vertex_positions_array[i*9+8]=array_of_triangles[i].c.position.z*-1;    
    }


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window=SDL_CreateWindow("OpenGL",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    if(!window){
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    SDL_Log("GL_VENDOR: %s",glGetString(GL_VENDOR));
    SDL_Log("GL_RENDERER: %s",glGetString(GL_RENDERER));
    SDL_Log("GL VERSION: %s",glGetString(GL_VERSION));
    SDL_Log("GL_SHADING_LANGUAGE_VERSION: %s",glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(0,1,0,1);

    GLuint vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    GLuint vbo[2];
    glGenBuffers(2,vbo);

    float fov = (60.0/2)*(M_PI/180);
    float camera_distance=tan(fov);
    
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    //copy vertices in vbo[0];
    glBufferData(GL_ARRAY_BUFFER,vertex_positions_array_size,vertex_positions_array,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    float colors[]={
        1,0,0,
        0,1,0,
        0,1,1
    };

    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    //copy vertices in vbo[0];
    glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    GLuint program = glCreateProgram();
    glEnable(GL_DEPTH_TEST);    

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER,"vertex.glsl");
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER,"frag.glsl");

    glAttachShader(program,vertex_shader);
    glAttachShader(program,fragment_shader);

    glLinkProgram(program);

    glDetachShader(program,vertex_shader);
    glDetachShader(program,fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glUseProgram(program);

    

    GLint x_uniform=glGetUniformLocation(program,"x");
    GLint y_uniform=glGetUniformLocation(program,"y");
    
    GLint camera_distance_uniform=glGetUniformLocation(program,"camera_distance");    
    glUniform1f(camera_distance_uniform,camera_distance);  
    SDL_Log("fov: %f",camera_distance);
   
    float triangle_x=0;
    float triangle_y=0;
   
    for(;;){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                return 0;
            }  
            if(event.type==SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_RIGHT)
                    triangle_x+=0.1;
                if(event.key.keysym.sym==SDLK_LEFT)
                    triangle_x-=0.1;
                if(event.key.keysym.sym==SDLK_UP)
                    triangle_y+=0.1;
                if(event.key.keysym.sym==SDLK_DOWN)
                    triangle_y-=0.1;
            }            
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        glUniform1f(x_uniform,triangle_x);
        glUniform1f(y_uniform,triangle_y);
        
        glDrawArrays(GL_TRIANGLES,0,vertex_positions_array_size);

        SDL_GL_SwapWindow(window);
    }
    return 0;
}