#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// A function to generate noise
float noise(vec2 uv) {
    // Generate a random number based on the integer part of the coordinates and the seed
    return fract(sin(dot(uv, vec2(12, 78))) * 43758);
    // Return the random number
}

void main() {
    frag_color = texture(tex, tex_coord);

    // create red vector
    vec4 red = vec4(frag_color.r, frag_color.g * 0.5, frag_color.b * 0.5, frag_color.a);

    // Generate a random noise value based on the texture coordinates
    float n = noise(tex_coord);

    // Use the noise value to randomly alter the grayscale value of the pixel
    frag_color.rgb = vec3(red + n * 0.15 - 0.1);
}