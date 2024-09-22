#include "jetpack.hpp"
#include "player.hpp"
#include "obstacle.hpp"
#include "score.hpp"
#include "jumpCoord.hpp"
#include "armor.hpp"

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

    cubos.depends(jumpCoordPlugin);

    cubos.depends(scorePlugin);
    cubos.depends(playerPlugin);
    cubos.depends(obstaclePlugin);

    cubos.depends(armorPlugin);
    
    cubos.component<Jetpack>();

    cubos.system("collides vs jetpack // adds to player")
        .call([](Commands cmds, Query<Entity, Player&, Position&, const RenderVoxelGrid&, CollidingWith&, const Jetpack&, Entity> collisions, Score& score) {
            for (auto [entPlayer, player, playerPos, voxel, collidingWith, jetpack, jetpackEnt] : collisions)
            {
                if(!player.hasJetpack)
                {
                    player.hasJetpack = true;
                    score.jetpackDuration = 3.0F; //TIME JETPACK DEFAULT

                    cmds.add(entPlayer, Jetpack{});
                    cmds.remove<RenderVoxelGrid>(entPlayer);
                    cmds.add(entPlayer, RenderVoxelGrid{voxelPlayerJetpack, voxel.offset});

                    if(player.hasArmor)
                    {
                        player.hasArmor = false;
                        cmds.remove<Armor>(entPlayer);
                    }
                }
                cmds.destroy(jetpackEnt);
            }
        });

    cubos.system("remove jetpack from player, expire")
        .with<Jetpack>()
        .call([](Commands cmds, const DeltaTime& dt, Query<Entity, Player&, Position&, const RenderVoxelGrid&, Jetpack&> query, Score& score) {
            for (auto [entPlayer, player, playerPos, voxel, jetpack] : query)
            {
                jetpack.duration -= dt.value();
                score.jetpackDuration = jetpack.duration;
                if(jetpack.duration <= 0)
                {
                    player.hasJetpack = false;
                    cmds.remove<Jetpack>(entPlayer);
                    cmds.remove<RenderVoxelGrid>(entPlayer);
                    cmds.add(entPlayer, RenderVoxelGrid{voxelPlayerBase, voxel.offset});
                }
            }
        });
        
    cubos.system("increase Y player > gets jetpack")
        .with<Jetpack>()
        .call([](Query<Player&> query, const DeltaTime& dt, JumpCoord& jumpCoord) {
            for(auto [player] : query)
            {
                if(jumpCoord.playerY < 18.0F)
                {
                    jumpCoord.playerY += 20.0F * dt.value();
                }
            }
        });


    cubos.system("decrease Y player > expire jetpack")
        .without<Jetpack>()
        .call([](Query<Player&> query, const DeltaTime& dt, JumpCoord& jumpCoord) {
            for(auto [player] : query)
            {
                if(jumpCoord.playerY > 0.0F)
                {
                    jumpCoord.playerY -= 20.0F * dt.value();
                }
            }
        });
}