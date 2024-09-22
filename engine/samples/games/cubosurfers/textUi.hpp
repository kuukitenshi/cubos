#pragma once

#include "obstacle.hpp"
#include <cubos/engine/prelude.hpp>

struct TextUi
{
    CUBOS_REFLECT;

    int score{0};
};

void textUiPlugin(cubos::engine::Cubos& cubos);
