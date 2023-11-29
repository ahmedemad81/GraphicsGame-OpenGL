#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0, 0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).
//TODO: (Req 1) Finish this shader

out Varyings {
  vec3 color;
} vs_out;

  // For translation and scale, we need to add two uniforms
  uniform vec2 translation = vec2(0.0, 0.0);
  uniform vec2 scale = vec2(1.0, 1.0);

void main(){

  const vec3 positions[3] = vec3[3](
    vec3(-0.5, -0.5, 0.0),
    vec3(0.5, -0.5, 0.0),
    vec3(0.0, 0.5, 0.0)
  );

  const vec3 colors[3] = vec3[3](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
  );

  // We need to transform the vertex position (only x and y) as z will be handled later (Depth testing)
  vec2 transformedPosition = (positions[gl_VertexID].xy) * scale + translation;

  // We need to set the w component to 1.0 as it is a point
  gl_Position = vec4(transformedPosition, positions[gl_VertexID].z , 1.0); 

  // Extract the color from the array
  vs_out.color = colors[gl_VertexID]; 
}