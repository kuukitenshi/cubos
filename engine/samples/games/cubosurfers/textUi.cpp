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
#include "score.hpp"

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(TextUi)
{
    return cubos::core::ecs::TypeBuilder<TextUi>("textUi").build();
}

void textUiPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(settingsPlugin);
    cubos.depends(windowPlugin);
    cubos.depends(renderTargetPlugin);
    cubos.depends(imguiPlugin);

    cubos.depends(scorePlugin);
    
    cubos.depends(playerPlugin);

    cubos.system("show ImGui score").tagged(imguiTag)
        .call([](Query<Player&> query, Score& score) {
        
        ImGui::Begin("Info Window");
        ImGui::Text("Score: %d\n", score.score);
        for(auto [player] : query)
        {
            if (player.hasArmor) 
                ImGui::Text(">> Oh an Armor!\nNow u can kys :)");
            if (player.hasJetpack)
                ImGui::Text(">> Congrats for the Jetpack!\nFly lil bird o.o\n\nDuration: %.2fs", score.jetpackDuration);
        }
        ImGui::SetWindowSize(ImVec2(253, 150));
        ImGui::End();
    });
}
