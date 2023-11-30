#pragma once

#include <glad/gl.h>

namespace our {

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //DONE (Req 5) Complete this function
            // Create a texture object ex. glGenTextures(GLsizei n, GLuint * textures);
            // n : Specifies the number of texture names to be generated. (1 as we only want one texture)
            // textures : Specifies an array in which the generated texture names are stored.
            glGenTextures(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() { 
            //DONE (Req 5) Complete this function
            // Delete the texture object ex. glDeleteTextures(GLsizei n, const GLuint * textures);
            // n : Specifies the number of textures to be deleted. (1 as we only want one texture)
            // textures : Specifies an array of texture names to be deleted. 
            glDeleteTextures(1, &name);
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName() {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //DONE (Req 5) Complete this function
            // Bind the texture object ex. glBindTexture(GLenum target, GLuint texture);
            // target : Specifies the target to which the texture is bound. (Bind to GL_TEXTURE_2D)
            // texture : Specifies the name of a texture. (Texture name to be bound)
            glBindTexture(GL_TEXTURE_2D, name);
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind(){
            //DONE (Req 5) Complete this function
            // Bind the texture object ex. glBindTexture(GLenum target, GLuint texture);
            // target : Specifies the target to which the texture is bound. 
            // texture : Specifies the name of a texture.
            // Since the name 0 refers to the default texture object, this effectively unbinds any previously bound texture from GL_TEXTURE_2D
            glBindTexture(GL_TEXTURE_2D, 0);

        }

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}