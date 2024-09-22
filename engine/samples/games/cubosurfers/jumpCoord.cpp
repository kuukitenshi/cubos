#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

#include "jumpCoord.hpp"
#include "jetpack.hpp"
#include "player.hpp"

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(JumpCoord)
{
    return cubos::core::ecs::TypeBuilder<JumpCoord>("JumpCoord")
        .withField("playerY", &JumpCoord::playerY)
        .build();
}

void jumpCoordPlugin(cubos::engine::Cubos& cubos)
{
    cubos.resource<JumpCoord>();
}