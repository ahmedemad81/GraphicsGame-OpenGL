#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a check board where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//DONE (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];


void main(){
  // Get the normalized position of the current pixel in the window 
  vec2 normalizedPosition = gl_FragCoord.xy / float(size); 

  // Get the color of the current pixel. 
  // The mod is used to determine if the pixel is even or odd. 
  bool isEven = mod(int(normalizedPosition.x) + int(normalizedPosition.y), 2) == 0;

  // if the pixel is even, set the color to colors[0], otherwise set it to colors[1]
  // colors[0] is the top-left most tile
  // colors[1] is the 2 tiles adjacent to it
  // The alpha value should be 1.0 (fully opaque)
  frag_color = isEven ? vec4(colors[0],1.0) : vec4(colors[1],1.0);

}