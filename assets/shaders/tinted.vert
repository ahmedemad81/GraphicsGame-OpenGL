#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out Varyings {
    vec4 color;
} vs_out;

uniform mat4 transform;

void main(){
    //DONE (Req 7) Change the next line to apply the transformation matrix

    // gl_Position is a built-in variable OpenGL that represents the output position of a vertex shader 
    // after the various transformation stages in the graphics pipeline.
    // It is a 4D vector that represents the homogeneous coordinates of the vertex in clip space.
    // its value will be automatically interpolated across the primitive (e.g., a triangle) during rasterization 
    // to determine the positions of fragments (pixels) in the fragment shader.
    gl_Position = transform * vec4(position, 1.0f);
    vs_out.color = color;
}