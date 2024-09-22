#include "jetpack.hpp"
#include "player.hpp"
#include "obstacle.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/defaults/plugin.hpp>
#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/render/lights/environment.hpp>
#include <cubos/engine/render/voxels/palette.hpp>
#include <cubos/engine/scene/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>
#include <cubos/engine/collisions/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Jetpack)
{
    return cubos::core::ecs::TypeBuilder<Jetpack>("Jetpack")
        .withField("duration", &Jetpack::duration)
        .withField("remaingTime", &Jetpack::remainingTime)
        .build();
}

void jetpackPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(inputPlugin);
    cubos.depends(collisionsPlugin);
    cubos.depends(transformPlugin);
    
    cubos.depends(playerPlugin);
    cubos.depends(obstaclePlugin);
    
    cubos.component<Jetpack>();

    cubos.system("add jetpack to player") // quando colide adiciona jetpack, se nao tiver
        .with<Jetpack>()
        .call([](Commands cmds, Query<Entity, const Player&, const CollidingWith&, const Obstacle&> collisions) {
            for (auto [ent, player, collidingWith, obstacle] : collisions)
            {
                if(!player.hasJetpack)
                    cmds.add(ent, Jetpack{});
            }
        });

    cubos.system("remove jetpack from playe, expire") // o jetpack q expira, se tiver
        .with<Jetpack>()
        .call([](Commands cmds, Query<Entity, const Player&, const Jetpack&> query) {
            for (auto [ent, player, jetpack] : query)
            {
                if(jetpack.remainingTime <= 0)
                    cmds.remove<Jetpack>(ent);
            }
        });
}