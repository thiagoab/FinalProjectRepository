#pragma once
#include <string>
#include <glm/glm.hpp> // math library
#include "textureClass.h"
#include "SpriteRenderer.h"

using namespace std;

#define MAP_WIDTH 1750
#define MAP_HEIGHT 1500
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//struct Position {
//	int x;
//	int y;
//};
//typedef struct Position position;
//
//struct TileSize {
//	int width;
//	int height;	
//};
//typedef struct TileSize tileSize;


class tileClass
{
private:
	glm::vec2 size;	
	glm::vec2 pos;
	glm::vec2 collOffsetXY;
	glm::vec2 collOffsetZW;
	float rotation; // 8 potential values from 0-315 (45 degree increments), where rotation 0  =  positive X-axis,  rotation 90 = negative Y-axis
	glm::vec3 color;
	
public:
	tileClass();
	tileClass(glm::vec2 pos, glm::vec2 size, textureClass sprite, float rotation = 0.0f, glm::vec2 collOffsetXY = { 0,0 }, glm::vec2 collOffsetZW = { 0,0 }, glm::vec3 color = { 1.0f, 1.0f, 1.0f });
	~tileClass();
		
	glm::vec2 getSize();
	void setRotation(float newRotation);
	float getRotation() { return rotation; }
	glm::vec3 getColor() { return color; }

	virtual void Draw(SpriteRenderer& renderer);
	textureClass sprite;

	glm::vec2 getPos() { return pos; }
	void adjustPos(glm::vec2 adjust);
	void resetPos(glm::vec2 newPos);

	bool operator==(tileClass &obj);
	glm::vec4 calculateCollBox();
		
	bool tileClass::isOnScreen(glm::vec2 renderingPort);
	
	/*void setCollOffset(int offSet[]);
	glm::vec4 getCollOffset();*/

};

