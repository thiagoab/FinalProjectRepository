#pragma once
#include "mapElementClass.h"
#include "spawnerClass.h"
#include "resourceManagerClass.h"
#include "SpriteRenderer.h"
#include <vector>
#include <fstream>
#include <sstream>


// Class to hold arrays of all objects on the map. Each object of mapClass inside the main gameClass represent a level
// Member functions to load map from text file and convert ascii characters into vectors of game objects.
class mapClass
{
public:
	mapClass();
	~mapClass();

	void loadMapInfo(string file); // load characters from text file into mapInfo vector
	void prepareMapObjects();
	void Draw(SpriteRenderer &renderer);
	
	std::vector<std::vector<char>> getMapInfo(void) { return mapInfo;  }
	float getTileWidth() { return tileWidth; }
	float getTileHeigth() { return tileHeight; }
	int getLevel(void) { return level; }

	int findFirstDoorPiece(int index);
	int destroyDoor(int index);
	bool isDoorAdjacent(glm::vec2 refPos, glm::vec2 pos, glm::vec2 size);

	glm::vec2 getEntrancePos() { return entrancePos; }
	glm::vec2 getExitPos() { return exitPos; }

	vector<mapElementClass> walls;
	vector<mapElementClass> floors;
	vector<mapElementClass> pickups;
	vector <spawnerClass> spawns;
	vector< vector <mapElementClass> > doors;

private:

	vector<vector<char>> mapInfo; // 2D vector which contains ascii map loaded from text file. Each ascii character symbolizes an ingame tile eg 'w' = wall
	float tileHeight;	
	float tileWidth;
	int	level;
	glm::vec2 entrancePos;
	glm::vec2 exitPos;
};