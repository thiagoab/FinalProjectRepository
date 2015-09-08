#include "creatureClass.h"

creatureClass::creatureClass() : health(0), speed(0.0f), power(0)
{
	frameIndex = 0;	
	strcpy(currentAction, "walking");
}

creatureClass::creatureClass(char* creature, int nrActions, textureClass sprite, int health, float speed, int power, glm::vec2 pos, glm::vec2 size,
							float rotation , glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color)
							: tile(pos, size, sprite, rotation, collOffsetXY, collOffsetZW, color), creature(creature), 
							nrActions(nrActions), health(health), speed(speed), power(power)
{

	frameIndex = 0;
	strcpy(currentAction, "walking");
}

creatureClass::~creatureClass() { }

void creatureClass::incrementHealth(int increase)
{
	health += increase;
}

int creatureClass::getHealth()
{
	return health;
}

void creatureClass::Draw(SpriteRenderer& renderer)
{
	if (creature == "dwarf" || creature == "axe" || creature == "skull")
		getCreatureTexture();
	renderer.DrawSprite(tile.sprite, tile.getPos(), tile.getSize(), 0.0f, tile.getColor());
}

char * creatureClass::determineFramesPerAction(char creature[], int act, int &nrFrames) {

	char action[14] = "";
	switch (act) {
	case 0:
		strcat(action, "walking ");
		if (creature == "dwarf")
			nrFrames = 8;
		else if ( creature == "skull")
			nrFrames = 10;
		else if (creature == "axe")
			nrFrames = 4;

		break;

	case 1:
		strcat(action, "attack ");
		if (creature == "dwarf")
			nrFrames = 2;
		break;
	case 2:
		strcat(action, "tipping over ");
		nrFrames = 10;
		break;
	case 3:
		strcat(action, "been hit ");
		nrFrames = 9;
		break;
	}

	return action;
}


void creatureClass::getCreatureTexture(){

	char filepathCreature[100] = "";
	strcpy(filepathCreature, "");
	strcat(filepathCreature, "textures/");
	strcat(filepathCreature, creature);
	strcat(filepathCreature, "/");
	strcat(filepathCreature, currentAction);
	strcat(filepathCreature, " ");

	char d[3] = "";
	strcpy(d, getWindDirection(tile.getRotation()));
	strcat(filepathCreature, d);
	strcat(filepathCreature, "000");

	char index[3] = { frameIndex + 48 };
	strcat(filepathCreature, index);
	strcat(filepathCreature, ".png");

	tile.sprite = resourceManagerClass::GetTexture(filepathCreature);
}

void creatureClass::increaseIndex() {

	frameIndex++;
	int nrFrames = 0;
	
	if (getCurrentAction() == "walking"){
		determineFramesPerAction(this->creature, 0, nrFrames);
		if (frameIndex == nrFrames)
			frameIndex = 0;
	}
	else if(getCurrentAction() == "tipping over")
	{
		determineFramesPerAction(this->creature, 2, nrFrames);
		
		if (frameIndex == nrFrames)
			frameIndex--;	
	}
	else if (getCurrentAction() == "been hit") {
		determineFramesPerAction(this->creature, 3, nrFrames);
		if (frameIndex == nrFrames) 
		{
			strcpy(currentAction, "walking");
			frameIndex = 0;
		}
	}
	else {
		determineFramesPerAction(this->creature, 1, nrFrames);
		
		if (frameIndex == nrFrames) { // if player is not walking and if end of alternative action cycle reached
			strcpy(currentAction, "walking");
			frameIndex = 0;
		}
	}
}

void creatureClass::setCurrentAction(char action[])
{ 
	strcpy(currentAction, action); 
}


char* creatureClass::getWindDirection(int rotate) {
	char d[3] = "";

	if (rotate >= 45 && rotate <= 135)
		strcat(d, "n");
	else if (rotate >= 225 && rotate <= 315)
		strcat(d, "s");

	if (rotate >= 135 && rotate <= 225)
		strcat(d, "w");
	else if (rotate >= 315 || rotate <= 45)
		strcat(d, "e");

	return d;
}

float creatureClass::getSpeed()
{
	return speed;
}

void creatureClass::getDirection(int & x, int & y)
{
	x = dirX;
	y = dirY;
}

void creatureClass::setDirection()
{
	int rotat = int(tile.getRotation());
	
	dirX = round(cos(rotat*3.141592 /180)); // angle has to be converted to radians ( 180 degrees = 1 pi radians)
	dirY = round(sin(rotat*3.141592 /180))* -1;
}

