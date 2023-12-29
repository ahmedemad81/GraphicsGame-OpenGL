#include "collider.hpp"

namespace our
{
    // Reads light parameters from the given json object
    void Collider::deserialize(const nlohmann::json &data)
    {
        if(!data.is_object()) return;

        Radius = data.value("Radius", 1.0f);
    }

}