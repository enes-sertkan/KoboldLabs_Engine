#pragma once

#include "Action.h";
#include "sObject.h"

#include <iostream>
#include "Scene.hpp"
#include "cPhysics.h"
#include <glm/vec3.hpp>

void DrawDebugCube(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program, Scene* scene);

struct PhysicsData
{
    glm::vec3 position;
    glm::vec3 acceleration = glm::vec3(0,0,0);
    glm::vec3 velocity = glm::vec3(0,0,0);
    bool isStatic = false;
};

class aPlanePhysics : public Action
{
public:
 
    cPhysics* physMan = nullptr;
    unsigned int numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;

    PhysicsData physData;

    void Start() override
    {
        physData.position = object->mesh->positionXYZ;

    }
    void Update() override
    {
        if (!physMan) return;

        
        physData.position = object->mesh->positionXYZ;
        
                // The size of the AABBs that we sliced up the Galactical model in the broad phase
                const float AABBSIZE = 500.0f;

                // Using the same XYZ location in space we used for the triangle vertices,
                //  we are going to pass the location of the viper to get an ID
                //  of an AABB/Cube that WOULD BE at that location (if there was one...)
                unsigned long long hypotheticalAABB_ID
                    =   physMan->calcBP_GridIndex(
                        physData.position.x,
                        physData.position.y,
                        physData.position.z, AABBSIZE);

                // Where would that hypothetical AABB be in space
                glm::vec3 minXYZofHypotheticalCube = physMan->calcBP_MinXYZ_FromID(hypotheticalAABB_ID, AABBSIZE);

                //// Draw a cube at that location
                //sMesh* pDebugAABB = pFindMeshByFriendlyName("AABB_MinXYZ_At_Origin");
                //pDebugAABB->positionXYZ = minXYZofHypotheticalCube;
                //pDebugAABB->bIsVisible = true;
                //pDebugAABB->uniformScale = 1'000.0f;

                // Is this an AABB that's already part of the broad phase? 
                // i.e. is it already in the map?
                std::map< unsigned long long, cPhysics::cBroad_Cube* >::iterator
                    it_pCube = physMan->map_BP_CubeGrid.find(hypotheticalAABB_ID);
                //
                if (it_pCube == physMan->map_BP_CubeGrid.end())
                {
                    // NO, there is no cube there
                    //pDebugAABB->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                    numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;
                }
                // NOT equal to the end
                if (it_pCube != physMan->map_BP_CubeGrid.end())
                {
                    // YES, there is an AABB (full of triangles) there!
                 //   pDebugAABB->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
                    // 
                    // 


                    cPhysics::cBroad_Cube* pTheAABB_Cube = it_pCube->second;
                    float size = pTheAABB_Cube->getMaxXYZ().x - pTheAABB_Cube->getMinXYZ().x;
                    glm::vec3 center = glm::vec3(pTheAABB_Cube->getMinXYZ().x + size / 2, pTheAABB_Cube->getMinXYZ().y + size / 2, pTheAABB_Cube->getMinXYZ().z + size / 2);
                    DrawDebugCube(center, glm::vec4(1, 1, 1, 1), size, object->scene->programs[0], object->scene);
                    std::cout << pTheAABB_Cube->vec_pTriangles.size() << std::endl;

                    float closestDistance = 100000000000;
                    int ammount = pTheAABB_Cube->vec_pTriangles.size() / 1000;
                    int i = 0;
                    
                    for(cPhysics::sTriangle tri: pTheAABB_Cube->vec_pTriangles)
                    {
                        i++;
                        if (i > ammount)
                        {
                            i = 0;
                            DrawDebugCube(tri.vertices[0], glm::vec4(1, 1, 1, 1), 0.1, object->scene->programs[0], object->scene);
                        }
                       
                    //Very stupid and simple way to check collisions. TODO: Change    
                        float distance = glm::distance(tri.vertices[0], physData.position);
                        if (distance < closestDistance) closestDistance = distance;
                        if (distance<50)
                        {
                            object->mesh->positionXYZ = object->startTranform->position;
                        }
                    
                    
                    }
                    std::cout << closestDistance << std::endl;

                    // Pass THIS smaller list of triangles to the narrow phase
                    numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = pTheAABB_Cube->vec_pTriangles.size();
                }


                std::cout << "[BROAD] There are " << numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing << "triangles in current box." << std::endl;

            
             //  DrawMesh(pDebugAABB, program);
            //    pDebugAABB->bIsVisible = false;

            
        

    }


};