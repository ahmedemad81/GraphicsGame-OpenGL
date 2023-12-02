#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL; // If fragment_depth ≤ depth_buffer_depth, the fragment passes the depth test and is considered visible.
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;        // c = α cs + (1 − α) cd
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //DONE (Req 4) Write this function
            // You should use the functions glEnable, glDisable, glCullFace, glFrontFace, 
            // glDepthFunc, glBlendFunc, glBlendEquation, glColorMask and glDepthMask

            // Enable/Disable Face Culling
            if (faceCulling.enabled) {
                glEnable(GL_CULL_FACE);
            } else {
                glDisable(GL_CULL_FACE);
            }
            // Set Face Culling
            glCullFace(faceCulling.culledFace);
            // Set Front Face 
            glFrontFace(faceCulling.frontFace);

            // Enable/Disable Depth Testing 
            if (depthTesting.enabled) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
            // Set Depth Function
            glDepthFunc(depthTesting.function);

            // Enable/Disable Blending
            if (blending.enabled) {
                glEnable(GL_BLEND);
            } else {
                glDisable(GL_BLEND);
            }
            // Set Blending Function 
            // Ex. glBlendFunc(GLenum sfactor, GLenum dfactor)
            glBlendFunc(blending.sourceFactor, blending.destinationFactor);

            // Set Blending Equation 
            // Ex. glBlendEquation(GLenum mode)
            glBlendEquation(blending.equation);
            
            // Set Blending Color 
            // Ex. glBlendColor(GLfloat red,GLfloat green,GLfloat blue, GLfloat alpha)
            glBlendColor(blending.constantColor.r, blending.constantColor.g, blending.constantColor.b, blending.constantColor.a);

            // Color Mask
            // Ex. glColorMask(GLboolean red,GLboolean green,GLboolean blue, GLboolean alpha)
            // glColorMask is used to enable or disable writing to the color buffer.
            // If disabled, no color values are written to the color buffer regardless of drawing operations attempted.
            glColorMask(colorMask.r, colorMask.g, colorMask.b, colorMask.a);

            // Depth Mask
            // Ex. glDepthMask(GLboolean flag)
            // glDepthMask is used to enable or disable writing into the depth buffer.
            glDepthMask(depthMask);
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}