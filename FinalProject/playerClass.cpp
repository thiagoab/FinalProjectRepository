#include "playerClass.h"



playerClass::playerClass()
{
	range = PLAYER_ATTACK_RANGE;
	treasure = 0;
	lives = 3;
	keys = 0;
	kills = 0;
	speciality = warrior;
	
}

playerClass::playerClass(textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color)
: creatureClass(sprite, health, speed, power, pos, size, rotation, collOffsetXY, collOffsetZW, color)
{
	range = PLAYER_ATTACK_RANGE;
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

//void playerClass::hugWall(tileClass wall) {
//
//	switch (int (this->tile.getRotation())) {
//	case 0 :
//		this->tile.resetPos({ this->tile.getPos().x, wall.getPos().y + wall.getSize().y });
//		break;
//		
//	case 90:
//		this->tile.resetPos({ wall.getPos().x - this->tile.getSize().x, this->tile.getPos().y });
//		break;
//
//	case 180:
//		this->tile.resetPos({ this->tile.getPos().x, wall.getPos().y - this->tile.getSize().y });
//		break;
//
//	case 270:
//		this->tile.resetPos({ wall.getPos().x + wall.getSize().x, this->tile.getPos().y });
//		break;
//	}
//}

//float playerClass::getSpeed()
//{
//	return creatureClass::getSpeed();
//}
