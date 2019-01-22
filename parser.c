#include "parser.h"

int max_index=0;
char* save_vertex(char* data, int *index){
    if(data[*index]=='v'){
        *index+=2;
    }
    char* c=NULL;
    size_t length=0;
    while(!SDL_isspace(data[*index]) && data[*index]!='v' && data[*index]!='\n'){
        length++;
        c=realloc(c,length);       
        c[length-1]=data[*index];
        *index+=1;        
    }    
    if(data[*index]=='\n')
    {
        length++;
        c=realloc(c,length);       
        c[length-1]=0;
        return c;
    }   
    else if(SDL_isspace(data[*index]))
    {
        length++;
        c=realloc(c,length);       
        c[length-1]=0;
        *index+=1;
        return c;
    }
    
    return c;
}

char* save_faces(char* data, int *index,size_t* max_size){
    if(data[*index]=='f'){
        *index+=2;
    }
    char* c=NULL;
    size_t length=0;
    while(data[*index]!='/'&&*index<=*max_size){
        length++;
        c=realloc(c,length);
        c[length-1]=data[*index];
        *index+=1;
    }
    if(data[*index]=='/')
    {
        length++;
        c=realloc(c,length);       
        c[length-1]=0; 
        while(!SDL_isspace(data[*index])&&data[*index]!='f'&&*index<=*max_size){
            *index+=1;
        }
        if(SDL_isspace(data[*index])&&*index<=*max_size){
            index+=1;
        }
        else if(data[*index]=='f'&&*index<=*max_size){
            index-=1;
        }
        return c;
    }
    return c;
}

char* read_file(const char* filename, size_t *file_size){
    FILE *file_reader=fopen(filename,"rb");
    if(!file_reader)
    return NULL;

    fseek(file_reader,0,SEEK_END);
    *file_size=ftell(file_reader);
    fseek(file_reader,0,SEEK_SET);

    char *data=malloc(*file_size);

    if(!data)
    {
        fclose(file_reader);
        return NULL;
    }

    fread (data,1,*file_size,file_reader);
    fclose(file_reader);
    return data;
}

void parse_obj(char* data,size_t* file_size){

    for(int i=0;i<*file_size;i++)
    {
        if(data[i]=='v'&&data[i+1]==' '){            
            char* x=save_vertex(data,&i);
            char* y=save_vertex(data,&i);
            char* z=save_vertex(data,&i);            
            vertex_array_size++;
            vertex_array=realloc(vertex_array,sizeof(vertex_t)*vertex_array_size);
            vertex_t *vertex=vertex_new(Vector3_new((float)atof(x),(float)atof(y),(float)atof(z)));
            vertex_array[vertex_array_size-1]=*vertex;
        }
    }
}

void parse_normals(char* data,size_t* file_size){

    for(int i=0;i<*file_size;i++)
    {
        if(data[i]=='v'&&data[i+1]==' '){            
            char* x=save_vertex(data,&i);
            char* y=save_vertex(data,&i);
            char* z=save_vertex(data,&i);            
            vertex_array_size++;
            vertex_array=realloc(vertex_array,sizeof(vertex_t)*vertex_array_size);
            vertex_t *vertex=vertex_new(Vector3_new((float)atof(x),(float)atof(y),(float)atof(z)));
            vertex_array[vertex_array_size-1]=*vertex;
        }
    }
}

void parse_obj_faces(char* data,size_t* file_size){
    for(int i=0;i<*file_size;i++)
    {
        if(data[i]=='f'){   
            char* index1=save_faces(data,&i, file_size);
            char* index2=save_faces(data,&i,file_size);
            char* index3=save_faces(data,&i,file_size);         
            
            triangle_t *triangle=triangle_new(&vertex_array[atoi(index1)-1],&vertex_array[atoi(index2)-1],&vertex_array[atoi(index3)-1]);
            
            append_triangle(triangle);
        }
    }
}


