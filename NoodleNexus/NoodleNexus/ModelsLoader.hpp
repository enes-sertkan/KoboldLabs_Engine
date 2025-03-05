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

    modelInfo.modelName = "wall";
    modelInfo.meshPath = "assets/models/objects/wall01.ply";
    fileManager->WriteModelFile(&modelInfo, "wall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "BigWall";
    modelInfo.meshPath = "assets/models/objects/floor.ply";
    fileManager->WriteModelFile(&modelInfo, "bigWall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "cubeWall";
    modelInfo.meshPath = "assets/models/objects/wall01side.ply";
    fileManager->WriteModelFile(&modelInfo, "cubekWall.txt", "XYZNUVRGBA");

    modelInfo.modelName = "MinotaurE";
    modelInfo.meshPath = "assets/models/Chars/MinoE.ply";
    fileManager->WriteModelFile(&modelInfo, "minotaur.txt", "XYZNUVRGBA");

    modelInfo.modelName = "TheseusE";
    modelInfo.meshPath = "assets/models/Chars/TheseusE.ply";
    fileManager->WriteModelFile(&modelInfo, "theseus.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FoodE";
    modelInfo.meshPath = "assets/models/Food/MelonE.ply";
    fileManager->WriteModelFile(&modelInfo, "melonE.txt", "XYZNUVRGBA");

    modelInfo.modelName = "WaterE";
    modelInfo.meshPath = "assets/models/Food/WaterE.ply";
    fileManager->WriteModelFile(&modelInfo, "waterE.txt", "XYZNUVRGBA");

    modelInfo.modelName = "MinotaurV";
    modelInfo.meshPath = "assets/models/Chars/MinoV.ply";
    fileManager->WriteModelFile(&modelInfo, "minotaurV.txt", "XYZNUVRGBA");

    modelInfo.modelName = "TheseusV";
    modelInfo.meshPath = "assets/models/Chars/TheseusV.ply";
    fileManager->WriteModelFile(&modelInfo, "theseusV.txt", "XYZNUVRGBA");

    modelInfo.modelName = "FoodV";
    modelInfo.meshPath = "assets/models/Food/BananaV.ply";
    fileManager->WriteModelFile(&modelInfo, "bananaV.txt", "XYZNUVRGBA");

    modelInfo.modelName = "WaterV";
    modelInfo.meshPath = "assets/models/Food/WaterV.ply";
    fileManager->WriteModelFile(&modelInfo, "waterV.txt", "XYZNUVRGBA");

    modelInfo.modelName = "door";
    modelInfo.meshPath = "assets/models/extras/SM_Env_Door_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "door.txt", "XYZNUVRGBA");

    modelInfo.modelName = "floor";
    modelInfo.meshPath = "assets/models/extras/SM_Env_Floor_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "floor.txt", "XYZNUVRGBA");

    modelInfo.modelName = "ceiling";
    modelInfo.meshPath = "assets/models/extras/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "ceiling.txt", "XYZNUVRGBA");

    //Props
    //small
    modelInfo.modelName = "Plants";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Plant_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Plants.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Chair";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_SwivelChair_04_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Chair.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Bed";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Bed_02_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Bed.txt", "XYZNUVRGBA");

    //medium
    modelInfo.modelName = "console";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Console_05_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "console.txt", "XYZNUVRGBA");

    modelInfo.modelName = "SLadder";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_StepLadder_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "SLadder.txt", "XYZNUVRGBA");

    modelInfo.modelName = "Cdesk";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_ControlDesk_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "Cdesk.txt", "XYZNUVRGBA");

    //large
    modelInfo.modelName = "printer";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_3DPrinter_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "printer.txt", "XYZNUVRGBA");

    modelInfo.modelName = "treadmill";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Treadmill_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "treadmill.txt", "XYZNUVRGBA");

    modelInfo.modelName = "stairs";
    modelInfo.meshPath = "assets/models/extras/SM_Prop_Stairs_01_xyz_n_rgba_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "stairs.txt", "XYZNUVRGBA");


    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

}