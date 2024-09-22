#pragma once

#include <cubos/engine/prelude.hpp>

struct Score
{
    CUBOS_REFLECT;

    int score{0};
    float jetpackDuration{3.0F};
};

void scorePlugin(cubos::engine::Cubos& cubos);