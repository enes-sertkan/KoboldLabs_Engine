#include "sharedThings.h"

cBasicFlyCamera* g_pFlyCamera = NULL;

//cLightManager* g_pLightManager = NULL;
unsigned int g_selectedLightIndex = 0;
bool g_bShowDebugSpheres = true;

glm::vec3 g_rgb_from_HTML(unsigned int red, unsigned int green, unsigned int blue)
{
	return glm::vec3((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f);
}


glm::vec3 moveTowards(const glm::vec3& point, const glm::vec3& endPoint, float speed) {
    // Calculate the vector from point to endPoint
    glm::vec3 direction = endPoint - point;

    // Get the distance to the endPoint
    float distance = glm::length(direction);

    // If the point is close enough, return the endPoint
    if (distance <= speed) {
        return endPoint;
    }

    // Normalize the direction vector and move the point towards the endPoint
    glm::vec3 normalizedDirection = glm::normalize(direction);
    return point + normalizedDirection * speed;
}

glm::vec2 moveTowards2(const glm::vec2& point, const glm::vec2& endPoint, float speed) {
    // Calculate the vector from point to endPoint
    glm::vec2 direction = endPoint - point;

    // Get the distance to the endPoint
    float distance = glm::length(direction);

    // If the point is close enough, return the endPoint
    if (distance <= speed) {
        return endPoint;
    }

    // Normalize the direction vector and move the point towards the endPoint
    glm::vec2 normalizedDirection = glm::normalize(direction);
    return point + normalizedDirection * speed;
}

// HACK: Show "targeting LASER"
bool g_bShowLASERBeam = true;