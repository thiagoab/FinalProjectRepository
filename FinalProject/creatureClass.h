#pragma once
#include "tileClass.h"

class creatureClass
{
private:
	int health;
	float speed;
	int power;
	int dirX;  // dirX and dirY is -1, 0 or +1, in combination they determine in which direction projectile is travelling
	int dirY;
	
		
public:
	tileClass tile;
	
	creatureClass();
	creatureClass(textureClass sprite, int health, float speed, int power, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color);
	~creatureClass();

	void incrementHealth(int increase);
	int getHealth();
	float getSpeed();
	void getDirection(int &x, int &y);
	
	void setDirection();

	void Draw(SpriteRenderer& renderer);
};




	

	
