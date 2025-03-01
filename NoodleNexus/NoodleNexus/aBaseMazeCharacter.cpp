#include "aBaseMazeCharacter.h"
#include "Theseus.h"

void BazeMazeCharacter::HandleEncounter()
{
	//Do logic for when they meet.
	maze->thesChar->health -= 100;
	object->Destroy();

}