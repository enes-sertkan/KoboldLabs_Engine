#pragma once

#include "GLCommon.h"

#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "cLightManager.h"
#include "Scene.hpp"

extern cBasicFlyCamera* g_pFlyCamera;

extern cLightManager* g_pLightManager;
extern unsigned int g_selectedLightIndex;
//extern Scene* currentScene;


// GLFW callback function signatures
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
// Mouse entering and leaving the window
void cursor_enter_callback(GLFWwindow* window, int entered);
// Mouse button...
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// Mouse scroll wheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// WE call these (not GLFW)
void handleKeyboardAsync(GLFWwindow* window,Object* screen_quad,  Scene* scene);
void handleMouseAsync(GLFWwindow* window);

// Can call this from anything that has this header file
void DrawDebugSphere(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program, Scene* scene);

extern bool g_bShowDebugSpheres;

glm::vec3 g_rgb_from_HTML(unsigned int red, unsigned int green, unsigned int blue);

glm::vec3 moveTowards(const glm::vec3& point, const glm::vec3& endPoint, float speed);
glm::vec2 moveTowards2(const glm::vec2& point, const glm::vec2& endPoint, float speed);

// HACK: Show "targeting LASER"
extern bool g_bShowLASERBeam;