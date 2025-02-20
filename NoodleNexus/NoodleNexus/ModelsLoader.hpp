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
    modelInfo.modelName = "Cubalizer";
    modelInfo.meshPath = "assets/models/Cube_xyz_n_uv.ply";
    fileManager->WriteModelFile(&modelInfo, "cube02.txt", "XYZNUV");

   /* modelInfo.modelName = "banners";
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
    fileManager->WriteModelFile(&modelInfo, "tyres.txt", "XYZNUVRGBA");*/

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

    //// Plane
    //modelInfo.modelName = "Plane";
    //modelInfo.meshPath = "assets/models/UFO.ply";
    //fileManager->WriteModelFile(&modelInfo, "Plane.txt", "XYZNUVRGBA");

    //// Classroom
    ////do not forget to make castle Bodiam_Castle.ply
    //modelInfo.modelName = "Castle";
    //modelInfo.meshPath = "assets/models/Bodiam_Castle.ply";
    //fileManager->WriteModelFile(&modelInfo, "Castle.txt", "XYZNUVRGBA");

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