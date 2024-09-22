#include "enemy.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Enemy)
{
    return cubos::core::ecs::TypeBuilder<Enemy>("Enemy").build();
}

void enemyPlugin(cubos::engine::Cubos& cubos)
{
    cubos.component<Enemy>();
}