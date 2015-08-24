#pragma once
#include "creatureClass.h"

enum enemyType {ghost = 0};
typedef enemyType EnemyType;

class enemyClass : public creatureClass
{
private:
	bool isVisible;
	EnemyType type;

public:
	enemyClass();
	enemyClass(textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, 
				glm::vec2 collOffsetXY = { 0, 0 }, glm::vec2 collOffsetZW = { 0,0 }, glm::vec3 color = { 1.0f, 1.0f, 1.0f });
	          
	~enemyClass();
};

