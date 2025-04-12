#pragma once
#include "Action.h"
#include <map>
#include "aTurretPlacer.h"
#include "aPlayerShooting.h"
#include "aTargetedParticleEmitter .h"
#include "aTurretDestroyer.h"

class aToolManager : public Action {
private:
    struct ToolInfo {
        Action* action;
        std::string modelPath;
        Object* visual;
    };

    std::map<int, ToolInfo> tools;  // Maps number keys (1-9) to tool info
    int activeToolKey = -1;

public:
    void RegisterTool(int numberKey, Action* toolAction, const std::string& visualModelPath) {
        if (numberKey >= GLFW_KEY_1 && numberKey <= GLFW_KEY_9) {
            ToolInfo info;
            info.action = toolAction;
            info.modelPath = visualModelPath;
            info.visual = nullptr;
            tools[numberKey - GLFW_KEY_1] = info;
        }
    }
    aTargetedParticleEmitter* particleEmmitter = nullptr;

    void Start() override {
        // Create all tool visuals
        for (std::map<int, ToolInfo>::iterator it = tools.begin(); it != tools.end(); ++it) {
            ToolInfo& toolInfo = it->second;

            // Create visual object
            toolInfo.visual = object->scene->GenerateMeshObjectsFromObject(
                toolInfo.modelPath,
                glm::vec3(0.0f), // Local position relative to parent
                0.1f,            // Scale
                glm::vec3(0.0f,90.f,0.f),
                false,            // Override color
                glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), // Orange color
                false,
                object->scene->sceneObjects
            );

            if (aTurretDestroyer* destroyer = dynamic_cast<aTurretDestroyer*>(toolInfo.action))
            {
                toolInfo.visual->mesh->textures[0] = "ToolDestroyer/ToolDestroyer_albedo.bmp";
                toolInfo.visual->mesh->AOtexture = "ToolDestroyer/ToolDestroyer_AO.bmp";
                toolInfo.visual->mesh->STTexture = "ToolDestroyer/ToolDestroyer_ST.bmp";

                object->useRotationMatrix = true;
                // Set as child and hide initially
                object->AddChild(toolInfo.visual);
                toolInfo.visual->isActive = false;
                toolInfo.visual->mesh->positionXYZ = glm::vec3(0.45f, -0.15f, 0.15f);
                toolInfo.visual->mesh->rotationEulerXYZ = glm::vec3(26.5f, -150.f, 20.5f);
                toolInfo.visual->mesh->uniformScale = 1.f;
                toolInfo.visual->startTranform->position = glm::vec3(0.0f, 0.0f, 0.1f); 

                    Object* connectionEmiiter = object->scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", glm::vec3(0) , 1.f, glm::vec3(0.f), false, glm::vec4(0.5f, 0.4f, 0.4f, 1.f), true, object->scene->sceneObjects);

                    toolInfo.visual->AddChild(connectionEmiiter);
                    connectionEmiiter->mesh->positionXYZ = glm::vec3(0);
                    connectionEmiiter->mesh->positionXYZ.y = 0.12f;
           
     
                    connectionEmiiter->mesh->isParticleEmitter = true;
                    connectionEmiiter->mesh->metal = 0.1;
                    connectionEmiiter->mesh->bDoNotLight = true;


               particleEmmitter = new aTargetedParticleEmitter();
                object->scene->AddActionToObj(particleEmmitter, connectionEmiiter);
                particleEmmitter->Start();
                particleEmmitter->emitterTargetPos = glm::vec3(0);
                    connectionEmiiter->name = "SMOKE PARTICLES";
                    particleEmmitter->colorStart = glm::vec4(1, 0.647, 0.2, 1.f);;
                    particleEmmitter->colorEnd = glm::vec4(1, 0.647, 0.2, 1.f);;
                //bulletCol->particles = particleEmmitter;
                   particleEmmitter->spawnRate = 10.f;
                   particleEmmitter->spawnActive = false;
                   particleEmmitter->destroyOnNoParticles = false;
                   particleEmmitter->sizeStart = 0.065f;
              //     particleEmmitter->sizeEnd = 0.1f;
                //   particleEmmitter->destroyOnNoParticles = true;
          /*      smokeEmiiter->mesh->transperency = 0.99f;
                particleEmmitter->particlesToSpawn = 5.f;
                particleEmmitter->minDirection = glm::vec3(-0.1, 1, -0.1);
                particleEmmitter->maxDirection = glm::vec3(0.1, 1, 0.1);
                particleEmmitter->colorEnd = glm::vec4(0.6, 0.6, 0.6, 0);
                particleEmmitter->colorStart = glm::vec4(1, 0.647, 0.2, 1.f);
                particleEmmitter->velocityRange = glm::vec2(0.2f, 1.f);
         
                particleEmmitter->lifeTimeRange = glm::vec2(0.7, 1.5f);*/


                   if (aTurretDestroyer* destroyer = dynamic_cast<aTurretDestroyer*>(toolInfo.action)) 
                       destroyer->particles = particleEmmitter;
                   
            }
        }

        SwitchToTool(0);
    }

    void Update() override {
        // Check number keys 1-9
        for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; key++) {
            if (glfwGetKey(object->scene->window, key)) {
                int toolIndex = key - GLFW_KEY_1;
                SwitchToTool(toolIndex);
                break;
            }
        }
    }

private:
    void SwitchToTool(int toolIndex) {
        if (toolIndex == activeToolKey) return;

        if (particleEmmitter)
            particleEmmitter->spawnActive = false;
        // Deactivate all tools and hide visuals
        for (std::map<int, ToolInfo>::iterator it = tools.begin(); it != tools.end(); ++it) {
            ToolInfo& info = it->second;

            if (aTurretPlacer* placer = dynamic_cast<aTurretPlacer*>(info.action)) {
                placer->SetActive(false);
            }
            else {
                info.action->isActive = false;
            }

            if (info.visual) {
                info.visual->isActive = false;
            }
        }

        // Activate selected tool and show visual
        std::map<int, ToolInfo>::iterator found = tools.find(toolIndex);
        if (found != tools.end()) {
            ToolInfo& info = found->second;

            if (aTurretPlacer* placer = dynamic_cast<aTurretPlacer*>(info.action)) {
                placer->SetActive(true);
            }
            else {
                info.action->isActive = true;
            }

            if (info.visual) {
                info.visual->isActive = true;
                // Position in front of player (adjust as needed)
           


            }

            activeToolKey = toolIndex;
        }
    }
};