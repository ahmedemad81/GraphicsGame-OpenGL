#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

void main()
{
    // The fog factor is the distance between the camera and the fragment
    // divided by the fog distance
    float fogFactor = tex_coord.y /2+ tex_coord.x / 2.5;

    // Reddish fog color
    vec3 fogColor = vec3(1.0, 0.3, 0.3);

    // Sample the scene color
    vec3 sceneColor = texture(tex, tex_coord).rgb;

    // Mix the scene color with the fog color based on the fog factor
    vec3 finalColor = mix(sceneColor, fogColor, fogFactor);

    // Output the final color
    frag_color = vec4(finalColor, 1.0);
}