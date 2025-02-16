#include "KLFileManager.hpp"


void CreateModelFiles(KLFileManager* fileManager)
{


    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;


    modelInfo.modelName = "Cube";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager->WriteModelFile(&modelInfo, "Cube.txt", "XYZNUVRGBA");

    // Sky Sphere
    modelInfo.modelName = "SkySphere";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "skysphere.txt", "XYZNUVRGBA");

    // Solar System
    modelInfo.modelName = "Sun";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "sun.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Earth";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "earth.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Mercury";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "mercury.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Venus";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "venus.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Mars";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "mars.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Jupiter";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "jupiter.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Saturn_Sphere";
    modelInfo.meshPath = "assets/models/Saturn_Sphere.ply";
    fileManager->WriteModelFile(&modelInfo, "saturn.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Saturn_Ring";
    modelInfo.meshPath = "assets/models/Saturn_Ring.ply";
    fileManager->WriteModelFile(&modelInfo, "saturn-ring.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Uranus";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "uranus.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Neptune";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "neptune.txt", "XYZNUVRGBA");




    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

}