// aTheseus.hpp
#pragma once
#include "Action.h"
#include "MazeGenerator.hpp"

class aTheseus : public Action {
public:
    glm::ivec2 gridPosition;
    MazeGenerator* maze = nullptr;

    void Update() override {
        // Movement logic here (handle keyboard input)
    }
};
