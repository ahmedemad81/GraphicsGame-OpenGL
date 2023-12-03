#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Vignette is a postprocessing effect that darkens the corners of the screen
// to grab the attention of the viewer towards the center of the screen

void main(){
    //DONE (Req 11) Modify this shader to apply vignette
    // To apply vignette, divide the scene color
    // by 1 + the squared length of the 2D pixel location the NDC space
    // Hint: remember that the NDC space ranges from -1 to 1
    // while the texture coordinate space ranges from 0 to 1
    // We have the pixel's texture coordinate, how can we compute its location in the NDC space?

    // The length function calculates the distance from the center of the NDC space (0, 0) to the transformed texture coordinate. 
    // This distance represents how far the current pixel is from the center.
    // Squaring emphasizes the difference from the center and increases the effect towards the corners.
    // Adding 1 ensures that the denominator is always greater than 1.

    // To transform from texture coordinate space to NDC space, we can multiply by 2 and subtract 1
    vec3 vignette_color = texture(tex, tex_coord).rgb / (1 + pow(length(tex_coord * 2 - 1), 2));

    // The fragment color is of type vec4 to set the alpha value to 1
    frag_color = vec4(vignette_color, 1);
}