#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vertex_normals[];
in vec2 out_uvs[];
in vec3 instance_color[];

out vec3 vertex_normals_frag;
out vec2 out_uvs_frag;
out vec3 instance_color_frag;
out vec3 normal_frag;
uniform float distance_geom;

// in vec2 uv_out;
// out vec2 uv2_out;

void main(){
    vec3 first=normalize(gl_in[1].gl_Position.xyz-gl_in[0].gl_Position.xyz);
    vec3 second = normalize(gl_in[2].gl_Position.xyz-gl_in[0].gl_Position.xyz);
    vec3 normal=cross(first,second);
    normal_frag=normal;
    vec4 normal_for_pos=vec4(normal,0);

    gl_Position = gl_in[0].gl_Position+normal_for_pos*distance_geom;
    vertex_normals_frag=vertex_normals[0];
    out_uvs_frag=out_uvs[0];
    instance_color_frag=instance_color[0];
    //uv2_out=uv_out[0];
    EmitVertex();
    gl_Position = gl_in[1].gl_Position+normal_for_pos*distance_geom;
    vertex_normals_frag=vertex_normals[1];
    out_uvs_frag=out_uvs[1];
    instance_color_frag=instance_color[1];
    //uv2_out=uv_out[1];
    EmitVertex();
    gl_Position = gl_in[2].gl_Position+normal_for_pos*distance_geom;
    vertex_normals_frag=vertex_normals[2];
    out_uvs_frag=out_uvs[2];
    instance_color_frag=instance_color[2];
    //uv2_out=uv_out[2];
    EmitVertex();


    EndPrimitive();
}