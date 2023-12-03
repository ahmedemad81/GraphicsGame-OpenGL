#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

#include <iostream>
using namespace std;

namespace our
{

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json &config)
    {
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if (config.contains("sky"))
        {
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram *skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();

            // DONE (Req 10) Pick the correct pipeline state to draw the sky
            // Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            // We will draw the sphere from the inside, so what options should we pick for the face culling.
            PipelineState skyPipelineState{};
            skyPipelineState.depthTesting.enabled = true;
            skyPipelineState.depthTesting.function = GL_LEQUAL;
            skyPipelineState.faceCulling.enabled = true;
            skyPipelineState.faceCulling.culledFace = GL_FRONT;
            skyPipelineState.faceCulling.frontFace = GL_CCW;

            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky
            Sampler *skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if (config.contains("postprocess"))
        {
            // DONE (Req 11) Create a framebuffer
            // Framebuffer is used to perform additional image processing on the rendered scene after the initial rendering to the screen.
            // The rendered results are stored in a framebuffer object (FBO) rather than being immediately displayed on the screen.
            // void glGenFramebuffers(	GLsizei n, GLuint *ids);
            // n : Specifies the number of framebuffer object names to generate.
            // ids : Specifies an array in which the generated framebuffer object names are stored.
            glGenFramebuffers(1, &postprocessFrameBuffer);

            // DONE (Req 11) Create a color and a depth texture and attach them to the framebuffer
            // Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            // The depth format can be (Depth component with 24 bits).
            
            // Bind the framebuffer using the bind method of the Texture2D class
            // void glBindFramebuffer(GLenum target, GLuint framebuffer);
            // target : Specifies the target to which the framebuffer object is bound. (Bind to GL_FRAMEBUFFER)
            // framebuffer : Specifies the framebuffer object name.
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);

            // Create a color texture
            colorTarget = texture_utils::empty(GL_RGBA, windowSize);
            
            // Depth texture is used to store depth information for each pixel in a scene. 
            // It contains depth values that represent the distance of objects from the viewer (camera).

            // Create a depth texture
            depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT, windowSize);

            // Attach the color texture to the framebuffer
            // void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
            // target : Specifies the target to which the framebuffer object is bound. (Bind to GL_FRAMEBUFFER)
            // attachment : Specifies the attachment point of the framebuffer. (GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT)
            // textarget : Specifies the type of texture to attach. (GL_TEXTURE_2D)
            // texture : Specifies the texture object to attach.
            // level : Specifies the mipmap level of the texture image to attach.
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);


            // DONE (Req 11) Unbind the framebuffer just to be safe
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // By using Vertex Arrays and Vertex Buffers together, you can efficiently manage and organize vertex data, 
            // reducing redundant state changes. The VAO helps encapsulate the setup and configuration of vertex attributes, 
            // while VBOs store the actual data.

            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler *postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram *postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy()
    {
        // Delete all objects related to the sky
        if (skyMaterial)
        {
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if (postprocessMaterial)
        {
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World *world)
    {
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent *camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        for (auto entity : world->getEntities())
        {
            // If we hadn't found a camera yet, we look for a camera in this entity
            if (!camera)
                camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
            {
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if (command.material->transparent)
                {
                    transparentCommands.push_back(command);
                }
                else
                {
                    // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if (camera == nullptr)
            return;

        // DONE: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one

        // We know that the view matrix contains the camera forward vector w
        // Extracting w from the view matrix will be extracting first 3 columns in the third row counting from 1
        glm::mat4 ViewMatrix = camera->getViewMatrix();
        glm::vec3 cameraForward = glm::vec3(ViewMatrix[2][0], ViewMatrix[2][1], ViewMatrix[2][2]);
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand &first, const RenderCommand &second)
                  {
            //DONE: (Req 9) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second".
            // Distance from the camera to the center of the first object is greater than the distance from the camera to the center of the second object
            // We use distance from the camera to the center of the object to sort the transparent objects
            // The z component isn't enough to sort the transparent objects as the z component is the distance from the camera to the object center in the camera space
            return glm::distance(first.center, cameraForward) > glm::distance(second.center, cameraForward); });

        // DONE: (Req 9) Get the camera ViewProjection matrix and store it in VP
        //  As it was already calculated (View Matrix)
        glm::mat4 VP = camera->getProjectionMatrix(windowSize) * ViewMatrix;

        // DONE: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        //  The view port start is the bottom left corner of the window (0,0)
        //  The viewport size is the window size
        glViewport(0, 0, windowSize.x, windowSize.y);

        // DONE: (Req 9) Set the clear color to black and the clear depth to 1
        //  Black color is rgba 0001
        glClearColor(0, 0, 0, 1);
        glClearDepth(1.0);

        // DONE: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        //  To allow color blending from rgba channels we will set all the color mask true
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        // Render opaque objects only
        glDepthMask(GL_TRUE);

        // If there is a postprocess material, bind the framebuffer
        if (postprocessMaterial)
        {
            // DONE (Req 11) bind the framebuffer
            // void glBindFramebuffer(GLenum target, GLuint framebuffer);
            // target : Specifies the target to which the framebuffer object is bound. (Bind to GL_FRAMEBUFFER)
            // framebuffer : Specifies the framebuffer object name.
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);
        }

        // TODO: (Req 9) Clear the color and depth buffers
        //  Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);
        // Clear the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        // TODO: (Req 9) Draw all the opaque commands
        //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (unsigned long int i = 0; i < opaqueCommands.size(); i++)
        {
            opaqueCommands[i].material->transparent = false;
            opaqueCommands[i].material->setup();
            opaqueCommands[i].material->shader->set("transform", VP * opaqueCommands[i].localToWorld);
            opaqueCommands[i].mesh->draw();
        }

        // If there is a sky material, draw the sky
        if (this->skyMaterial)
        {
            // DONE (Req 10) setup the sky material
            skyMaterial->setup();

            // DONE (Req 10) Get the camera position
            // Position of the camera component (last column)
            glm::vec3 cameraPos = camera->getOwner()->getLocalToWorldMatrix()[3];

            // DONE (Req 10) Create a model matrix for the sky such that it always follows the camera (sky sphere center = camera position)
            glm::mat4 skyModelMatrix = glm::translate(glm::mat4(1.0f), cameraPos);

            // DONE (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
            // We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?

            // It is orthographic projection matrix with an additional translation applied to shift the entire scene to the far plane in NDC.
            // The first two rows maintain the x and y coordinates without any changes.
            // The third row removes the z-coordinate, flattening all points to the z=0 plane.
            // The fourth row shifts the entire scene along the positive z-axis to z=1 (far plane) in NDC.
            // It is used for rendering background elements like a skybox, ensuring that it is drawn behind all other objects in the scene.
            glm::mat4 alwaysBehindTransform = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f);
            // DONE (Req 10) set the "transform" uniform
            glm::mat4 transformMatrix = alwaysBehindTransform * VP * skyModelMatrix;
            skyMaterial->shader->set("transform", transformMatrix);
            // DONE (Req 10) draw the sky sphere
            skySphere->draw();
        }
        // TODO: (Req 9) Draw all the transparent commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (unsigned long int i = 0; i < transparentCommands.size(); i++)
        {
            transparentCommands[i].material->transparent = true;
            transparentCommands[i].material->setup();
            transparentCommands[i].material->shader->set("transform", VP * transparentCommands[i].localToWorld);
            transparentCommands[i].mesh->draw();
        }

        // If there is a postprocess material, apply postprocessing
        if (postprocessMaterial)
        {
            // DONE (Req 11) Return to the default framebuffer
            // void glBindFramebuffer(GLenum target, GLuint framebuffer);
            // target : Specifies the target to which the framebuffer object is bound. (Bind to GL_FRAMEBUFFER)
            // framebuffer : Specifies the framebuffer object name.
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // DONE (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            // Bind the vertex array using the bind method of the Texture2D class
            glBindVertexArray(postProcessVertexArray);

            // void glDrawArrays(GLenum mode, GLint first, GLsizei count);
            // mode : Specifies what kind of primitives to render. (GL_TRIANGLES)
            // first : Specifies the starting index in the enabled arrays.
            // count : Specifies the number of indices to be rendered.
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // Unbind the vertex array using the unbind method of the Texture2D class
            glBindVertexArray(0);
        }
    }

}