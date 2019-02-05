#version 410 core

in vec2 out_uvs;
uniform sampler2D tex_base_color;
out vec4 color;

void main(){
    vec3 base_color;
    float grey;
    if(out_uvs.y>0.8||out_uvs.y<0.2){
        grey=0;
    }
    else{
        base_color = texture(tex_base_color, out_uvs).xyz;    
        grey=dot(base_color,vec3(0.299,0.587,0.114));
    }
    color=vec4(vec3(grey),1);
}