#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //DONE (Req 7) Write this function
        pipelineState.setup(); // Setup the pipeline state that was implemented in pipeline-state.hpp
        shader->use();         // Use the shader that was implemented in shader.hpp
    }

    void LitMaterial::setup() const 
    {
        Material::setup();

        /*TODO (req Light): SEND NEEDED DATA TO SHADER*/
    }

    void LitMaterial::deserialize(const nlohmann::json& data)
    {
        Material::deserialize(data);
        if(!data.is_object()) return;
        diffuse = data.value("diffuse", glm::vec4(1.0f));
        specular = data.value("specular", glm::vec4(1.0f));
        ambient = data.value("ambient", glm::vec4(1.0f));
        emissive = data.value("emissive", glm::vec4(1.0f));

        shininess = data.value("shininess", 1.0f);
    }


    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    void TintedMaterial::setup() const {
        //DONE (Req 7) Write this function        
        // Call the setup function of the parent class
        Material::setup();               
        
        // Set the "tint" uniform to the value in the member variable tint. 
        // This is done by calling the set function of vec4 type of the shader class 
        shader->set("tint", tint);     

    }

    void LitTintedMaterial::setup() const 
    {
        LitMaterial::setup();

        /*TODO (req Light): SEND NEEDED DATA TO SHADER*/
    }

    void LitTintedMaterial::deserialize(const nlohmann::json& data)
    {
        LitMaterial::deserialize(data);
        if(!data.is_object()) return;
        albedo_tint = data.value("albedo_tint", glm::vec4(1.0f));
        specular_tint = data.value("specular_tint", glm::vec4(1.0f));
        emissive_tint = data.value("emissive_tint", glm::vec4(1.0f));
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    void TexturedMaterial::setup() const {
        //DONE (Req 7) Write this function

        // Call the setup function of the parent class
        TintedMaterial::setup();

        // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
        // This is done by calling the set function of float type of the shader class
        shader->set("alphaThreshold", alphaThreshold);

        // Activate the texture unit which will be used to bind the texture and sampler to it
        // void glActiveTexture(GLenum texture);
        // texture: Specifies which texture unit to make active.
        // texture must be one of GL_TEXTUREi, where i ranges from zero to the value of GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS minus one. 
        // The initial value is GL_TEXTURE0.
        glActiveTexture(GL_TEXTURE0);

        // Bind the texture to the texture unit, if no texture is found, unbind the texture
        if (texture) {
            texture->bind();
        }
        else {
            Texture2D::unbind();
        }

        // Bind the sampler to the texture unit, if no sampler is found, unbind the sampler
        if (sampler) {
            // This method binds this sampler to the given texture unit GL_TEXTURE0
            sampler->bind(0);
        }
        else {
            // This static method ensures that no sampler is bound to the given texture unit GL_TEXTURE0
            Sampler::unbind(0);
        }

        // Set the "tex" uniform to the value of the texture unit number
        // This is done by calling the set function of int type of the shader class
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LitTexturedMaterial::setup() const 
    {
        LitTintedMaterial::setup();

        /*TODO (req Light): SEND NEEDED DATA TO SHADER*/
    }

    void LitTexturedMaterial::deserialize(const nlohmann::json& data)
    {
        LitTintedMaterial::deserialize(data);
        if(!data.is_object()) return;

        albedo_map = AssetLoader<Texture2D>::get(data.value("albedo_map", ""));
        albedo_sampler = AssetLoader<Sampler>::get(data.value("albedo_sampler", ""));
        specular_map = AssetLoader<Texture2D>::get(data.value("specular_map", ""));
        specular_sampler = AssetLoader<Sampler>::get(data.value("specular_sampler", ""));
        roughness_map = AssetLoader<Texture2D>::get(data.value("roughness_map", ""));
        roughness_sampler = AssetLoader<Sampler>::get(data.value("roughness_sampler", ""));
        roughness_range = data.value("roughness_range", glm::vec2(0.0f, 1.0f)); 
        ambient_occlusion_map = AssetLoader<Texture2D>::get(data.value("ambient_occlusion_map", ""));
        ambient_occlusion_sampler = AssetLoader<Sampler>::get(data.value("ambient_occlusion_sampler", ""));      
        emissive_map = AssetLoader<Texture2D>::get(data.value("emissive_map", ""));
        emissive_sampler = AssetLoader<Sampler>::get(data.value("emissive_sampler", ""));

        alphaThreshold = data.value("alphaThreshold", 0.0f);
    }

}