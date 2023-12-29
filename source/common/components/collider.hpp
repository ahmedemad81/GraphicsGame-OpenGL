#pragma once

#include "../ecs/component.hpp"
#include "../asset-loader.hpp"

// #include "light.hpp"  // CHECK

namespace our {
    class Collider : public Component {
    public:
        float Radius;
        static std::string getID() { return "Collider"; }

        // Receives the mesh & material from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}
