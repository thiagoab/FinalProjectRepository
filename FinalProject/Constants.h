#pragma once
#include <glm/glm.hpp> // math library

namespace Constants
{
	// tile constants
	const static int tileWidth = 40; // in pixels
	const static int tileHeight = 60; // in pixels

	// map constants
	const static int mapWidth = 1760; // in pixels
	const static int mapHeight = 1495; // in pixels
	const static int mapWidthInTiles = mapWidth / tileWidth;
	const static int mapHeightInTiles = mapHeight / tileHeight;
	const static int verticalOffset = 25;
	const static int nrOfMaps = 3;
	
	// window constants
	const static int windowWidth = 800; // in pixels
	const static int windowHeight = 600; // in pixels


	const static int playerSpeed = 150;
	const static int playerAttackRange = 10;

	const static int foodValue = 100;
	const static int treasureValue = 100;
	const static int enemyHealth = 10;
	const static int enemySpeed = 100;
	const static int enemyPower = 10;
	const static int bulletSpeed = 200;
	const static int bulletPower = 10;
	
	const static float spawnRate = 1.0f;
	const static float fireRate = 0.25f;
	const static float animationFrameTime = 0.075f;

	const static std::string fontFilePath = "fonts/SEVESBRG.ttf";
};