#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

#include "score.hpp"

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Score)
{
    return cubos::core::ecs::TypeBuilder<Score>("score")
        .withField("score", &Score::score)
        .withField("jetpackDuration", &Score::jetpackDuration)
        .build();
}

void scorePlugin(cubos::engine::Cubos& cubos)
{
    cubos.resource<Score>();
}