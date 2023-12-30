#version 330 core

//TODO: (Light) Implement Lit Textured Vertex Shader

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
//sending normal as an attribute as we need the value of the normal now to calculate light correctly
layout(location = 3) in vec3 normal;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    //The vertex position relative to the world space
    vec3 world;
    //vector from the vertex to the eye relative to the world space
    vec3 view;
    //normal on the surface relative to the world space
    vec3 normal;
} vs_out;

uniform mat4 transform;
//to transform the surface normal
uniform mat4 objectToInvTranspose;
//to pass the data of the vertex relative to the world space
uniform mat4 objectToWorld;
//used to calculate the specular
uniform vec3 cameraPosition;

void main(){
    gl_Position = transform * vec4(position, 1.0); // apply transformation mat to the vec
    //calculate the position relative to the world space
    vs_out.world = (objectToWorld * vec4(position, 1.0f)).xyz;
    vs_out.tex_coord = tex_coord;
    //calculate the view vector relative to the world space to be passed to the fragment shader to calculate the phong factor
    vs_out.view = cameraPosition - vs_out.world;
    vs_out.color = color;
    //calculate the normal
    vs_out.normal = normalize((objectToInvTranspose * vec4(normal, 0.0f)).xyz);
}