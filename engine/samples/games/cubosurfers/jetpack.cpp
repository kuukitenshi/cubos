#include "jetpack.hpp"
#include "player.hpp"
#include "obstacle.hpp"
#include "score.hpp"

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

CUBOS_REFLECT_IMPL(Jetpack)
{
    return cubos::core::ecs::TypeBuilder<Jetpack>("Jetpack")
        .withField("duration", &Jetpack::duration)
        .build();
}

static const Asset<VoxelGrid> voxelPlayerJetpack = AnyAsset("c7263b46-be18-47c2-b3ef-05592b2e9dec");
static const Asset<VoxelGrid> voxelPlayerBase = AnyAsset("57d1b886-8543-4b8b-8f78-d911e9c4f896");

void jetpackPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(inputPlugin);
    cubos.depends(collisionsPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(renderVoxelsPlugin);

    cubos.depends(scorePlugin);
    cubos.depends(playerPlugin);
    cubos.depends(obstaclePlugin);
    
    cubos.component<Jetpack>();

    cubos.system("collides to jetpack and adds it to player")
        .call([](Commands cmds, Query<Entity, Player&, Position&, const RenderVoxelGrid&, CollidingWith&, const Jetpack&, Entity> collisions, Score& score) {
            for (auto [entPlayer, player, playerPos, voxel, collidingWith, jetpack, jetpackEnt] : collisions)
            {
                if(!player.hasJetpack)
                {
                    player.hasJetpack = true;
                    score.jetpackDuration = 3.0F;

                    cmds.add(entPlayer, Jetpack{});
                    cmds.remove<RenderVoxelGrid>(entPlayer);
                    cmds.add(entPlayer, RenderVoxelGrid{voxelPlayerJetpack, voxel.offset});
                    

                    // FIXME:
                    playerPos.vec.y += 1000.0F;
                    CUBOS_INFO("---> Y coord: {}", playerPos.vec.y);
                }
                cmds.destroy(jetpackEnt);
            }
        });

    cubos.system("remove jetpack from player, expire")
        .with<Jetpack>()
        .call([](Commands cmds, const DeltaTime& dt, Query<Entity, Player&, Position&, Jetpack&> query, Score& score) {
            for (auto [entPlayer, player, playerPos, jetpack] : query)
            {
                jetpack.duration -= dt.value();
                score.jetpackDuration = jetpack.duration;
                if(jetpack.duration <= 0)
                {
                    // FIXME:
                    player.hasJetpack = false;
                    playerPos.vec.y = 0.0F;

                    cmds.remove<Jetpack>(entPlayer);
                    cmds.remove<RenderVoxelGrid>(entPlayer);
                    cmds.add(entPlayer, RenderVoxelGrid{voxelPlayerBase, glm::vec3{-4.0F, 0.0F, 0.0F}});
                }
            }
        });
}