#include "spawner.hpp"
#include "armor.hpp"
#include "jetpack.hpp"
#include "obstacle.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

#include <cubos/engine/collisions/shapes/box.hpp>
#include <cubos/engine/voxels/grid.hpp>
#include <cubos/engine/render/voxels/plugin.hpp>
#include <cubos/engine/render/voxels/grid.hpp>
#include <cubos/engine/collisions/collider.hpp>
#include <cubos/engine/collisions/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Spawner)
{
    return cubos::core::ecs::TypeBuilder<Spawner>("Spawner")
        .withField("scene", &Spawner::scene)
        .withField("sceneRoot", &Spawner::sceneRoot)
        .withField("period", &Spawner::period)
        .withField("laneWidth", &Spawner::laneWidth)
        .withField("accumulator", &Spawner::accumulator)
        .withField("accPowerUpArmor", &Spawner::accPowerUpArmor)
        .withField("periodPowerUpArmor", &Spawner::periodPowerUpArmor)
        .withField("accPowerUpJetpack", &Spawner::accPowerUpJetpack)
        .withField("periodPowerUpJetpack", &Spawner::periodPowerUpJetpack)
        .build();
}

static const Asset<VoxelGrid> voxelArmor = AnyAsset("fd89da0f-72d2-4ef9-9297-f73ff2ca5c63");
static const Asset<VoxelGrid> voxelJetpack = AnyAsset("732498a6-7675-4efa-86ef-8f35d3bec485");

Position genSpawnPos(float& acc, float& period, Position& position, Spawner& spawner)
{
    acc -= period;
    int offset = (rand() % 3) - 1;
    Position spawnPosition = position;
    spawnPosition.vec.x += static_cast<float>(offset) * spawner.laneWidth;
    return spawnPosition;
}

void spawnerPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(assetsPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(collisionsPlugin);
    cubos.depends(renderVoxelsPlugin);
    cubos.depends(obstaclePlugin);

    cubos.depends(jetpackPlugin);

    cubos.component<Spawner>();

    cubos.system("spawn stuff")
        .call([](Commands commands, const DeltaTime& dt, Assets& assets, Query<Spawner&, Position&> spawners) {
            for (auto [spawner, position] : spawners)
            {
                spawner.accumulator += dt.value();
                spawner.accPowerUpArmor += dt.value();
                spawner.accPowerUpJetpack += dt.value();

                if (spawner.accumulator >= spawner.period)
                {
                    Position spawnPosition = genSpawnPos(spawner.accumulator, spawner.period, position, spawner);
                    commands.spawn(assets.read(spawner.scene)->blueprint).add(spawner.sceneRoot, spawnPosition);
                }
                if(spawner.accPowerUpArmor >= spawner.periodPowerUpArmor)
                {
                    Position spawnPosition = genSpawnPos(spawner.accPowerUpArmor, spawner.periodPowerUpArmor, position, spawner);
                    commands.create()
                        .add(Armor{})
                        .add(Obstacle{glm::vec3{0.0F, 0.0F, -100.0F}})
                        .add(spawnPosition)
                        .add(BoxCollisionShape{cubos::core::geom::Box{glm::vec3{3.0F, 7.0F, 2.0F}}})
                        .add(Rotation{})
                        .add(Collider{})
                        .add(RenderVoxelGrid{voxelArmor, glm::vec3{-4.0F, 0.0F, 0.0F}});
                }
                if(spawner.accPowerUpJetpack >= spawner.periodPowerUpJetpack)
                {
                    Position spawnPosition = genSpawnPos(spawner.accPowerUpJetpack, spawner.periodPowerUpJetpack, position, spawner);
                    commands.create()
                        .add(Jetpack{})
                        .add(Obstacle{glm::vec3{0.0F, 0.0F, -100.0F}})
                        .add(spawnPosition)
                        .add(BoxCollisionShape{cubos::core::geom::Box{glm::vec3{3.0F, 7.0F, 2.0F}}})
                        .add(Rotation{})
                        .add(Collider{})
                        .add(RenderVoxelGrid{voxelJetpack, glm::vec3{-3.0F, 0.0F, 0.0F}});
                }
            }
        });
}
