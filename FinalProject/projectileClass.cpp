#include "projectileClass.h"



projectileClass::projectileClass()
{	
	setDirection();
}

projectileClass::projectileClass(textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color) 
	: creatureClass(sprite, health, speed, power, pos, size, rotation, collOffsetXY, collOffsetZW, color)
{
	setDirection();
}


projectileClass::~projectileClass()
{	
}


