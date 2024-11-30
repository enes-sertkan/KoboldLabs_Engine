#include "KLFileManager.hpp"


void CreateModelFiles(KLFileManager* fileManager)
{


    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;


    modelInfo.modelName = "Cube";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager->WriteModelFile(&modelInfo, "Cube.txt", "XYZNUV");

    modelInfo.modelName = "Sphere";
    modelInfo.meshPath = "assets/models/Sphere_radius_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Sphere.txt", "XYZNUV");

    // Maze
    modelInfo.modelName = "Hangar";
    modelInfo.meshPath = "assets/models/SM_Bld_Hanger_01_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Hangar.txt", "XYZNUVRGBA");

    modelInfo.modelName = "wall";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "wall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "rotatedWall";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "rotatedWall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "door";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Door_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "door.txt", "XYZNUVRGBA");

    modelInfo.modelName = "floor";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Floor_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "floor.txt", "XYZNUVRGBA");

    modelInfo.modelName = "ceiling";
    modelInfo.meshPath = "assets/models/Ply/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "ceiling.txt", "XYZNUVRGBA");

    //Props
    //small
    modelInfo.modelName = "Plants";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_Plant_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Plants.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Chair";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_SwivelChair_04_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Chair.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Bed";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_Bed_02_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Bed.txt", "XYZNUVRGBA");

    //medium
    modelInfo.modelName = "console";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_Console_05_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "console.txt", "XYZNUVRGBA");

    modelInfo.modelName = "SLadder";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_StepLadder_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "SLadder.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Cdesk";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_ControlDesk_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Cdesk.txt", "XYZNUVRGBA");

    //large
    modelInfo.modelName = "printer";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_3DPrinter_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "printer.txt", "XYZNUVRGBA");

    modelInfo.modelName = "treadmill";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_Treadmill_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "treadmill.txt", "XYZNUVRGBA");

    modelInfo.modelName = "stairs";
    modelInfo.meshPath = "assets/models/Ply/SM_Prop_Stairs_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "stairs.txt", "XYZNUVRGBA");

    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

}