#pragma once

#include "textureClass.h"
#include "SpriteRenderer.h"
#include "Constants.h"


class tileClass
{
private:
	// 2D Vectors
	glm::vec2 size;	
	glm::vec2 pos;
	glm::vec2 collOffsetXY;
	glm::vec2 collOffsetZW;
	
	// 3D Vectors
	glm::vec3 color;
	
	float rotation; // 8 potential values from 0-315 (45 degree increments), where rotation 0  =  positive X-axis,  rotation 90 = negative Y-axis
	
public:
	textureClass sprite;
	
	tileClass();
	tileClass(glm::vec2 pos, glm::vec2 size, textureClass sprite, float rotation = 0.0f, glm::vec2 collOffsetXY = { 0,0 }, glm::vec2 collOffsetZW = { 0,0 }, glm::vec3 color = { 1.0f, 1.0f, 1.0f });
	~tileClass();
		
	glm::vec2 getSize();
	glm::vec2 getPos() { return pos; }
	glm::vec3 getColor() { return color; }
	float getRotation() { return rotation; }
	glm::vec4 calculateCollBox();
	
	void setRotation(float newRotation);
	void adjustPos(glm::vec2 adjust);
	void resetPos(glm::vec2 newPos);

	bool isOnScreen(glm::vec2 renderingPort);
	
	virtual void Draw(SpriteRenderer& renderer);
	
	bool operator==(tileClass &obj);	
};

