#ifndef _CAMERA_HPP_


#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/euler_angles.hpp>

struct Camera {
    glm::vec3 position;   // Camera position in world space
    glm::vec3 rotation;   // Rotation in Euler angles (pitch, yaw, roll)

    float fov;            // Field of View (for perspective projection)
    float aspectRatio;    // Aspect ratio (width / height)
    float nearClip;       // Near clipping plane
    float farClip;        // Far clipping plane
    bool isPerspective;   // True = perspective, False = orthographic

    // Constructor to initialize camera parameters
    Camera(glm::vec3 startPosition, glm::vec3 startRotation, float aspect, bool perspective = true)
        : position(startPosition), rotation(startRotation), aspectRatio(aspect), isPerspective(perspective),
        fov(45.0f), nearClip(0.1f), farClip(100.0f)
    {}

    // Returns the View Matrix (converts rotation to direction vectors)
    glm::mat4 getViewMatrix() const {
        glm::mat4 rotationMatrix = glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
        glm::vec3 front = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); // Forward direction
        glm::vec3 up = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)); // Up direction
        return glm::lookAt(position, position + front, up);
    }

    // Returns the Projection Matrix
    glm::mat4 getProjectionMatrix() const {
        if (isPerspective) {
            return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        }
        else {
            float orthoSize = 10.0f;
            return glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, nearClip, farClip);
        }
    }
};


#endif // !_CAMERA_HPP_
