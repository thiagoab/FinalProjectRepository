#pragma once
#include "tileClass.h"

enum TileContent { wall = 0, door = 1, floorTile = 2, mapEntrance = 3, mapExit = 4, food = 5, keyTile = 6, treasure = 7 };
typedef enum TileContent tileContent;

class mapElementClass
{
private:
	bool isAccessible;		

public:
	mapElementClass() {};
	mapElementClass(glm::vec2 pos, glm::vec2 size, textureClass sprite, tileContent content, glm::vec3 color = { 1.0f, 1.0f, 1.0f }, float rotation = 0, glm::vec2 collOffsetXY = { 0,0 }, glm::vec2 collOffsetZW = { 0,0 });
	~mapElementClass();

	void setAccess(tileContent content);

	void Draw(SpriteRenderer& renderer);	

	tileClass tile;
	tileContent content;
};

