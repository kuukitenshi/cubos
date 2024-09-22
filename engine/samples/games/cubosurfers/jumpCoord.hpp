#pragma once

#include <cubos/engine/prelude.hpp>

struct JumpCoord
{
    CUBOS_REFLECT;

    float playerY{0.0F};
};

void jumpCoordPlugin(cubos::engine::Cubos& cubos);
