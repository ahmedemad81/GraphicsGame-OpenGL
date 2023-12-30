#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class MovementSystem {
    public:

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();

                // Player Position
                glm::vec3 playerPos = {0, 0, 10};

                for (auto entity : world->getEntities())
                {
                    if (entity->name == "player")
                    {
                        playerPos = entity->localTransform.position;
                    }
                }

                // Move Monsters in the direction of the player
                if (movement)
                {
                    if (entity->name == "monster")
                    {
                        // Get the direction from the zombie to the player
                        auto direction = (playerPos - entity->localTransform.position);
                        // Normalize the direction
                        direction = normalize(direction);
                        // Move the zombie in the direction of the player
                        entity->localTransform.position += deltaTime * direction * 3.0f;
                        // Rotate the zombie to look at the player
                        auto angle = atan2(direction.x, direction.z);
                        entity->localTransform.rotation = glm::vec3(0, angle, 0);
                    }
                    if (entity->name == "skull")
                    {
                        // Get the direction from the zombie to the player
                        auto direction = (playerPos - entity->localTransform.position);
                        // Normalize the direction
                        direction = normalize(direction);
                        // Rotate the zombie to look at the player
                        auto angle = atan2(direction.x, direction.z);
                        entity->localTransform.rotation = glm::vec3(-90, angle, 0);
                    }
                }

            }
        }

    };

}
