#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//TODO: (Light) Implement light component cpp

namespace our
{

    // Reads light parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json& data)
    {
        if (!data.is_object())
            return;

        std::string lightTypeStr = data.value("lightType", "directional");
        if(lightTypeStr == "spot"){
            lightType = LightType::SPOT;
            printf("%d \n", LightType::SPOT);
        } else if(lightTypeStr == "point"){
            lightType = LightType::POINT;
            printf("%d \n", LightType::POINT);
        } else {
            lightType = LightType::DIRECTIONAL;
            printf("%d \n", LightType::DIRECTIONAL);
        }

        diffuse = data.value("diffuse", glm::vec3(1.0f));
        specular = data.value("specular", glm::vec3(1.0f));  
        ambient = data.value("ambient", glm::vec3(1.0f));

        attenuation_constant = data.value("attenuation_constant", 0.0f);
        attenuation_linear = data.value("attenuation_linear", 0.0f);
        attenuation_quadratic = data.value("attenuation_quadratic", 1.0f);

        inner_angle = data.value("inner_angle", glm::quarter_pi<float>());
        outer_angle = data.value("outer_angle", glm::half_pi<float>());
    }
}