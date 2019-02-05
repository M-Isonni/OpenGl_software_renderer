#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include "generator.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

int main(int argc, char **argv){      

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window=SDL_CreateWindow("OpenGL",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_OPENGL);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    if(!window){
        return -1;
    }   

    SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);    

    glClearColor(0,1,0,1);  

    GLuint fbs[1];
    glGenFramebuffers(1,fbs);

    GLuint render_textures[2];
    glGenTextures(2,render_textures);
    glBindFramebuffer(GL_FRAMEBUFFER,fbs[0]);

    glBindTexture(GL_TEXTURE_2D,render_textures[0]);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,1024,1024,0,GL_DEPTH_COMPONENT,GL_FLOAT,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D,render_textures[1]);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1024,1024,0,GL_RGBA,GL_FLOAT,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,render_textures[0],0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,render_textures[1],0);
    
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    GLuint program = attach_vertex_fragment_to_program("vertex.glsl","pp_geom.glsl","frag.glsl"); 
    GLuint program2 = attach_vertex_fragment_to_program("vertex2.glsl",NULL,"frag2.glsl");
    
                    glUseProgram(program2);
                    GLuint vao;
                    glGenVertexArrays(1,&vao);
                    glBindVertexArray(vao);

                    GLuint vbo[2];
                    glGenBuffers(2,vbo);  
                    float quad[]={
                        1,1,0,
                        -1,1,0,
                        -1,-1,0,
                        1,1,0,
                        -1,-1,0,
                        1,-1,0
                    };
                    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
                    glBufferData(GL_ARRAY_BUFFER,6 *3* sizeof(float),quad,GL_STATIC_DRAW);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
                    float uv[]={
                        1,1,
                        0,1,
                        0,0,
                        1,1,
                        0,0,
                        1,0
                    };
                    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
                    glBufferData(GL_ARRAY_BUFFER,6 * 2*sizeof(float),uv,GL_STATIC_DRAW);
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
                    GLuint uniform_tex=glGetUniformLocation(program2,"tex_base_color");
                    glUniform1f(uniform_tex,0); 
    
    glUseProgram(program);

    meshes_num=0;
    meshes_to_draw=NULL;  
    GenerateMesh("Mannequin.FBX","Texture.TGA",&program,0.01); 
    SDL_Log("Enter2");
    glUniform1f(meshes_to_draw[0].x_uniform,0.5);
    glUniform1f(meshes_to_draw[0].y_uniform,0.5); 
    SDL_Log("xuniform: %d",meshes_to_draw[0].x_uniform); 
    
    GLint rot_uniform=glGetUniformLocation(program,"rot");    

    float fov = (60.0/2)*(M_PI/180);
    float camera_distance=tan(fov);

    GLint camera_distance_uniform=glGetUniformLocation(program,"camera_distance");    
    glUniform1f(camera_distance_uniform,camera_distance);    

    float colors[]={
        1,0,1,
        0,1,0
    };
    GLint uniform_colors=glGetUniformLocation(program,"colors");    
    glUniform1fv(uniform_colors,6,colors);
    //glUniform1f(camera_distance_uniform,camera_distance);   

    GLint uniform_distance=glGetUniformLocation(program,"distance_geom");  
    SDL_Log("distance %d",uniform_distance);
    float distance_geom=0;
       
    float rot=0;     
    


    for(;;){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                return 0;
            }  
            if(event.type==SDL_KEYDOWN){
                // if(event.key.keysym.sym==SDLK_RIGHT)
                //     meshes_to_draw[meshes_num-1].position_x+=0.1;
                // if(event.key.keysym.sym==SDLK_LEFT)
                //     meshes_to_draw[meshes_num-1].position_x-=0.1;
                if(event.key.keysym.sym==SDLK_UP)
                    meshes_to_draw[meshes_num-1].position_y+=0.1;
                if(event.key.keysym.sym==SDLK_DOWN){                    
                    meshes_to_draw[meshes_num-1].position_y-=0.1;
                }
                if(event.key.keysym.sym==SDLK_LEFT){
                    //SDL_Log("print");
                    distance_geom+=0.01;
                }
                if(event.key.keysym.sym==SDLK_RIGHT){

                    distance_geom-=0.01;
                }
                
            } 
            if(event.type==SDL_DROPFILE){
                    char *filename=event.drop.file;  
                    int i=0;
                    while(filename[i]!=0){
                        i++;
                    }
                    i--;
                    if(filename[i]=='X'||filename[i]=='x'){
                        GenerateMesh(filename,"Random.TGA",&program,0.01);
                        glUniform1f(meshes_to_draw[meshes_num-1].x_uniform,0.5);
                        glUniform1f(meshes_to_draw[meshes_num-1].y_uniform,0.5); 
                        SDL_Log(filename);         
                    }
                    if(filename[i]=='A'||filename[i]=='a'){
                        glBindVertexArray(meshes_to_draw[meshes_num-1].vao);
                        AddTexture(filename,&meshes_to_draw[meshes_num-1]);
                        glUniform1f(meshes_to_draw[meshes_num-1].tex_uniform,GL_TEXTURE0+meshes_to_draw[meshes_num-1].texture_num);
                    }
                }             
        }
        rot+=0.01;  
        glBindFramebuffer(GL_FRAMEBUFFER,fbs[0]);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(program);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);       
        glUniform1f(rot_uniform,rot);
        glUniform1f(uniform_distance,distance_geom);  
         
        for(int i=0;i<meshes_num;i++){
            glBindVertexArray(meshes_to_draw[i].vao);
            glUniform1f(meshes_to_draw[i].x_uniform,meshes_to_draw[i].position_x);
            glUniform1f(meshes_to_draw[i].y_uniform,meshes_to_draw[i].position_y); 
            glUniform1f(meshes_to_draw[i].scale_uniform,meshes_to_draw[i].scale);
            glBindTexture(GL_TEXTURE_2D,meshes_to_draw[i].tex);
            glDrawArraysInstanced(GL_TRIANGLES,0,meshes_to_draw[i].vertices_len/3,2);
        }

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(program2);
        glBindTexture(GL_TEXTURE_2D,render_textures[1]);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,6);



        SDL_GL_SwapWindow(window);
    }
    return 0;
}