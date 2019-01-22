#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 input_color;
out vec3 vertex_color;

uniform float x;
uniform float y;
uniform float camera_distance;

void main(){ 

    // float new_x=vertex.x*2/width-1;
    // float new_y=(height-vertex.y)*2/height-1;

    gl_Position=vec4(vertex.x+x,vertex.y+y,vertex.z*camera_distance*(vertex.z+5),camera_distance*(vertex.z+5));
    vertex_color = input_color;
}