#pragma once
#include "creatureClass.h"
#include "textureClass.h"
#include <map>

#define PLAYER_ATTACK_RANGE 10

enum playerType { warrior = 0 };
typedef playerType PlayerType;

class playerClass : public creatureClass
{
private:
	int range;
	int treasure;
	int lives;
	int keys;
	int kills;
	PlayerType speciality;


	int nrActions = 1;
	int nrFrames = 8;
	int nrDirections = 8;
	char filepath[100];
	int frameIndex;

	/*map <string, textureClass[8]> directionMap;	*/
	
			
public:
	playerClass();
	playerClass(textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 collOffsetXY = { 0, 55 }, glm::vec2 collOffsetZW = { 0,0 }, glm::vec3 color = { 1.0f, 1.0f, 1.0f });
	~playerClass();

	void increaseTreasure(int increase);
	int getTreasure();

	void Draw(SpriteRenderer& renderer);

	void incrementKeys(int i) { keys += i; }
	void incrementLives(int i) { lives += i; }
	void incrementTreasure(int i) { treasure += i; }
	void incrementKills() { kills += 1; }

	void loadPlayerTextures();

	//float getSpeed();

	char* getWindDirection(int rotate);
	void playerClass::getPlayerTexture(char action[10], int frame);
	void resetIndex() { frameIndex = 0; }
	void increaseIndex();

	
};		


