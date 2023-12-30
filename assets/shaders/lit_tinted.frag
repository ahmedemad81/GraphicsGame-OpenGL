#version 330 core

//TODO: (Light) Implement Lit Tinted Fragment Shader

in Varyings {
   vec4 color;
   //The vertex position relative to the world space
   vec3 world;
   //vector from the vertex to the eye relative to the world space
   vec3 view;
   //normal on the surface relative to the world space
   vec3 normal;
} fsin;

struct Material {
   vec3 diffuse;
   vec3 specular;
   vec3 ambient;
   float shininess;
};

struct Light {
   int type; //0 point, 1 directional, 2 spot
   //Phong model (ambient, diffuse, specular)
   vec3 diffuse;
   vec3 specular;
   vec3 ambient;
   //Position  -> for spot and point light types
	//Direction -> for spot and directional light types
   vec3 position, direction;
   //attenuation -> used for spot and point light types
	//intensity of the light is affected by this equation -> 1/(a + b*d + c*d^2)
	//where a is attenuation_constant, b is attenuation_linear and c is attenuation_quadratic
   float attenuation_constant;
   float attenuation_linear;
   float attenuation_quadratic;
   //For spot light -> to define the inner and outer cones of spot light
	//For the space that lies between outer and inner cones, light intensity is interpolated
   float inner_angle, outer_angle;
};

#define TYPE_POINT          0
#define TYPE_DIRECTIONAL    1
#define TYPE_SPOT           2
#define MAX_LIGHT_COUNT     16

//vector of all lights
uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;
uniform Material material;
uniform float alpha;

out vec4 frag_color;

//uniform vec4 tint;
//uniform sampler2D tex;

void main(){
    ////////////////////////////////////////////////////////////////////////////////////////////
   //Normalize normal and view vectors
	//Normalize function returns a vector with the same direction as its parameter, v, but with length 1
   vec3 normal = normalize(fsin.normal);
   vec3 view = normalize(fsin.view);

   //ensuring that the light sources doesn't exceed the maximum count
   int count = min(light_count, MAX_LIGHT_COUNT);
   vec3 accumulated_light = vec3(0.0);

   //looping over the light sources
   for(int index = 0; index < count; index++){
      Light light = lights[index];
      vec3 light_direction;
      //set initial value for attenuation as no attenuation in directional light
      float attenuation = 1;
      if(light.type == TYPE_DIRECTIONAL)
         light_direction = light.direction;
      else {
         //for point and spot lights we calculate the light direction
         light_direction = fsin.world - light.position;
         //length function returns sqrt(x[0]^2 + x[1]^2 + ......);
         float distance = length(light_direction);
         //getting unit vector that has the same direction of the light
         light_direction /= distance;
         //calculating flactuations in intensity due to the distance from light source
         attenuation *= 1.0f / (light.attenuation_constant +
                        light.attenuation_linear * distance +
                        light.attenuation_quadratic * distance * distance);
         if(light.type == TYPE_SPOT){
            //for spot lights get inner and outer cone -> add thyeir effect to the attenuation
            float angle = acos(dot(light.direction, light_direction));
            attenuation *= smoothstep(light.outer_angle, light.inner_angle, angle);
         }
      }
      //reflect function takes (incident, normal) and returns the reflection direction calculated as I - 2.0 * dot(N, I) * N
      //For the function to work correctly normal vector must be normalized, thus initially we normalized the vector above
      vec3 reflected = reflect(light_direction, normal);
      //calculate lambert and phong factors
      float lambert = max(0.0f, dot(normal, -light_direction));
      float phong = pow(max(0.0f, dot(view, reflected)), material.shininess);
      //As cclor= M.ambient * I.ambient + M.diffuse * I.diffuse * lambert + M.specular * I.specular * phong
      //so color = ambient + diffuse + specular

      vec3 diffuse = material.diffuse * light.diffuse * lambert;
      vec3 specular = material.specular * light.specular * phong;
      vec3 ambient = material.ambient * light.ambient;
      //vec3 emissive = material.emissive * light.emissive;
      //accumulated_light += (diffuse + specular + emissive) * attenuation + ambient;
      //taking attenuation factor into consideration
      accumulated_light += (diffuse + specular) * attenuation + ambient;
      //accumulated_light = specular;
   }
   //final light of the pixel
   frag_color = fsin.color * vec4(accumulated_light, alpha);
   //frag_color = vec4(dot(view, normal));
   //frag_color = vec4(accumulated_light, 1.0f);
    ////////////////////////////////////////////////////////////////////////////////////////////
}