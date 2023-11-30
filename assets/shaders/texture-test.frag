#version 330 core

in Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} fs_in;

out vec4 frag_color;

uniform sampler2D tex;

void main(){
    //DONE (Req 5) Change the following line to read the fragment color
    // Get the color from the texture at the received texture coordinates 
    //Multiplying the texture value by the fragment color allows you 
    //to blend the texture color with the fragment color, creating a range of visual effects
    //(Ex. a texture of a brick wall with a red fragment color will result in a red brick wall)

    frag_color = fs_in.color * texture(tex, fs_in.tex_coord);
}