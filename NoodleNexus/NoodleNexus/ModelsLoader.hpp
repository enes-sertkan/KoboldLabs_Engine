#include "KLFileManager.hpp"


void CreateModelFiles(KLFileManager* fileManager)
{


    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;


    modelInfo.modelName = "Cube";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager->WriteModelFile(&modelInfo, "Cube.txt", "XYZNUV");

    // Sky Sphere
    modelInfo.modelName = "SkySphere";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "skysphere.txt", "XYZNUV");

    // Solar System
    modelInfo.modelName = "Sun";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "sun.txt", "XYZNUV");

    modelInfo.modelName = "Earth";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "earth.txt", "XYZNUV");

    modelInfo.modelName = "Mercury";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "mercury.txt", "XYZNUV");

    modelInfo.modelName = "Venus";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "venus.txt", "XYZNUV");

    modelInfo.modelName = "Mars";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "mars.txt", "XYZNUV");

    modelInfo.modelName = "Jupiter";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "jupiter.txt", "XYZNUV");

    modelInfo.modelName = "Saturn";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Saturn.txt", "XYZNUV");

    modelInfo.modelName = "Uranus";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "uranus.txt", "XYZNUV");

    modelInfo.modelName = "Neptune";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Neptune.txt", "XYZNUV");




    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

}