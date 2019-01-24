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

   
    
    GLuint program = attach_vertex_fragment_to_program(); 

    meshes_num=0;
    meshes_to_draw=NULL;  
    GenerateMesh("Mannequin.FBX","Texture.TGA",&program,0.01); 
    SDL_Log("Enter2");
    glUniform1f(meshes_to_draw[0].x_uniform,0.5);
    glUniform1f(meshes_to_draw[0].y_uniform,0.5);  
    // GenerateMesh("Mannequin.FBX","Texture.TGA",&program,0.01);
    // glUniform1f(meshes_to_draw[1].x_uniform,-0.5);
    // glUniform1f(meshes_to_draw[1].y_uniform,0.5);  
    SDL_Log("xuniform: %d",meshes_to_draw[0].x_uniform); 
    //SDL_Log("xuniform: %d",meshes_to_draw[1].x_uniform);
    
    
    GLint rot_uniform=glGetUniformLocation(program,"rot");    

    float fov = (60.0/2)*(M_PI/180);
    float camera_distance=tan(fov);

    GLint camera_distance_uniform=glGetUniformLocation(program,"camera_distance");    
    glUniform1f(camera_distance_uniform,camera_distance);  

    //glUniform1f(meshes_to_draw[0].tex_uniform,meshes_to_draw[0].texture_num); 
    //glUniform1f(meshes_to_draw[1].tex_uniform,meshes_to_draw[1].texture_num);
       
    float rot=0;     
   
    for(;;){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                return 0;
            }  
            if(event.type==SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_RIGHT)
                    meshes_to_draw[meshes_num-1].position_x+=0.1;
                if(event.key.keysym.sym==SDLK_LEFT)
                    meshes_to_draw[meshes_num-1].position_x-=0.1;
                if(event.key.keysym.sym==SDLK_UP)
                    meshes_to_draw[meshes_num-1].position_y+=0.1;
                if(event.key.keysym.sym==SDLK_DOWN){                    
                    meshes_to_draw[meshes_num-1].position_y-=0.1;
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
                        GenerateMesh(filename,"Texture.TGA",&program,0.01);
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

        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);       
        
        glUniform1f(rot_uniform,rot);
         
        for(int i=0;i<meshes_num;i++){

            glBindVertexArray(meshes_to_draw[i].vao);
            glUniform1f(meshes_to_draw[i].x_uniform,meshes_to_draw[i].position_x);
            glUniform1f(meshes_to_draw[i].y_uniform,meshes_to_draw[i].position_y); 
            glUniform1f(meshes_to_draw[i].scale_uniform,meshes_to_draw[i].scale);
            glDrawArrays(GL_TRIANGLES,0,meshes_to_draw[i].vertices_len/3);

        }

        SDL_GL_SwapWindow(window);
    }
    return 0;
}