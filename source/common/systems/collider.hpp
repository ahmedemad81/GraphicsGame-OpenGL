#pragma once

#include <vector>
#include<iostream>
#include <glm/glm.hpp>

#include "../ecs/world.hpp"
#include "../ecs/component.hpp"
#include "../components/collider.hpp"
#include "../application.hpp"


int health = 2;

using namespace std;
namespace our
{

    class ColliderSystem {
        Application* app;
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            vector<Collider*> Colliders; // Initialize vector to save all colliders


            Entity * player = nullptr;
            for(auto entity : world->getEntities()){
                auto collider = entity->getComponent<Collider>();
                if(collider) // if collider exists , push it to vector
                {
                    Colliders.push_back(collider);
                }
                if(entity->name == "player")
                {
                    player = entity;
                }
            }
            for (auto collider1 : Colliders)
			{
				for (auto collider2 : Colliders)
				{
					if (collider1 != collider2)
					{
                        // Get name of each collider
						auto collider1_name = collider1->getOwner()->name; 
						auto collider2_name = collider2->getOwner()->name;

                        // Calculate center of each collider
						auto collider1_center = collider1->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
						auto collider2_center = collider2->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
						
                        // Calculate absolute distance between each collider center
						auto dist = abs(glm::distance(collider1_center, collider2_center));
                        
            
                        // If distance between each collider center is less than sum of their radius, then they are colliding
						if (dist <= collider1->Radius + collider2->Radius) 
						{
                            // Destroy monster who got hit by sword
                            if(collider1_name=="sword" && collider2_name=="monster") 
                            {
                                world->markForRemoval(collider2->getOwner());
                                world->deleteMarkedEntities();
                            }
                            
                            // If player collides with monster, player loses health
                            if(collider1_name=="player" && collider2_name=="monster") 
                            {
                                if (health == 2)
                                {
                                    health -= 1;
                                    world->markForRemoval(collider2->getOwner());
                                    world->deleteMarkedEntities(); // delete the monster who hit the player
                                    app->changeState("injured"); // change state to injured
                                }
                                else if (health == 1)
                                {
                                    app->changeState("lose"); // change state to lose
                                }

                            }

                            // If player collides with skull, player loses health
                            if (collider1_name == "player" && collider2_name == "skull")
                            {
                                if (health == 2)
                                {
                                    health -= 1;
                                    world->markForRemoval(collider2->getOwner());
                                    world->deleteMarkedEntities(); // delete the monster who hit the player
                                    app->changeState("injured"); // change state to injured
                                }
                                else if (health == 1)
                                {
                                    app->changeState("lose"); // change state to lose
                                }

                            } 
					    }
                        // If player collides with plane, player goes up again
                        if (collider1_name == "player" && collider2_name == "plane")
                        {
                            if (player->localTransform.position.y - 1.5 <= collider2->getOwner()->localTransform.position.y)
                            {
                                player->localTransform.position.y += 0.3f;
                            }
                            
                        }
                        
				    }
			    }
            }
        };
    };
}