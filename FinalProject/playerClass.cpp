#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "playerClass.h"
#include "resourceManagerClass.h"


playerClass::playerClass()
{
	range = Constants::playerAttackRange;
	treasure = 0;
	lives = 3;
	keys = 0;
	kills = 0;
	speciality = warrior;
	strcpy(filepath, "");
	strcat(filepath, "textures/dwarf/");
	frameIndex = 0;
	
}

playerClass::playerClass(textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color)
: creatureClass(sprite, health, speed, power, pos, size, rotation, collOffsetXY, collOffsetZW, color)
{
	range = Constants::playerAttackRange;
	treasure = 0;
	lives = 3;
	keys = 0;
	kills = 0;
	speciality = warrior;	
	strcpy(filepath, "");
	strcat(filepath, "textures/dwarf/");
	frameIndex = 0;
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
	getPlayerTexture("walking", frameIndex);
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


void playerClass::getPlayerTexture(char action[], int frame){
	
	strcpy(filepath, "");
	strcat(filepath, "textures/dwarf/");
	strcat(filepath, action);
	strcat(filepath, " ");
	
	char d[3] = "";
	strcpy(d, getWindDirection(tile.getRotation()));
	strcat(filepath, d);
	strcat(filepath, "000");
	
	char index[3] = { frame + 48 };
	strcat(filepath, index);
	strcat(filepath, ".png");
	
	tile.sprite = resourceManagerClass::GetTexture(filepath);	
}


void playerClass::loadPlayerTextures() {
	
		for (int act = 0; act < nrActions; act++) {
		
		char action[10] = "";
		strcpy(filepath, "");
		strcat(filepath, "textures/dwarf/");
		
		switch (act) {
		case 0:
			strcat(action, "walking ");
			break;				
		}
		
			strcat(filepath, action);
		
			char filepath2[100] = "";
		for (int direction = 0; direction < nrDirections; direction++) {
			strcpy(filepath2, filepath);
			char d[3] = "";
			strcpy(d, getWindDirection(direction * 45));
			strcat(filepath2, d);
			strcat(filepath2, "000");
			
			char filepath3[100] = "";
			for (int i = 0; i < nrFrames; i++) {
				strcpy(filepath3, filepath2);
				char index[3] = { i + 48 };
				
				strcat(filepath3, index);
				strcat(filepath3, ".png");
				
				resourceManagerClass::LoadTexture(filepath3, GL_TRUE, filepath3);
				
			}
			
		}
		
	}
	
}


char* playerClass::getWindDirection(int rotate) {
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

void playerClass::increaseIndex() {
	
	frameIndex++;
	if (frameIndex == nrFrames)
		frameIndex = 0;
	
}