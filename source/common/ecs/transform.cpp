#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //DONE (Req 3) Write this function

        // Create a 4D matrix for each transformation
        // Scaling using scale vec3 (x, y, z) from transform.hpp
        glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), scale);
        // Rotation using rotation vec3 (y(yaw), x(pitch), z(roll)) from transform.hpp
        glm::mat4 rotationM = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        // Translation using position vec3 (x, y, z) from transform.hpp
        glm::mat4 translationM = glm::translate(glm::mat4(1.0f), position);

        // Return the matrix in the order of scaling, rotation, translation
        return translationM * rotationM * scaleM;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}