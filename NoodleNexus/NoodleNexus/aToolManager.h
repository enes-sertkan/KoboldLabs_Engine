#pragma once
#include "Action.h"
#include <map>
#include "aTurretPlacer.h"
#include "aPlayerShooting.h"

class aToolManager : public Action {
private:
    std::map<int, Action*> tools;  // Maps number keys (1-9) to tool actions
    int activeToolKey = -1;

public:
    void RegisterTool(int numberKey, Action* toolAction) {
        if (numberKey >= GLFW_KEY_1 && numberKey <= GLFW_KEY_9) {
            tools[numberKey - GLFW_KEY_1] = toolAction;
        }
    }
    void Start() override {
        SwitchToTool(0);
    }

    void Update() override {
        // Check number keys 1-9
        for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; key++) {
            if (glfwGetKey(object->scene->window, key)) {
                int toolIndex = key - GLFW_KEY_1;
                SwitchToTool(toolIndex);
                break; // Only handle one key press per frame
            }
        }
    }

private:
    void SwitchToTool(int toolIndex) {
        if (toolIndex == activeToolKey) return;

        // Deactivate all tools
        for (auto& pair : tools) {
            if (aTurretPlacer* placer = dynamic_cast<aTurretPlacer*>(pair.second)) {
                placer->SetActive(false);
            }
            else {
                pair.second->isActive = false;
            }
        }

        // Activate selected tool if exists
        if (tools.find(toolIndex) != tools.end()) {
            if (aTurretPlacer* placer = dynamic_cast<aTurretPlacer*>(tools[toolIndex])) {
                placer->SetActive(true);
            }
            else {
                tools[toolIndex]->isActive = true;
            }
            activeToolKey = toolIndex;
        }
    }
};