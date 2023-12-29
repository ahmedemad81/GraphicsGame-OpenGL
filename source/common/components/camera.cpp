#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //Done: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt
        
        // We need to multiply vectors by the transformation Matrix
        // To multiply the 4D Matrix we will use a vec3 (coordinated,w)
        // Cast back te vec3 as it is the input for lookAt function (Ignore w)
        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(M*glm::vec4(0,0,0,1)),glm::vec3(M*glm::vec4(0,0,-1,1)),glm::vec3(M*glm::vec4(0,1,0,0)));
        return viewMatrix;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //Done: (Req 8) Write this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective

        // Aspect ratio is the ratio of the width to the height of the viewport
        float aspectRatio = (float)viewportSize.x /viewportSize.y;
        
        // Check if the camera is orthographic or perspective
        // If it is orthographic, we need to compute the left, right, bottom and up values by the above rules (camera.hpp -> orthoHeight)
        // Orthographic camera doesn't depend on the fovY value as it assumes that all the objects are away from the camera with the same width as the camera is at infinite distance from the scene
        // If it is perspective, we will use values from the camera.hpp file 
        // Perspective camera depends on the fovY value as it assumes that the objects are closer to the camera will seem larger than objects far away (more realistic/ human eye)
         if(cameraType == CameraType::ORTHOGRAPHIC){
            float orthoWidth = (float)orthoHeight * aspectRatio;
            return glm::ortho(-orthoWidth/2, orthoWidth/2, -orthoHeight/2, orthoHeight/2, near, far);
        } else{
            // if (cameraType == CameraType::PERSPECTIVE)
            return glm::perspective(fovY, aspectRatio, near, far);
        }
    }
}