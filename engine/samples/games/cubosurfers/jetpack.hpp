#pragma once

#include <cubos/engine/prelude.hpp>

struct Jetpack
{
    CUBOS_REFLECT;

    float duration{5.0F};
    float remainingTime{5.0F};
};

void jetpackPlugin(cubos::engine::Cubos& cubos);
