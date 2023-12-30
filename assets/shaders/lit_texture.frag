#version 330 core

//TODO: (Light) Implement Lit Textured Fragment Shader

in Varyings {
   vec4 color;
   vec2 tex_coord;
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
   vec3 emissive;
   float shininess;
};

struct TexturedMaterial{
   sampler2D albedo_map;
   sampler2D specular_map;
   sampler2D ambient_occlusion_map; 
   sampler2D roughness_map;
   sampler2D emissive_map;
   vec2 roughness_range; 
   vec3 albedo_tint;
   vec3 specular_tint;
   vec3 emissive_tint;
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

uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;
uniform TexturedMaterial tex_material;
uniform sampler2D tex;

out vec4 frag_color;

uniform vec4 tint;

void main(){
    ////////////////////////////////////////////////////////////////////////////////////////////
    //Normalize normal and view vectors
	//Normalize function returns a vector with the same direction as its parameter, v, but with length 1
   vec3 normal = normalize(fsin.normal);
   vec3 view = normalize(fsin.view);

   //ensuring that the light sources doesn't exceed the maximum count
   int count = min(light_count, MAX_LIGHT_COUNT);
   //creating an instance of material to sample from the textures according to the tex_coord
   Material material;
   //albedo is used to set the value of diffuse
   material.diffuse = tex_material.albedo_tint * texture(tex_material.albedo_map, fsin.tex_coord).rgb;
   //specular is used to set the value of specular
   material.specular = tex_material.specular_tint * texture(tex_material.specular_map, fsin.tex_coord).rgb;
   //emissive is used to set the value of emissive
   material.emissive = tex_material.emissive_tint * texture(tex_material.emissive_map, fsin.tex_coord).rgb;
   //ambient occlusion is used to set the value of ambient to allow for the occlusion of darker areas
   material.ambient = material.diffuse * texture(tex_material.ambient_occlusion_map, fsin.tex_coord).r;
   //roughness is used to set the value of specular power
   float roughness = mix(tex_material.roughness_range.x, tex_material.roughness_range.y, 
                           texture(tex_material.roughness_map, fsin.tex_coord).r);
   material.shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;
   //setting the value of emissive with material.emissive
   vec3 emissive = material.emissive;
   //starting the light with emissive value so as when their is no light the emissive is rendered correctly
   vec3 accumulated_light = emissive;

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
      vec3 diffuse = material.diffuse * light.ambient * lambert;
      vec3 specular = material.specular * light.ambient * phong;
      vec3 ambient = material.ambient * light.ambient;
      //accumulated_light += (diffuse + specular + emissive) * attenuation;
      //accumulated_light = tex_material.albedo_tint;
      //taking attenuation factor into consideration
      accumulated_light += (diffuse + specular) * attenuation + ambient;
   }
   //final light of the pixel
   frag_color = fsin.color * vec4(accumulated_light, 1.0) * texture(tex, fsin.tex_coord);//taking the texture into consideration
   //frag_color = vec4(accumulated_light, 1.0f);
    ////////////////////////////////////////////////////////////////////////////////////////////
}