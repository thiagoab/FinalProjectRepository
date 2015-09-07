#pragma once
// GLEW
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW
#include <GLFW\glfw3.h>
#include <iostream>

#include "mapElementClass.h"
#include "playerClass.h"
#include "enemyClass.h"
#include "mapClass.h" // class to load stationary map elements (eg walls, treasure, food, spawn points) from file and keeps them in a container
#include "SpriteRenderer.h"
#include "resourceManagerClass.h"
#include <glm\glm.hpp>
#include "irrKlang\irrKlang.h"
#include "spawnerClass.h"
#include "projectileClass.h"
#include "TextRenderer.h"
#include "AStarTree.h"
#include "Constants.h"


class gameClass
{
public:
	gameClass();
	~gameClass();

	void initializeGame();
	void render();
	void update(float deltaTime);
	void processInput(float deltaTime);

	bool getIsPlayerDead(void) { return isPlayerDead; }

	bool checkCollision(creatureClass *creature, bool&);

	bool getKeys(int i) { return keys[i]; }
	bool getKeysPressed(int i) { return keysPressed[i]; }

	void setKeys(int i, bool flip) { keys[i] = flip; }
	void setKeysPressed(int i, bool flip) { keysPressed[i] = flip; }

	void loadCreatureTextures(char creatureString[], int totalActions);

private:
	
	bool keys[1024], keysPressed[1024];
	bool isPlayerDead;
	float spawnTimer, projectileTimer;
	glm::vec2 renderingPort;
	float animationTime;
	irrklang::ISoundEngine *SoundEngine;

	mapClass mapLevel1;

	TextRenderer *textRenderer;
	SpriteRenderer *renderer;
	playerClass *player;
	projectileClass *activeProjectile;
	vector <enemyClass> enemies;
	vector <projectileClass> projectiles;

	void spawnEnemies();
	void moveEnemies(float deltaTime);
	
	void playerPickedUpItem(mapElementClass item);

	void fireProjectile();
	void moveProjectiles(float deltaTime);
};
