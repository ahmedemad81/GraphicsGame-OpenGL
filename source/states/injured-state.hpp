#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/collider.hpp>
#include "ecs/component.hpp"

// This state shows how to use the ECS framework and deserialization.
class Injuredstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::ColliderSystem colliderSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // We intialize the collider system
        colliderSystem.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer_injured"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);

        health = 1; // Set health to 1

        // We update the collider system
        colliderSystem.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();

        if(keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }

        // If no entites called monster exist, go to win state 
        int monster_count = 0;
        our::Entity* win_wall = nullptr;
        our::Entity* player = nullptr;
        for(auto entity : world.getEntities()){
            if (entity->name == "monster")
            {
                monster_count++;
            }
            if (entity->name == "win_wall")
            {
                win_wall = entity;
            }
            if (entity->name == "player")
            {
                player = entity;
            }
        }
        if ((win_wall->localTransform.position.z + 0.5 >= player->localTransform.position.z) && (monster_count == 0))
        {
            getApp()->changeState("win");
        }
    }

    std::string getName() override {
        return "injured";
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};