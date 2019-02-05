#version 410 core

out vec4 color;
in vec3 vertex_normals_frag;
in vec2 out_uvs_frag;
in vec3 instance_color_frag;
in vec3 normal_frag;

uniform sampler2D tex_base_color;

void main(){

    vec3 light_point= vec3(1,0,-1);

    vec3 base_color = texture(tex_base_color,vec2(out_uvs_frag.x,1-out_uvs_frag.y)).xyz;

    float lambert = clamp(dot(vertex_normals_frag, light_point), 0, 1);
    //base_color *=lambert;
   // base_color=instance_color_frag;
    base_color*=lambert;
    color=vec4(base_color,1);
}