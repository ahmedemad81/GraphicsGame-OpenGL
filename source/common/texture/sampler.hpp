#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

namespace our {

    // This class defined an OpenGL sampler
    class Sampler {
        // The OpenGL object name of this sampler 
        GLuint name;
    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name" 
        Sampler() {
            //DONE (Req 6) Complete this function
            // Create a sampler object
            //glGenSamplers( GLsizei n, GLuint *samplers)
            // n : Specifies the number of sampler object names to generate.
            // samplers : Specifies an array in which the generated sampler object names are stored.
            glGenSamplers(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler() { 
            //DONE (Req 6) Complete this function
            // Delete the sampler object
            // glDeleteSamplers( GLsizei n, const GLuint *samplers)
            // n : Specifies the number of sampler objects to be deleted.
            // samplers : Specifies an array of sampler objects to be deleted.
            glDeleteSamplers(1, &name);
        }

        // This method binds this sampler to the given texture unit
        void bind(GLuint textureUnit) const {
            //DONE (Req 6) Complete this function
            // Bind the sampler object to the given texture unit
            // glBindSampler( GLuint unit, GLuint sampler)
            // unit : Specifies the index of the texture unit to which the sampler is bound.
            // sampler : Specifies the name of a sampler object to bind to the texture unit specified by unit.
            glBindSampler(textureUnit, name);
        }

        // This static method ensures that no sampler is bound to the given texture unit
        static void unbind(GLuint textureUnit){
            //DONE (Req 6) Complete this function
            // Bind the sampler object to the given texture unit
            // glBindSampler( GLuint unit, GLuint sampler)
            // unit : Specifies the index of the texture unit to which the sampler is bound.
            // sampler : Specifies the name of a sampler object to bind to the texture unit specified by unit. 
            // When sampler is zero, the default sampler is bound to the specified texture unit.
            glBindSampler(textureUnit, 0);
        }

        // This function sets a sampler paramter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        // Parameters being set includes GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_FILTER to set the filtering parameters and GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T to set the wrapping parameters
        // For example, To set GL_TEXTURE_MAG_FILTER parameter, the parameter argument should be GL_TEXTURE_MAG_FILTER and the value argument should be GL_LINEAR
        void set(GLenum parameter, GLint value) const {
            //DONE (Req 6) Complete this function
            // Set the sampler parameter
            // glSamplerParameteri( GLuint sampler, GLenum pname, GLint param)
            // sampler : Specifies the sampler object whose parameter to modify.
            // pname : Specifies the symbolic name of a single-valued sampler parameter.
            // param : Specifies the value of pname.
            glSamplerParameteri(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        void set(GLenum parameter, GLfloat value) const {
            //DONE (Req 6) Complete this function
            // Set the sampler parameter
            // glSamplerParameterf( GLuint sampler, GLenum pname, GLfloat param)
            // sampler : Specifies the sampler object whose parameter to modify.
            // pname : Specifies the symbolic name of a single-valued sampler parameter.
            // param : Specifies the value of pname.
            glSamplerParameterf(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter
        void set(GLenum parameter, glm::vec4 value) const {
            glSamplerParameterfv(name, parameter, &(value.r));
        }

        // Given a json object, this function deserializes the sampler state
        void deserialize(const nlohmann::json& data);

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
    };

}