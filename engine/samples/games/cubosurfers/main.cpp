#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/defaults/plugin.hpp>
#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/render/lights/environment.hpp>
#include <cubos/engine/render/voxels/palette.hpp>
#include <cubos/engine/scene/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

#include "obstacle.hpp"
#include "player.hpp"
#include "spawner.hpp"
#include "armor.hpp"
#include "textUi.hpp"
#include "enemy.hpp"
#include "jetpack.hpp"
#include "timer.hpp"
#include "score.hpp"
#include "jumpCoord.hpp"

#include <cubos/engine/collisions/shapes/box.hpp>
#include <cubos/engine/voxels/grid.hpp>
#include <cubos/engine/render/voxels/plugin.hpp>
#include <cubos/engine/render/voxels/grid.hpp>
#include <cubos/engine/collisions/collider.hpp>
#include <cubos/engine/collisions/plugin.hpp>

using namespace cubos::engine;

static const Asset<Scene> SceneAsset = AnyAsset("ee5bb451-05b7-430f-a641-a746f7009eef");
static const Asset<VoxelPalette> PaletteAsset = AnyAsset("101da567-3d23-46ae-a391-c10ec00e8718");
static const Asset<InputBindings> InputBindingsAsset = AnyAsset("b20900a4-20ee-4caa-8830-14585050bead");

static const Asset<VoxelGrid> voxelPlayerBase = AnyAsset("57d1b886-8543-4b8b-8f78-d911e9c4f896");

int main()
{
    Cubos cubos{};

    cubos.plugin(defaultsPlugin);

    cubos.plugin(enemyPlugin);
    cubos.plugin(timerPlugin);
    cubos.plugin(jumpCoordPlugin);

    cubos.plugin(playerPlugin);
    cubos.plugin(scorePlugin);
    cubos.plugin(obstaclePlugin);

    cubos.plugin(armorPlugin);
    cubos.plugin(jetpackPlugin);

    cubos.plugin(spawnerPlugin);
    cubos.plugin(textUiPlugin);

    cubos.startupSystem("configure settings").tagged(settingsTag).call([](Settings& settings) {
        settings.setString("assets.io.path", SAMPLE_ASSETS_FOLDER);
    });

    cubos.startupSystem("set the palette, environment, input bindings and spawn the scene")
        .tagged(assetsTag)
        .call([](Commands commands, const Assets& assets, RenderPalette& palette, Input& input, RenderEnvironment& environment) {
            palette.asset = PaletteAsset;
            environment.ambient = {0.1F, 0.1F, 0.1F};
            environment.skyGradient[0] = {0.2F, 0.4F, 0.8F};
            environment.skyGradient[1] = {0.6F, 0.6F, 0.8F};
            input.bind(*assets.read(InputBindingsAsset));
            commands.spawn(assets.read(SceneAsset)->blueprint);
        });

    cubos.system("restart the game on input")
        .call([](Commands cmds, const Assets& assets, const Input& input, Query<Entity> all, Score& score, Timer& timer) {
            if (input.justPressed("restart"))
            {
                timer.totalTimePassed = 0.0F;
                for (auto [ent] : all)
                {
                    score.score = 0;
                    cmds.destroy(ent);
                }
                cmds.spawn(assets.read(SceneAsset)->blueprint);
            }
        });

    //--------------------------------------------COLLISIONS ZOMBIES--------------------------------
    cubos.system("player collide vs zombie")
        .call([](Commands cmds, const Assets& assets, Query<Entity, Player&, const RenderVoxelGrid&, const CollidingWith&, const Enemy&, Entity> collisions, Query<Entity> all, Score& score, Timer& timer) {
            for (auto [entPlayer, player, voxel, collidingWith, enemy, zombEnt] : collisions)
            {
                if(player.hasArmor)
                {
                    player.hasArmor = false;
                    cmds.remove<Armor>(entPlayer);
                    cmds.remove<RenderVoxelGrid>(entPlayer);
                    cmds.add(entPlayer, RenderVoxelGrid{voxelPlayerBase, voxel.offset});
                    cmds.destroy(zombEnt);
                }
                else
                {
                    timer.totalTimePassed = 0.0F;
                    for (auto [ent] : all)
                    {
                        score.score = 0;
                        cmds.destroy(ent);
                    }
                    cmds.spawn(assets.read(SceneAsset)->blueprint);
                    (void)player;
                }
            }
        });

    //-----------------------------------------------------------------------------------------------------<

    cubos.system("speed up osbstacles through the time")
        .call([](Query<Obstacle&> obstacles, const DeltaTime& dt, Timer& timer) {
            for (auto [obstacle] : obstacles)
            {
                timer.totalTimePassed += dt.value();
                obstacle.velocity.z = -100.0F - (timer.totalTimePassed * timer.timeConst); //speed up the obstacles through the time
                // CUBOS_INFO("---> Speeding: {}", obstacle.velocity.z);
            }
        });

    cubos.run();
}

