#include <iostream>
#include <map>
#include <vector>

#include <imgui.h>

#include <cubos/core/ecs/reflection.hpp>

#include <cubos/core/reflection/external/map.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>
#include <cubos/core/reflection/external/vector.hpp>
#include <cubos/core/reflection/reflect.hpp>
#include <cubos/core/reflection/traits/constructible.hpp>
#include <cubos/core/reflection/traits/fields.hpp>
#include <cubos/core/reflection/traits/nullable.hpp>

#include <cubos/engine/imgui/data_inspector.hpp>
#include <cubos/engine/imgui/plugin.hpp>
#include <cubos/engine/render/target/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/window/plugin.hpp>

#include "textUi.hpp"
#include "player.hpp"

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(TextUi)
{
    return cubos::core::ecs::TypeBuilder<TextUi>("textUi")
        .withField("score", &TextUi::score)
        .build();
}

void textUiPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(settingsPlugin);
    cubos.depends(windowPlugin);
    cubos.depends(renderTargetPlugin);
    cubos.depends(imguiPlugin);

    cubos.depends(playerPlugin);

    cubos.resource<TextUi>();

   cubos.system("show ImGui score").tagged(imguiTag)
        .call([](TextUi& text, Query<Player&> query) {
        
        ImGui::Begin("Info Window");
        ImGui::Text("Score: %d \n", text.score);
        for(auto [player] : query)
        {
            if (player.hasArmor)
                ImGui::Text("Congrats u have an Armor! \nNow u can kys :)\n");
            if (player.hasJetpack)
                ImGui::Text("Congrats u have a Jetpack! \nFly lil bird :)\n");
        }
        ImGui::SetWindowSize(ImVec2(250, 130));
        ImGui::End();
    });
}
