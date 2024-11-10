#include "UsableItem.hpp"



class Laser : public UsableItem
{
public:
	glm::vec3 target;
	float lazerLength;

	void Use() override
	{
		std::cout << "LAZER TIME" << std::endl;
		scene->DrawRay(position, target, program,glm::vec4(1.f,0.f,0.f,1.f),false,0.1f);
	}
};