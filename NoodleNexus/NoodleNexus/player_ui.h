#ifndef PLAYER_UI
#define PLAYER_UI
#pragma once

#include "imgui/imgui.h"  
#include "player.h"

enum UICorner {
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3
};



namespace PlayerUI {
    inline void Initialize() {
        // Set up custom UI style
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.35f);
    }

    inline ImVec2 GetCornerPosition(int corner, const ImVec2& size, int slotIndex = 0, float padding = 70.0f, float spacing = 10.0f) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 pos = viewport->WorkPos;

        switch (corner) {
        case TopLeft:
            return ImVec2(pos.x + padding, pos.y + padding + (size.y + spacing) * slotIndex);
        case TopRight:
            return ImVec2(pos.x + viewport->WorkSize.x - size.x - padding, pos.y + padding + (size.y + spacing) * slotIndex);
        case BottomLeft:
            return ImVec2(pos.x + padding, pos.y + viewport->WorkSize.y - size.y - padding - (size.y + spacing) * slotIndex);
        case BottomRight:
            return ImVec2(pos.x + viewport->WorkSize.x - size.x - padding, pos.y + viewport->WorkSize.y - size.y - padding - (size.y + spacing) * slotIndex);
        default:
            return ImVec2(pos.x + padding, pos.y + padding);
        }
    }

    inline void RenderHPBar(int health, int maxHealth) {
        const float currentHP = health;
        const float maxHP = maxHealth;
        const float ratio = currentHP / maxHP;

        ImVec2 barSize(400, 35);
        ImVec2 window_pos = GetCornerPosition(BottomLeft, barSize, 1);

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
            const ImVec2 barSize(400, 35);
            const float rounding = 6.0f;

            // Health color based on ratio
            ImVec4 healthColor = (ratio > 0.6f)
                ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f)    // Green
                : (ratio > 0.3f)
                ? ImVec4(0.7f, 0.5f, 0.1f, 1.0f)    // Orange
                : ImVec4(0.8f, 0.0f, 0.0f, 1.0f);   // Red

            // Position before the progress bar
            ImVec2 cursorPos = ImGui::GetCursorScreenPos();



            // Draw the health bar
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, healthColor);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
            ImGui::ProgressBar(ratio, barSize, "");
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();

            // Draw the outline box behind
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddRect(
                cursorPos,
                ImVec2(cursorPos.x + barSize.x, cursorPos.y + barSize.y),
                IM_COL32(20, 20 ,20 , 255), // Black
                rounding,
                0,
                5.0f // thickness
            );


        }

        ImGui::PopStyleVar(2);
    }

    inline void RenderStaminaBar(int stamina, int maxStamina) {
        const float currentStamina = stamina;
        const float maxStam = maxStamina;
        const float ratio = currentStamina / maxStam;
        

        ImVec2 barSize(300, 30);
        ImVec2 window_pos = GetCornerPosition(BottomLeft, barSize, 0);

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

        if (ImGui::Begin("##PlayerStaminaBar", nullptr, flags)) {
            const ImVec2 barSize(400, 20);
            const float rounding = 6.0f;

            // Stamina color based on ratio
            ImVec4 staminaColor = (ratio > 0.6f)
                ? ImVec4(0.6f, 0.8f, 1.0f, 1.0f)    // Icy blue
                : (ratio > 0.3f)
                ? ImVec4(0.75f, 0.9f, 1.0f, 1.0f)   // Light sky blue
                : ImVec4(0.9f, 0.97f, 1.0f, 1.0f);  // Almost white blue

            // Position before the progress bar
            ImVec2 cursorPos = ImGui::GetCursorScreenPos();



            // Draw the health bar
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, staminaColor);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
            ImGui::ProgressBar(ratio, barSize, "");
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();

            // Draw the outline box behind
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddRect(
                cursorPos,
                ImVec2(cursorPos.x + barSize.x, cursorPos.y + barSize.y),
                IM_COL32(20, 20, 20, 255), // Black
                rounding,
                0,
                5.0f // thickness
            );


        }

        ImGui::PopStyleVar(2);
    }

    inline void RenderCrosshair(float size = 10.0f, float thickness = 2.0f, ImU32 color = IM_COL32(255, 255, 255, 255)) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 center = ImVec2(viewport->GetCenter().x, viewport->GetCenter().y);

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        // Horizontal line
        drawList->AddLine(
            ImVec2(center.x - size, center.y),
            ImVec2(center.x + size, center.y),
            color,
            thickness
        );

        // Vertical line
        drawList->AddLine(
            ImVec2(center.x, center.y - size),
            ImVec2(center.x, center.y + size),
            color,
            thickness
        );
    }


    inline void Shutdown() {
        // Cleanup handled by main ImGui shutdown
    }
}

#endif PLAYER_UI