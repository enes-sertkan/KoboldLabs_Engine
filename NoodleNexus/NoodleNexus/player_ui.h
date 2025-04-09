#ifndef PLAYER_UI
#define PLAYER_UI
#pragma once

#include "imgui/imgui.h"  
#include "player.h"

namespace PlayerUI {
    inline void Initialize() {
        // Set up custom UI style
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.35f);
    }

    inline void RenderHPBar(const Player& player) {
        const float currentHP = player.GetHealth();
        const float maxHP = player.GetMaxHealth();
        const float ratio = currentHP / maxHP;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 window_pos = ImVec2(viewport->WorkPos.x + 150, viewport->WorkPos.y + 30);

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_AlwaysAutoResize;

        // Optional: No border or rounding on the window itself
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background

        if (ImGui::Begin("##PlayerHPBar", nullptr, flags)) {
            const ImVec2 barSize(350, 20);
            const float rounding = 6.0f;

            // Health color based on ratio
            ImVec4 healthColor = (ratio > 0.6f) ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) :
                (ratio > 0.3f) ? ImVec4(1.0f, 0.8f, 0.2f, 1.0f) :
                ImVec4(0.8f, 0.1f, 0.1f, 1.0f);

            // Position before the progress bar
            ImVec2 cursorPos = ImGui::GetCursorScreenPos();

            // Draw the outline box behind
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddRect(
                cursorPos,
                ImVec2(cursorPos.x + barSize.x, cursorPos.y + barSize.y),
                IM_COL32(0, 0, 0, 255), // Black
                rounding,
                0,
                10.0f // thickness
            );

            // Draw the health bar
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, healthColor);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
            ImGui::ProgressBar(ratio, barSize, "");
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }

        ImGui::End();
        ImGui::PopStyleVar(2);
    }


    inline void Shutdown() {
        // Cleanup handled by main ImGui shutdown
    }
}

#endif PLAYER_UI