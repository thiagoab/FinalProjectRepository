#pragma once
#include "creatureClass.h"

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
				
public:
	playerClass();
	playerClass(char* creature, int nrActions, textureClass sprite, int health, int speed, int power, glm::vec2 pos, glm::vec2 size, float rotation = 0.0f, glm::vec2 collOffsetXY = { 0, 55 }, glm::vec2 collOffsetZW = { 0,0 }, glm::vec3 color = { 1.0f, 1.0f, 1.0f });
	~playerClass();

	void increaseTreasure(int increase);
	int getTreasure();
	int getKeys(void) { return keys; }

	void Draw(SpriteRenderer& renderer);

	void incrementKeys(int i);
	void incrementLives(int i) { lives += i; }
	void incrementTreasure(int i) { treasure += i; }
	void incrementKills() { kills += 1; }
	
	std::string getPlayerType(void);			
};		


