#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 input_uvs;

out vec2 out_uvs;

void main(){ 
   
    gl_Position=vec4(vertex.xyz,1);
    out_uvs=input_uvs;
}