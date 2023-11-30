#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //DONE (Req 1) Create A shader program
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            //DONE (Req 1) Delete a shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //DONE (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program, name.c_str());
        }


        // Overloaded set functions to send data to the shader program with respect to the uniform type 

        void set(const std::string &uniform, GLfloat value) {
            //DONE (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            //DONE (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            //DONE (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //DONE (Req 1) Send the given 2D vector value to the given uniform
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //DONE (Req 1) Send the given 3D vector value to the given uniform
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //DONE (Req 1) Send the given 4D vector value to the given uniform
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //DONE (Req 1) Send the given matrix 4x4 value to the given uniform
            // The parameter break down:
            //  - location: the location of the uniform 
            //  - count: how many matrices we want to send , We want to send only one matrix (1)
            //  - transpose: whether we want to transpose the matrix or not (GL_FALSE) , We don't want to transpose the matrix
            //  - value_ptr(matrix): a pointer to the matrix ,We use glm::value_ptr to get a pointer to the matrix
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        //DONE (Req 1) Delete the copy constructor and assignment operator.
        //Question: Why do we delete the copy constructor and assignment operator?
        // Answer: Because we don't want to copy the shader program, we want to use the same one for all the objects 
        //  To prevent memory leaks and unnecessary duplication of the same shader program.

        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        
    };

}

#endif