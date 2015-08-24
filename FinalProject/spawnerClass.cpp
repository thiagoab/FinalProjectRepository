#include "spawnerClass.h"



spawnerClass::spawnerClass()
{
	
}

spawnerClass::spawnerClass(textureClass sprite, glm::vec2 pos, glm::vec2 size, int health, int speed, int power, float rotation,
	glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color) : creatureClass(sprite, health, speed, power, pos, size, rotation, collOffsetXY, collOffsetZW, color)
{}
	



spawnerClass::~spawnerClass()
{
}
