#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 input_normals;
layout(location=2) in vec2 input_uvs;
out vec3 vertex_normals;
out vec2 out_uvs;

uniform float colors[6];
out vec3 instance_color;

uniform float rot;
float rotation;
uniform float x;
uniform float y;
uniform float camera_distance;
uniform float scale;

vec3 rotate_y(vec3 v, float rot){
    vec3 vo;
    vo.x = cos(rot) * v.x - sin(rot)* v.z;
    vo.y = v.y;
    vo.z= sin(rot) * v.x + cos(rot) * v.z;
    return vo;
}

float linear_convert(float value,float old_min,float old_max,float new_min, float new_max){
    float gradient = (value - old_min) / (old_max - old_min);
    return new_min + gradient * (new_max-new_min);
}

void main(){ 
    //vec3 world_normals=vec3(0,0,0);
    if(gl_InstanceID==0){

        rotation=0;
    }
    else{
        rotation=rot;
    }
    instance_color=vec3(colors[gl_InstanceID*3],colors[gl_InstanceID*3+1],colors[gl_InstanceID*3+2]);
    vec3 world_position= rotate_y(vec3(vertex.y,vertex.z,vertex.x)*scale,rotation);
    vec3 world_normals= rotate_y(vec3(input_normals.y,input_normals.z,input_normals.x)*1,rotation);
    if(gl_InstanceID==0){        

        gl_Position=vec4(world_position.x,world_position.y,world_position.z*camera_distance*(world_position.z+5),camera_distance*(world_position.z+5));
    }
    else{
                gl_Position=vec4(world_position.x+x,world_position.y+y,world_position.z*camera_distance*(world_position.z+5),camera_distance*(world_position.z+5));
    }
    // ortographic size
    // float new_x=linear_convert(world_position.x,-200,200,-1,1);
    // float new_y=linear_convert(world_position.y,-200,200,-1,1);
    // float new_z=linear_convert(world_position.z,-200,200,-1,1);
    // gl_Position=vec4(new_x+x,new_y+y,new_z,1);

    //prospective size        
    vertex_normals = world_normals;
    out_uvs=input_uvs;
}