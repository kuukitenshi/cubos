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

using namespace cubos::engine;

static const Asset<Scene> SceneAsset = AnyAsset("ee5bb451-05b7-430f-a641-a746f7009eef");
static const Asset<VoxelPalette> PaletteAsset = AnyAsset("101da567-3d23-46ae-a391-c10ec00e8718");
static const Asset<InputBindings> InputBindingsAsset = AnyAsset("b20900a4-20ee-4caa-8830-14585050bead");

float totalTimePassed = 0.0f;
float const timeConst = 0.01f;

int main()
{
    Cubos cubos{};

    cubos.plugin(defaultsPlugin);

    cubos.plugin(enemyPlugin);
    cubos.plugin(playerPlugin);
    cubos.plugin(textUiPlugin);
    cubos.plugin(obstaclePlugin);

    cubos.plugin(jetpackPlugin);
    cubos.plugin(armorPlugin);

    cubos.plugin(spawnerPlugin);

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
            totalTimePassed = 0.0f;
        });

    cubos.system("restart the game on input")
        .call([](Commands cmds, const Assets& assets, const Input& input, Query<Entity> all, TextUi& text) {
            if (input.justPressed("restart"))
            {
                totalTimePassed = 0.0f;
                for (auto [ent] : all)
                {
                    text.score = 0;
                    cmds.destroy(ent);
                }
                cmds.spawn(assets.read(SceneAsset)->blueprint);
            }
        });

    cubos.system("player vs enemy collisions")
        .call([](Commands cmds, const Assets& assets, Query<Player&, const CollidingWith&, const Enemy&, Entity> collisions, Query<Entity> all, TextUi& text) {
            for (auto [player, collidingWith, enemy, zombEnt] : collisions)
            {
                CUBOS_INFO("Player collided with an obstacle without armor!"); // restart when the player colides with an obstacle
                totalTimePassed = 0.0f;
                if (player.hasArmor)
                {
                    player.hasArmor = false;
                    cmds.destroy(zombEnt);
                }
                else
                {
                    for (auto [ent] : all)
                    {
                        text.score = 0;
                        cmds.destroy(ent);
                    }
                    cmds.spawn(assets.read(SceneAsset)->blueprint);
                    (void)player; // here to shut up 'unused variable warning', you can remove it
                }
            }
        });

    cubos.system("player with armor vs enemy collisions")
        .call([](Commands cmds, Query<Player&, const CollidingWith&, const Armor&, Entity> collisions) {
            for (auto [player, collidingWith, armor, ent] : collisions)
            {
                CUBOS_INFO("Player collided with an armor!");
                if (!player.hasArmor)
                {
                    player.hasArmor = true;
                }
                cmds.destroy(ent);
            }
        });

    cubos.system("player vs jetpack collisions")
        .call([](Commands cmds, Query<Player&, const CollidingWith&, const Jetpack&, Entity> collisions) {
            for (auto [player, collidingWith, jetpack, ent] : collisions)
            {
                CUBOS_INFO("Player collided with an jetpack!");
                if (!player.hasJetpack)
                {
                    player.hasJetpack = true;
                }
                cmds.destroy(ent);
                // TODO: VOAR
                
            }
        });


    cubos.system("speed up osbstacles through the time")
        .call([](Query<Obstacle&> obstacles, const DeltaTime& dt) {
            for (auto [obstacle] : obstacles)
            {
                totalTimePassed += dt.value();
                obstacle.velocity.z += obstacle.velocity.z * totalTimePassed * dt.value() * timeConst;//speed up the obstacles through the time
                CUBOS_INFO("---> Speeding: {}", obstacle.velocity.z);
            } 
        });

    cubos.run();
}

