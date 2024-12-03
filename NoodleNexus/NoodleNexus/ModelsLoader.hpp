#include "KLFileManager.hpp"


void CreateModelFiles(KLFileManager* fileManager)
{


    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;


    modelInfo.modelName = "Cube";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager->WriteModelFile(&modelInfo, "Cube.txt", "XYZNUV");

    // Maze
    modelInfo.modelName = "baloons";
    modelInfo.meshPath = "assets/models/Race_track/baloons_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "baloons.txt", "XYZNUVRGBA");

    modelInfo.modelName = "banners";
    modelInfo.meshPath = "assets/models/Race_track/banners_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "banners.txt", "XYZNUVRGBA");

    modelInfo.modelName = "barriers";
    modelInfo.meshPath = "assets/models/Race_track/barriers_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "barriers.txt", "XYZNUVRGBA");

    modelInfo.modelName = "garage";
    modelInfo.meshPath = "assets/models/Race_track/garage_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "garage.txt", "XYZNUVRGBA");

    modelInfo.modelName = "grass01";
    modelInfo.meshPath = "assets/models/Race_track/grass_01_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "grass01.txt", "XYZNUVRGBA");

    modelInfo.modelName = "grass02";
    modelInfo.meshPath = "assets/models/Race_track/grass_02_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "grass02.txt", "XYZNUVRGBA");

    modelInfo.modelName = "gravel";
    modelInfo.meshPath = "assets/models/Race_track/gravel_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "gravel.txt", "XYZNUVRGBA");

    modelInfo.modelName = "metal_fence";
    modelInfo.meshPath = "assets/models/Race_track/metal_fence_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "metal_fence.txt", "XYZNUVRGBA");

    modelInfo.modelName = "road";
    modelInfo.meshPath = "assets/models/Race_track/road_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "road.txt", "XYZNUVRGBA");

    modelInfo.modelName = "rock";
    modelInfo.meshPath = "assets/models/Race_track/rock_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "rock.txt", "XYZNUVRGBA");

    modelInfo.modelName = "trees";
    modelInfo.meshPath = "assets/models/Race_track/trees_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "trees.txt", "XYZNUVRGBA");

    modelInfo.modelName = "tyres";
    modelInfo.meshPath = "assets/models/Race_track/tyres_xyznuvrbga.ply";
    fileManager->WriteModelFile(&modelInfo, "tyres.txt", "XYZNUVRGBA");

    // Sky Sphere
    modelInfo.modelName = "SkySphere";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "skysphere.txt", "XYZNUV");

    //Godray
    modelInfo.modelName = "GodRay";
    modelInfo.meshPath = "assets/models/Race_track/godray_xyznuvrgba.ply";
    fileManager->WriteModelFile(&modelInfo, "godray.txt", "XYZNUVRGBA");

    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

}