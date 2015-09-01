#pragma once
#include "creatureClass.h"

class spawnerClass : public creatureClass
{
public:
	spawnerClass();
	spawnerClass(char* creature, int nrActions, textureClass sprite, glm::vec2 pos, glm::vec2 size, int health = 100, int speed = 0, int power = 0, float rotation = 0.0f,
 		glm::vec2 collOffsetXY = { 0, 55 }, glm::vec2 collOffsetZW = { 0,0 }, glm::vec3 color = { 1.0f, 1.0f, 1.0f });
	~spawnerClass();

	static float spawnRate;
};

