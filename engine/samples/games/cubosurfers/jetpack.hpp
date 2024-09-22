#pragma once

#include <cubos/engine/prelude.hpp>

struct Jetpack
{
    CUBOS_REFLECT;

    float duration{3.0F};
};

void jetpackPlugin(cubos::engine::Cubos& cubos);
