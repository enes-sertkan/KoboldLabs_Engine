#ifndef _CAMERA_HPP_
#pragma once
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/euler_angles.hpp>

class Scene;


struct Camera {
    glm::vec3 position;   // Camera position in world space
    glm::vec3 rotation;   // Euler angles (pitch, yaw, roll) in degrees
    glm::vec2 resolution;
    bool nightMode=false;
    Scene* scene;


    float fov=60.f;
    float nearClip;
    float farClip;
    bool isPerspective;
    float drawDistance= 750.f;

    // Constructor
    Camera(glm::vec3 startPosition, glm::vec3 startRotation, glm::vec2 res, bool perspective = true)
        : position(startPosition), rotation(startRotation), resolution(res),
        isPerspective(perspective), fov(45.0f), nearClip(0.1f), farClip(1000.0f) {}


   
    // DO WE NEED THIS?
    //// Get View Matrix (rotation is applied without a target position)
    //glm::mat4 getViewMatrix() const {
    //    glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
    //    glm::vec3 front = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); // Forward direction
    //    glm::vec3 up = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)); // Up direction
    //    return glm::lookAt(position, position + front, up);
    //}

    //// Get Projection Matrix
    //glm::mat4 getProjectionMatrix() const {
    //    return isPerspective
    //        ? glm::perspective(glm::radians(fov), resolution.x, nearClip, farClip)
    //        : glm::ortho(-10.0f * resolution.x, 10.0f * resolution.y, -10.0f, 10.0f, nearClip, farClip);
    //}
};


#endif // !_CAMERA_HPP_
