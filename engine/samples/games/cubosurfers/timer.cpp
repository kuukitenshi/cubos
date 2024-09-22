#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

#include "timer.hpp"

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Timer)
{
    return cubos::core::ecs::TypeBuilder<Timer>("Timer")
        .withField("totalTimePassed", &Timer::totalTimePassed)
        .withField("timeConst", &Timer::timeConst)
        .build();
}

void timerPlugin(cubos::engine::Cubos& cubos)
{
    cubos.resource<Timer>();
}