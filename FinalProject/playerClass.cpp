#include "playerClass.h"

playerClass::playerClass()
{
	range = Constants::playerAttackRange;
	treasure = 0;
	lives = 3;
	keys = 0;
	kills = 0;	
}

playerClass::playerClass(char* creature, int nrActions, textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color)
: creatureClass(creature, nrActions, sprite, health, speed, power, pos, size, rotation, collOffsetXY, collOffsetZW, color)
{
	range = Constants::playerAttackRange;
	treasure = 0;
	lives = 3;
	keys = 0;
	kills = 0;
	speciality = warrior;		
}


playerClass::~playerClass()
{
}

void playerClass::increaseTreasure(int increase)
{
	treasure += increase;

}

int playerClass::getTreasure()
{
	return treasure;
}

void playerClass::Draw(SpriteRenderer & renderer)
{
	
	creatureClass::Draw(renderer);
}


std::string playerClass::getPlayerType(void)
{
	switch (speciality)
	{
	case 0:
		return "Warrior";
	default:
		return "";
	}
}

