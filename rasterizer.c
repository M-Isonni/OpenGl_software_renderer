#include "rasterizer.h"
#include <string.h>
#include <stdio.h>



vertex_t *vertex_new(Vector3_t *position){
    vertex_t *vertex=malloc(sizeof(vertex_t));
    memset(vertex,0,sizeof(vertex_t));
    vertex->position=*position;
    return vertex;
}

triangle_t *triangle_new(vertex_t *a,vertex_t *b, vertex_t *c){
    triangle_t *triangle= malloc(sizeof(triangle_t));
    triangle->a=*a;
    triangle->b=*b;
    triangle->c=*c;
    return triangle;
}

void append_triangle(triangle_t* value){    
    array_of_triangle_size++;
    triangle_t *resized_area=realloc(array_of_triangles,sizeof(triangle_t)*array_of_triangle_size);
    if(!resized_area){
        return;
    }
    array_of_triangles=resized_area;
    array_of_triangles[array_of_triangle_size-1]=*value;
}

static void view_to_raster(context_t *ctx, vertex_t *vertex){
    float fov = (60.0/2)*(M_PI/180);
    float camera_distance=tan(fov);
    float projected_x=vertex->view_position.x/(camera_distance*vertex->view_position.z);
    float projected_y=vertex->view_position.y/(camera_distance*vertex->view_position.z);

    vertex->raster_x=(projected_x+1)*(ctx->width*0.5);
    vertex->raster_y=ctx->height-(projected_y+1)*(ctx->height*0.5);
}

void rasterize(context_t *ctx,triangle_t *triangle,Vector3_t *camera){
    
    manage_camera(camera,triangle);
    view_to_raster(ctx,&triangle->a);
    view_to_raster(ctx,&triangle->b);
    view_to_raster(ctx,&triangle->c);

    vertex_t p[3];
    p[0]=triangle->a;
    p[1]=triangle->b;
    p[2]=triangle->c;

    bubble_sort(p,3);
    
    Vector3_t color;
    color.x=255;
    color.y=255;
    color.z=255;

    float slope_p0_p1 = slope(p[0].raster_x,p[0].raster_y,p[1].raster_x,p[1].raster_y);
    float slope_p0_p2 = slope(p[0].raster_x,p[0].raster_y,p[2].raster_x,p[2].raster_y);
    int slope = slope_p0_p1 >= slope_p0_p2 ? 1 : 0;    

    int y; 
    int x; 
    int x1;          
    int x2; 
    float gradient;   
    for(y=p[0].raster_y;y<=p[1].raster_y;y++){        
        
        gradient=1.0f;
        if(p[0].raster_y!=p[1].raster_y)
            gradient=(float)(y-p[0].raster_y)/(float)(p[1].raster_y-p[0].raster_y);

        x = lerp(p[0].raster_x,p[1].raster_x,gradient); 
        
        put_pixel(ctx,x,y,&color);
        
        gradient=1.0f;
        if(p[0].raster_y!=p[2].raster_y)
            gradient=(float)(y-p[0].raster_y)/(p[2].raster_y-p[0].raster_y);

        x1 = lerp(p[0].raster_x,p[2].raster_x,gradient);

        put_pixel(ctx,x1,y,&color);
        
        if(slope==0){
                for(x2=x;x2<x1;x2++){
                    color.x=x2;
                    color.y=y;
                    color.z=x1;
                    put_pixel(ctx,x2,y,&color);
                } 
             }
        else{
                for(x2=x1;x2<x;x2++){
                    color.x=x2;
                    color.y=y;
                    color.z=x1;
                    put_pixel(ctx,x2,y,&color);
                } 
             }
        }  
        
    color.x=255;
    color.y=255;
    color.z=255;

    for(y=p[1].raster_y;y<=p[2].raster_y;y++){ 

        gradient=1.0f;
        if(p[2].raster_y!=p[1].raster_y)
            gradient=(float)(y-p[1].raster_y)/(float)(p[2].raster_y-p[1].raster_y);

        x = lerp(p[1].raster_x,p[2].raster_x,gradient);  

        put_pixel(ctx,x,y,&color);

            gradient=1.0f;
        if(p[0].raster_y!=p[2].raster_y)
            gradient=(float)(y-p[0].raster_y)/(p[2].raster_y-p[0].raster_y);

        x1 = lerp(p[0].raster_x,p[2].raster_x,gradient);

        put_pixel(ctx,x1,y,&color);    
            
        if(slope==0){
                for(x2=x;x2<x1;x2++){
                    color.x=x2;
                    color.y=y;
                    color.z=x1;
                    put_pixel(ctx,x2,y,&color);
                } 
             }
        else
        {
                for(x2=x1;x2<x;x2++){
                    color.x=x2;
                    color.y=y;
                    color.z=x1;
                    put_pixel(ctx,x2,y,&color);
                } 
        }
    }   
} 

void put_pixel(context_t *ctx,int x, int y, Vector3_t* color){ 
    
    if(x<0||x>=ctx->width||y<0||y>=ctx->height){      
        return;
    }
    int index_a = (y*ctx->width+x)*4; 
    ctx->framebuffer[index_a++]=0;
    ctx->framebuffer[index_a++]=color->z;//b
    ctx->framebuffer[index_a++]=color->y;//g
    ctx->framebuffer[index_a]=color->x;//r   
}

void clear_screen(context_t *ctx){
    memset(ctx->framebuffer,0,ctx->width*ctx->height*4);
}

void bubble_sort(vertex_t *vertexes, int size){
    int i;
    for(i=0;i<size-1;i++)
    {
        int j;
        for(j=i+1;j<size;j++){
            vertex_t temp;
            if(vertexes[i].raster_y>vertexes[j].raster_y){
                temp=vertexes[i];
                vertexes[i]=vertexes[j];
                vertexes[j]=temp;   

            }
        }
    }
}

void manage_camera(Vector3_t *camera,triangle_t *triangle)
{
    triangle->a.view_position.x=triangle->a.position.x-camera->x;
    triangle->b.view_position.x=triangle->b.position.x-camera->x;
    triangle->c.view_position.x=triangle->c.position.x-camera->x;
    triangle->a.view_position.y=triangle->a.position.y-camera->y;
    triangle->b.view_position.y=triangle->b.position.y-camera->y;
    triangle->c.view_position.y=triangle->c.position.y-camera->y;
    triangle->a.view_position.z=triangle->a.position.z-camera->z;
    triangle->b.view_position.z=triangle->b.position.z-camera->z;
    triangle->c.view_position.z=triangle->c.position.z-camera->z;
   
}

float slope(float x0,float y0,float x1, float y1){

    return (x1-x0)/(y1-y0);

}

