#include "enemyClass.h"



enemyClass::enemyClass()
{
	type = ghost;
	isVisible = true;
}

enemyClass::enemyClass(char* creature, int nrActions, textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color)
	: creatureClass(creature, nrActions, sprite, health, speed, power, pos, size, rotation, collOffsetXY, collOffsetZW, color)
{
	type = ghost;
	isVisible = true;
}

enemyClass::~enemyClass()
{
}

float enemyClass::calculateRotation(int x, int y)
{	
	if (x!=0)
		x > 0 ? x = 1 : x = -1;
	if (y != 0)
		y > 0 ? y = -1 : y = +1;


	float rotation = 0.0f;
	if (x > 0) {
		rotation = 0 + y*45.0f;
		if (rotation == -45)
			rotation = 315;
	}
	else if (x < 0)
		rotation = 180 - y*45.0f;
	else {
		rotation = 180 - y * 90.0f;
	}
	
	return rotation;
}
