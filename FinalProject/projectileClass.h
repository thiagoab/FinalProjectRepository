#pragma once
#include "creatureClass.h"

class projectileClass :	public creatureClass
{
public:
	projectileClass();
	projectileClass(textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f,
		glm::vec2 collOffsetXY = { 0, 0 }, glm::vec2 collOffsetZW = { 0,0 }, glm::vec3 color = { 1.0f, 1.0f, 1.0f });
	~projectileClass();

	
};

