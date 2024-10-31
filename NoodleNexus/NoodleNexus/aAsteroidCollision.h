#pragma once
#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "PhysicsManager.h"
#include <vector>
#include "aExplosion.h"
#include "aHoldLaser.hpp"


#include <random>

// Generates a random integer between min and max (inclusive)
int RandomIntS(int min, int max) {
	static std::random_device rd;  // Seed for random number engine
	static std::mt19937 gen(rd()); // Mersenne Twister engine

	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

//I KNOW THAT IT WOULD BE BETTER TO SEPERATE LAZER AND COLLISIONS< BUT IT"S EXAM< SO.....
class aAsteroidCollision : public Action
{
public:
	glm::vec3 speed;
	bool isMoving = true;
	bool laserMode = false;

	GLuint program;
	std::vector<Object*> shipGuns;


	float getRandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}


	void CreateAsteroidChunk(Scene* scene, GLuint program, std::vector<Object*> shipGuns)
	{


		glm::vec4 color = glm::vec4(1, 0, 0, 1);

		std::string path;

		if (getRandomFloat(0, 2) > 1)
			path = "assets/models/Asteroid_015_x10_flatshaded_xyz_n_uv.ply";
		else
			path = "assets/models/Asteroid_011_x10_flatshaded_xyz_n_uv.ply";

		float scale = 0.2;
		

		



		Object* aster = scene->CreateObject(object->mesh->positionXYZ, glm::vec3(getRandomFloat(0, 360) , getRandomFloat(0, 360), getRandomFloat(0, 360)), scale, color, "AsteroidChunk", path);

		aMoveXYZSpeed* xyzSpeed = new aMoveXYZSpeed();
		scene->AddActionToObj(xyzSpeed, aster);
		xyzSpeed->speed = glm::vec3(getRandomFloat(-5000, 5000), getRandomFloat(-5000,  5000), getRandomFloat(-5000, 5000));

		aDEstroyOnPos* destrPos = new aDEstroyOnPos();
		scene->AddActionToObj(destrPos, aster);


		aRandomRotation* randomRot = new aRandomRotation();
		randomRot->rotationSpeed.x = getRandomFloat(-20, 20);
		randomRot->rotationSpeed.y = getRandomFloat(-20, 20);
		randomRot->rotationSpeed.z = getRandomFloat(-20, 20);
		scene->AddActionToObj(randomRot, aster);

		aAsteroidCollision* asterColision = new aAsteroidCollision();
		asterColision->program = program;
		asterColision->shipGuns = shipGuns;
		scene->AddActionToObj(asterColision, aster);


	}

	void DrawRay(glm::vec3 pos, glm::vec3 posEnd, GLuint program)
	{
		//Create An empty object and add LazerHolder To it;
		Object* laserHolder =  object->scene->CreateObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1.f,glm::vec4(0,0,0,0), "LaserHolder", "none");
		aHoldLaser* holdLaser = new aHoldLaser();
		object->scene->AddActionToObj(holdLaser, laserHolder);
		holdLaser->startPos = pos;
		holdLaser->endPos = posEnd;
		holdLaser->program = program;
	}
	
	Object* FindClosestGun(glm::vec3 pos)
	{
		Object* shipGun = nullptr;
			
		if (shipGuns.size() < 1)
		{
			std::cout << "NO SHIPGUNS\n";
			return nullptr;
		}
		shipGun	= shipGuns[0];//Just in case

		float minDistance = 100000000.f;
		for (Object* obj : shipGuns)
		{
			float distance = glm::distance(obj->mesh->positionXYZ, pos);
			if (distance < minDistance)
			{
				minDistance = distance;
				shipGun = obj;
			}
		}
		return shipGun;
	}

	//This is not good to keep it here, BUT it's fine for the  exam.
	void CreateExplosion(glm::vec3 pos)
	{
		glm::vec4 color = glm::vec4(1, 0, 0, 1);

		std::string path = "assets/models/Sphere_radius_1_xyz_N_uv.ply";

		//pos.x -= 400;

		float scale = 150;

		Object* explosion = object->scene->CreateObject(pos, object->mesh->rotationEulerXYZ, scale, color, "Explosion", path, false);

		aExplosion* explosionAction = new aExplosion();
		object->scene->AddActionToObj(explosionAction, explosion);

		
	}

	//This is not good to keep it here, BUT it's fine for the  exam.
	void CreateHitMark(glm::vec3 pos)
	{
		glm::vec4 color = glm::vec4(0.3, 0.3, 0.7, 1); //Made blue for better visibility

		std::string path = "assets/models/Sphere_radius_1_xyz_N_uv.ply";


		float scale = 151;
		//pos.x -= 350;

		Object* explosion = object->scene->CreateObject(pos, object->mesh->rotationEulerXYZ, scale, color, "HitMark", path, false);


	}



	



	void Update() override
	{

		if (object->name == "Asteroid") laserMode = object->scene->laserMode; //I'm gonnax do this. Yes. Not best, but ok.

		glm::vec3 pos = object->mesh->positionXYZ;
		glm::vec3 posEnd = object->mesh->positionXYZ;
		
		if (laserMode)
		posEnd.x -= 10000*object->mesh->uniformScale;
		else
		posEnd.x -= 1000 * object->mesh->uniformScale;
		
	//	DrawRay(pos, posEnd, program);
		std::vector<sCollision_RayTriangleInMesh> collisions;
		bool colided = object->scene->physicsMan->RayCast(pos, posEnd, collisions, false);


     if (colided)
	 {
		 
		 
		// DrawRay(glm::vec3(0, 0, 0), object->mesh->positionXYZ, program);

		 //Idea is that explosion is big enough to hide hitmark at the start
		 //So hitmark will be visible only after explosion
		 if (laserMode)
		 {
			 
			 glm::vec3 gunPos = FindClosestGun(object->mesh->positionXYZ)->mesh->positionXYZ; //taking position of closest gun.

			 DrawRay(gunPos, object->mesh->positionXYZ, program);
			 CreateExplosion(object->mesh->positionXYZ);
			 
			 int chunkCount = RandomIntS(2, 7);

			 for (int i = 0; i < chunkCount; i++)
				 CreateAsteroidChunk(object->scene, program, shipGuns);
			


		 }
		 else
		 {
			 CreateExplosion(collisions[0].theRay.endXYZ);
			 CreateHitMark(collisions[0].theRay.endXYZ);
		 }

		 object->Destroy();
	 }

	 for (sCollision_RayTriangleInMesh col : collisions)
	 {
	//	 printf("COLLIDED: %s", col.timeOfCollision);
	 }

	 //if (colided)
	 //printf("colided \n");
	 //else
	 //printf("not colided \n");
	}
};