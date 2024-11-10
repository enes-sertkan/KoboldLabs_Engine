#pragma once
#include "UsableItem.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class GranadeLauncher : public UsableItem
{
public:
    glm::vec3 target;
    glm::vec3 position;
    glm::vec3 velocity;
    float initialSpeed = 5.0f;
    bool exploded = false;
    float explosionSize = 0.0f;
    float explosionSpeed = 5.0f;
    std::vector<glm::vec3> trailPositions;

    Object* grenadeModel = nullptr;
    Object* trailModel = nullptr;
    Object* explosionModel = nullptr;

    GranadeLauncher()
    {
        std::vector<Object*> sceneObjects;
        grenadeModel = scene->GenerateMeshObjectsFromObject(
            "assets/models/Sphere_radius_xyz_n_uv.ply", position, 1.0f, glm::vec3(0.0f),
            true, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), true, sceneObjects);
        trailModel = scene->GenerateMeshObjectsFromObject(
            "assets/models/Sphere_radius_xyz_n_uv.ply", position, 0.2f, glm::vec3(0.0f),
            false, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), true, sceneObjects);
        explosionModel = scene->GenerateMeshObjectsFromObject(
            "assets/models/Sphere_radius_xyz_n_uv.ply", position, 1.0f, glm::vec3(0.0f),
            true, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), false, sceneObjects);
    }

    void Use() override
    {
        position = scene->fCamera->getEyeLocation();
        glm::vec3 direction = glm::normalize(target - position);
        velocity = direction * initialSpeed;
        Update(0.016f);
    }

    void Update(float deltaTime)
    {
        if (exploded) return;

        velocity *= 1.02f;
        position += velocity * deltaTime;
        grenadeModel->mesh->positionXYZ = position;
        trailPositions.push_back(position);
        if (trailPositions.size() > 10) {
            trailPositions.erase(trailPositions.begin());
        }

        for (size_t i = 0; i < trailPositions.size(); ++i) {
            trailModel->mesh->positionXYZ = trailPositions[i];
        }

        if (glm::distance(position, target) < 0.1f) {
            Explode();
        }
    }

    void Explode()
    {
        exploded = true;
        explosionSize += explosionSpeed;
        explosionModel->mesh->uniformScale = explosionSize;
        trailModel->mesh->positionXYZ = glm::vec3(0.f);
        createImpactMarker(position);
    }

    void RenderModel(Object* obj, glm::vec3 position, float size = 1.0f)
    {
        obj->mesh->positionXYZ = position; // Position should be glm::vec3
        obj->mesh->uniformScale = size;    // Scale should be a float
    }

    void Render()
    {
        if (!exploded) {
            RenderModel(grenadeModel, position); // Pass position (glm::vec3) and size (float)
        }

        for (const auto& trailPos : trailPositions) {
            RenderModel(trailModel, trailPos); // Pass trail position (glm::vec3)
        }

        if (exploded) {
            RenderModel(explosionModel, position, explosionSize); // Pass explosion size and position
        }
    }

    void createImpactMarker(const glm::vec3& position)
    {
        RenderModel(trailModel, position, 0.1f);  // Render the impact marker with position and size
    }
};