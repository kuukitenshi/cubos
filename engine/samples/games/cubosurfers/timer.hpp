#pragma once

#include <cubos/engine/prelude.hpp>

struct Timer
{
    CUBOS_REFLECT;

    float totalTimePassed{0.0F};
    float timeConst{0.7F};
};

void timerPlugin(cubos::engine::Cubos& cubos);
