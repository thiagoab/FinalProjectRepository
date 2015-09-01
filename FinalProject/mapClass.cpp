#include "mapClass.h"
#include <iostream>

mapClass::mapClass()
{
	tileWidth = 0;
	tileHeight = 0;
	level = 1;
}


mapClass::~mapClass()
{
}

void mapClass::loadMapInfo(string file)
{
	char tileChar;
	string line;
	ifstream fstream(file);	
	vector<char> fileLine;

	if (fstream) { // if file opened successfully
		while (getline(fstream, line)) { // read line by line from text file
			istringstream sstream(line); // copy line into stringstream
			fileLine.clear();

			while (sstream >> tileChar) { // copy characters from line one-by-one into vector fileLine
				fileLine.push_back(tileChar);
			}

			mapInfo.push_back(fileLine); //copy fileLine row-by-row into 2d vector called mapInfo
		}

		prepareMapObjects();
	}
}

void mapClass::prepareMapObjects()
{
	int fileRows = mapInfo.size();
	tileHeight = Constants::tileHeight; //60;// MAP_HEIGHT / float(fileRows);

	int fileColumns = mapInfo[0].size();
	tileWidth = Constants::tileWidth;// 40;// MAP_WIDTH / float(fileColumns);

	for (int row = 0; row < fileRows ; row++) { // iterate through the 2D vector containing mapInfo (ascii characters)
		for (int column = 0; column < fileColumns; column++)	{

			glm::vec2 pos(int (column * tileWidth) , int((row) * tileHeight - row *25) );
			glm::vec2 size(tileWidth, tileHeight);

			mapElementClass floorT({ pos.x, pos.y }, size, resourceManagerClass::GetTexture("floor"), floorTile, { 1.0f, 1.0f, 1.0f });
			floors.push_back(floorT);

			if (mapInfo[row][column] == 'w') {
				mapElementClass element({ pos.x, pos.y-15}, { size.x, size.y+15 }, resourceManagerClass::GetTexture("wall"), wall, { 1.0f, 1.0f, 1.0f }, 0, { 0, 25 }, { 0, 0 });
				walls.push_back(element);				
			} 
			if (mapInfo[row][column] == '2') {
				mapElementClass element(pos, size, resourceManagerClass::GetTexture("wall"), wall, { 1.0f, 1.0f, 1.0f }, 0, { 0, 25 }, { 0, 0 });
				walls.push_back(element);
			}
			else if (mapInfo[row][column] == 'f') {
				mapElementClass foodT(pos, size , resourceManagerClass::GetTexture("food"), food, { 1.0f, 1.0f, 1.0f}, 0, { 30, 55 }, { -30, -15 });
				pickups.push_back(foodT);
			}
			else if (mapInfo[row][column] == 't') {
				mapElementClass treasureT(pos, size, resourceManagerClass::GetTexture("treasure"), treasure, { 1.0f, 1.0f, 1.0f }, 0, { 30, 55 }, { -30, -15 });
				pickups.push_back(treasureT);
			}
			else if (mapInfo[row][column] == 'k') {
				mapElementClass keyT( pos, { size.x + 20, size.y + 20 }, resourceManagerClass::GetTexture("key"), keyTile, { 1.0f, 1.0f, 1.0f }, 0, { 30, 55 }, { -30, -15 });
				pickups.push_back(keyT);
			}
			else if (mapInfo[row][column] == 's') {
				spawnerClass spawnT("spawn", 0, resourceManagerClass::GetTexture("spawn"), pos, { size.x + 20, size.y}, 100, 0, 0, 0, { 25, 25 }, { -25, -10 }, { 1.0f, 1.0f, 1.0f });
				spawns.push_back(spawnT);				
			}
		
		}
	}

}

void mapClass::Draw(SpriteRenderer & renderer)
{
	glm::vec2 renderingPort;
	renderer.getCam(renderingPort.x, renderingPort.y);
	renderingPort *= -1;

	for (int i = floors.size() - 1; i >= 0; i--) {
		if (floors[i].tile.isOnScreen(renderingPort))
			floors[i].Draw(renderer);
	}

	//glDepthMask(false);

	/*for (int i = 0; i <floors.size(); i++) {
		floors[i].Draw(renderer);
	}*/
	
	for (int i = 0 ; i <walls.size(); i++) {
		if (walls[i].tile.isOnScreen(renderingPort))
			walls[i].Draw(renderer);
	}
	
	for (int i = 0; i <pickups.size(); i++) {
		if (pickups[i].tile.isOnScreen(renderingPort))
			pickups[i].Draw(renderer);
	}
	
	for (int i = 0; i <spawns.size(); i++) {
		if (spawns[i].tile.isOnScreen(renderingPort))
		spawns[i].Draw(renderer);
	}
}