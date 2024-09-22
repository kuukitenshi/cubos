#include "armor.hpp"
#include "player.hpp"

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

CUBOS_REFLECT_IMPL(Armor)
{
    return cubos::core::ecs::TypeBuilder<Armor>("Armor").build();
}

void armorPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(inputPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(collisionsPlugin);
    
    cubos.depends(playerPlugin);
    cubos.depends(obstaclePlugin);
    
    cubos.component<Armor>();

    cubos.system("add armor to player") // quando colide adiciona a armor, se nao tiver
        .with<Armor>()
        .call([](Commands cmds, Query<Entity, const Player&, const CollidingWith&, const Obstacle&> collisions) {
            for (auto [ent, player, collidingWith, obstacle] : collisions)
            {
                if(!player.hasArmor)
                    cmds.add(ent, Armor{});
            }
        });

    cubos.system("remove armor from player") // quando colide remove a armor, se tiver
        .with<Armor>()
        .call([](Commands cmds, Query<Entity, const Player&, const CollidingWith&, const Obstacle&> collisions) {
            for (auto [ent, player, collidingWith, obstacle] : collisions)
            {
                if(player.hasArmor)
                    cmds.remove<Armor>(ent);
            }
        });
}