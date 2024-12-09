#include "KLFileManager.hpp"


void CreateModelFiles(KLFileManager* fileManager)
{


    // Create an sModelDrawInfo object for testing
    sModelDrawInfo modelInfo;


    modelInfo.modelName = "Cube";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    // Call WriteModelFile to save the model info
    fileManager->WriteModelFile(&modelInfo, "Cube.txt", "XYZNUV");

    // Island
    modelInfo.modelName = "island";
    modelInfo.meshPath = "assets/models/output.ply";
    fileManager->WriteModelFile(&modelInfo, "island.txt", "XYZNUVRGBA");

    // Maze
    modelInfo.modelName = "entrance_01";
    modelInfo.meshPath = "assets/models/edited_models/entrance_01.ply";
    fileManager->WriteModelFile(&modelInfo, "entrance_01.txt", "XYZNUVRGBA");

    modelInfo.modelName = "floor_01";
    modelInfo.meshPath = "assets/models/edited_models/floor_01.ply";
    fileManager->WriteModelFile(&modelInfo, "floor_01.txt", "XYZNUVRGBA");

    modelInfo.modelName = "wall_01";
    modelInfo.meshPath = "assets/models/edited_models/wall_01.ply";
    fileManager->WriteModelFile(&modelInfo, "wall_01.txt", "XYZNUVRGBA");

    modelInfo.modelName = "dead_01";
    modelInfo.meshPath = "assets/models/edited_models/dead_01.ply";
    fileManager->WriteModelFile(&modelInfo, "dead_01.txt", "XYZNUVRGBA");

    modelInfo.modelName = "barrel_01";
    modelInfo.meshPath = "assets/models/edited_models/barrel_01.ply";
    fileManager->WriteModelFile(&modelInfo, "barrel_01.txt", "XYZNUVRGBA");

    modelInfo.modelName = "torch_01";
    modelInfo.meshPath = "assets/models/edited_models/torch_01.ply";
    fileManager->WriteModelFile(&modelInfo, "torch_01.txt", "XYZNUVRGBA");

    modelInfo.modelName = "anvil_01";
    modelInfo.meshPath = "assets/models/edited_models/anvil_01.ply";
    fileManager->WriteModelFile(&modelInfo, "anvil_01.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "metal_fence";
    //modelInfo.meshPath = "assets/models/Race_track/metal_fence_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "metal_fence.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "road";
    //modelInfo.meshPath = "assets/models/Race_track/road_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "road.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "rock";
    //modelInfo.meshPath = "assets/models/Race_track/rock_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "rock.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "trees";
    //modelInfo.meshPath = "assets/models/Race_track/trees_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "trees.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "tyres";
    //modelInfo.meshPath = "assets/models/Race_track/tyres_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "tyres.txt", "XYZNUVRGBA");

    // Sky Sphere
    modelInfo.modelName = "SkySphere";
    modelInfo.meshPath = "assets/models/Sphere_radius_1_xyz_N_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "skysphere.txt", "XYZNUV");

    ////Godray
    //modelInfo.modelName = "GodRay";
    //modelInfo.meshPath = "assets/models/Race_track/godray_xyznuvrgba.ply";
    //fileManager->WriteModelFile(&modelInfo, "godray.txt", "XYZNUVRGBA");

    ////Cloud
    //modelInfo.modelName = "Clouds";
    //modelInfo.meshPath = "assets/models/Race_track/clouds_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "clouds.txt", "XYZNUVRGBA");

    //// Classroom
    //modelInfo.modelName = "Chairs";
    //modelInfo.meshPath = "assets/models/Class_Room/chairs_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "chairs.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "board";
    //modelInfo.meshPath = "assets/models/Class_Room/board_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "board.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "casier";
    //modelInfo.meshPath = "assets/models/Class_Room/casier_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "casier.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "ceiling";
    //modelInfo.meshPath = "assets/models/Class_Room/ceiling_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "ceiling.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "clock";
    //modelInfo.meshPath = "assets/models/Class_Room/clock_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "clock.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "desk";
    //modelInfo.meshPath = "assets/models/Class_Room/desk_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "desk.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "door";
    //modelInfo.meshPath = "assets/models/Class_Room/door_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "door.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "etagere";
    //modelInfo.meshPath = "assets/models/Class_Room/etagere_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "etagere.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "ground";
    //modelInfo.meshPath = "assets/models/Class_Room/ground_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "grand_casier.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "grand_casier";
    //modelInfo.meshPath = "assets/models/Class_Room/grand_casier_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "ground.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "teacher_desk";
    //modelInfo.meshPath = "assets/models/Class_Room/teacher_desk_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "teacher_d.txt", "XYZNUVRGBA");

    //modelInfo.modelName = "wall_concrete";
    //modelInfo.meshPath = "assets/models/Class_Room/wall_concrete_xyznuvrbga.ply";
    //fileManager->WriteModelFile(&modelInfo, "wall_c.txt", "XYZNUVRGBA");

    //SelectBox
    modelInfo.modelName = "Select_Box";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "selectBox.txt", "XYZNUV");

}