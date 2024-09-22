#include "armor.hpp"
#include "player.hpp"
#include "enemy.hpp"

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

#include <cubos/engine/collisions/shapes/box.hpp>
#include <cubos/engine/voxels/grid.hpp>
#include <cubos/engine/render/voxels/plugin.hpp>
#include <cubos/engine/render/voxels/grid.hpp>
#include <cubos/engine/collisions/collider.hpp>
#include <cubos/engine/collisions/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Armor)
{
    return cubos::core::ecs::TypeBuilder<Armor>("Armor").build();
}

static const Asset<VoxelGrid> voxelPlayerArmor = AnyAsset("4892c2f3-10b3-4ca7-9de3-822b77a0ba7e");

void armorPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(inputPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(collisionsPlugin);
    cubos.depends(renderVoxelsPlugin);
    
    cubos.depends(playerPlugin);
    cubos.depends(obstaclePlugin);
    cubos.depends(enemyPlugin);
    
    cubos.component<Armor>();

    cubos.system("add armor to player // player vs armor collide")
        .call([](Commands cmds, Query<Entity, Player&, const RenderVoxelGrid&, const CollidingWith&, const Armor&, Entity> collisions) {
            for (auto [entPlayer, player, voxel, collidingWith, armor, entArmor] : collisions)
            {
                if(!player.hasJetpack)
                {
                    if(!player.hasArmor)
                    {
                        player.hasArmor = true;
                        cmds.add(entPlayer, Armor{});
                        cmds.remove<RenderVoxelGrid>(entPlayer);
                        cmds.add(entPlayer, RenderVoxelGrid{voxelPlayerArmor, voxel.offset});
                    }
                    cmds.destroy(entArmor);
                }
            }
        });
}