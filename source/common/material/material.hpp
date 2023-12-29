#pragma once

#include "pipeline-state.hpp"
#include "../texture/texture2d.hpp"
#include "../texture/sampler.hpp"
#include "../shader/shader.hpp"

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <json/json.hpp>

namespace our
{

    // This is the base class for all the materials
    // It contains the 3 essential components required by any material
    // 1- The pipeline state when drawing objects using this material
    // 2- The shader program used to draw objects using this material
    // 3- Whether this material is transparent or not
    // Materials that send uniforms to the shader should inherit from the is material and add the required uniforms
    class Material
    {
    public:
        PipelineState pipelineState;
        ShaderProgram *shader;
        bool transparent;

        // This function does 2 things: setup the pipeline state and set the shader program to be used
        virtual void setup() const;
        // This function read a material from a json object
        virtual void deserialize(const nlohmann::json &data);
    };

    class LitMaterial : public Material
    {
    public:
        glm::vec4 diffuse;
        glm::vec4 specular;
        glm::vec4 ambient;
        glm::vec4 emissive;

        float shininess;

        // This function does 2 things: setup the pipeline state and set the shader program to be used
        virtual void setup() const;
        // This function read a material from a json object
        virtual void deserialize(const nlohmann::json& data);
    };

    // This material adds a uniform for a tint (a color that will be sent to the shader)
    // An example where this material can be used is when the whole object has only color which defined by tint
    class TintedMaterial : public Material
    {
    public:
        glm::vec4 tint;

        void setup() const override;
        void deserialize(const nlohmann::json &data) override;
    };

    class LitTintedMaterial : public LitMaterial
    {
    public:
        glm::vec4 albedo_tint;
        glm::vec4 specular_tint;
        glm::vec4 emissive_tint;

        void setup() const override;
        void deserialize(const nlohmann::json& data) override;
    };
    // This material adds two uniforms (besides the tint from Tinted Material)
    // The uniforms are:
    // - "tex" which is a Sampler2D. "texture" and "sampler" will be bound to it.
    // - "alphaThreshold" which defined the alpha limit below which the pixel should be discarded
    // An example where this material can be used is when the object has a texture
    class TexturedMaterial : public TintedMaterial
    {
    public:
        Texture2D *texture;
        Sampler *sampler;
        float alphaThreshold;

        void setup() const override;
        void deserialize(const nlohmann::json &data) override;
    };
    class LitTexturedMaterial : public LitTintedMaterial
    {
        Texture2D* texture;
        Sampler* sampler;
        Texture2D* albedo_map;
        Sampler* albedo_sampler;
        Texture2D* specular_map;
        Sampler* specular_sampler;
        Texture2D* roughness_map;
        Sampler* roughness_sampler;
        glm::vec2 roughness_range; 
        Texture2D* ambient_occlusion_map; 
        Sampler* ambient_occlusion_sampler;          
        Texture2D* emissive_map;
        Sampler* emissive_sampler;

        float alphaThreshold;

        void setup() const override;
        void deserialize(const nlohmann::json& data) override;
    };

    // This function returns a new material instance based on the given type
    inline Material *createMaterialFromType(const std::string &type)
    {
        if (type == "tinted_lit")
        {
            return new LitTintedMaterial();
        }
        else if (type == "textured_lit")
        {
            return new LitTexturedMaterial();
        }
        else if (type == "lit")
        {
            return new LitMaterial();
        }
        else if (type == "tinted")
        {
            return new TintedMaterial();
        }
        else if (type == "textured")
        {
            return new TexturedMaterial();
        }
        else
        {
            return new Material();
        }
    }

}