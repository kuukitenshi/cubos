#pragma once

#include <cubos/engine/assets/asset.hpp>
#include <cubos/engine/prelude.hpp>
#include <cubos/engine/scene/scene.hpp>

struct Spawner
{
    CUBOS_REFLECT;

    cubos::engine::Asset<cubos::engine::Scene> scene; // Scene which will be spawned
    std::string sceneRoot;                            // Root entity of the scene
    float period{1.0F};                               // Time between spawns in seconds
    float laneWidth{1.0F};                            // Width of the lane
    float accumulator{0.0F};                          // Time accumulator

    float accPowerUpArmor{0.0F};                         
    float periodPowerUpArmor{3.4F};

    float accPowerUpJetpack{0.0F};                 
    float periodPowerUpJetpack{4.8F};                     
};

void spawnerPlugin(cubos::engine::Cubos& cubos);
