#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma once
#include "tileClass.h"
#include "textureClass.h"
#include "resourceManagerClass.h"
#include <map>
#include <vector>

class creatureClass
{
private:
	int health;
	float speed;
	int power;
	int dirX;  // dirX and dirY is -1, 0 or +1, in combination they determine in which direction projectile is travelling
	int dirY;
	int frameIndex;		
	char currentAction[10];	
	int nrActions;
 		
	char* creature; // creature name eg 'dwarf', 'skull'
		
public:
	tileClass tile;
	
	creatureClass();
	creatureClass(char* creature, int nrActions, textureClass sprite, int health, float speed, int power, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color);
	~creatureClass();

	void incrementHealth(int increase);
	int getHealth();
	float getSpeed();
	void getDirection(int &x, int &y);
	
	void setDirection();

	void Draw(SpriteRenderer& renderer);
	static char* getWindDirection(int rotate);
	static char * determineFramesPerAction(char creature[], int act, int &nrFrames);
	void getCreatureTexture();
	void resetIndex() { frameIndex = 0; }
	void increaseIndex();
	
	void setCurrentAction(char action[]);
	string getCurrentAction() { return currentAction; }
};




	

	
