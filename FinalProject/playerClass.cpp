#include "playerClass.h"
#include "resourceManagerClass.h"


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


//void playerClass::loadPlayerTextures() {
//	char action[10] = "";
//
//
//
//	for (int act = 0; act < nrActions; act++) {		
//
//		switch (act) {
//		case 0: 
//			strcat(action, "walking ");
//			break;
//		}
//
//		strcat(filepath, action);
//
//		for (int direction = 0; direction < nrDirections; direction++) {
//			char d[3] = "";
//
//			if (int(tile.getRotation() >= 45) && int(tile.getRotation() <= 135))
//				strcat(d, "n");
//			if (int(tile.getRotation() >= 225) && int(tile.getRotation() <= 315))
//				strcat(d, "s");
//			if (int(tile.getRotation() >= 135) && int(tile.getRotation() <= 225))
//				strcat(d, "w");
//			if (int(tile.getRotation() >= 225) && int(tile.getRotation() <= 45))
//				strcat(d, "e");
//
//			strcat(filepath, d);
//
//			for (int i = 0; i < nrFrames; i++) {
//				char index[2] = "";
//				strcpy(index, i );
//				resourceManagerClass::LoadTexture("textures/dwarf/walking s0000.png", GL_TRUE, action);
//			}
//		}
//	}
//}