#version 330 core

// This shader is designed to work with "triangle.vert" and it receives an
// interpolated varying which represents the vertex color.

in Varyings {
  vec3 color;
} fs_in;

out vec4 frag_color;

// currently the shader just returns the interpolated color varying.
// However, we want to mix the color channels around. We can do this using a 
// color matrix which we will send to the shader as 3 uniforms: red, green, blue.
// Each of these 3 variables will be a vec4. To apply the channel mixing for a
// certain channel (e.g. red), we apply this linear transformation:
// frag_color.r = red.r * fs_in.color.r + red.g + fs_in.color.g + red.b + fs_in.color.b + red.a;
// However, this line is too long to write, so we can simplify it using a dot product
// (which is defined in the "dot" function).

//TODO: (Req 1) Finish this shader and apply the channel mixing using the "dot" function.

uniform vec4 red = vec4(1.0, 0.0, 0.0, 0.0);
uniform vec4 green = vec4(0.0, 1.0, 0.0, 0.0);
uniform vec4 blue = vec4(0.0, 0.0, 1.0, 0.0);

void main(){

  // Color mixing using dot product (r, g, b, a) 
  // While dot between vec3 and vec4, the vec4 is implicitly treated as a vec3 by ignoring the fourth component (alpha) 
  //frag_color = vec4(dot(red, fs_in.color), dot(green, fs_in.color), dot(blue, fs_in.color), 1.0); 
  // or
  frag_color = vec4(dot(red,vec4(fs_in.color,1.0)), dot(green,vec4(fs_in.color,1.0)), dot(blue,vec4(fs_in.color,1.0)), 1.0);
}