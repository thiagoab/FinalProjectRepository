#include "tileClass.h"

// assuming screensize of 800x640, this will give viewport tile dimensions of 25(w) x 20(h). ? Aim for total map size of 100 x 100 tiles ?
tileClass::tileClass() : pos({ 0, 0 }), size({ Constants::tileWidth, Constants::tileHeight }), rotation(0.0f), collOffsetXY({ 0,0 }), collOffsetZW({ 0,0 }), color({ 1.0f, 1.0f, 1.0f })
{

}


tileClass::tileClass(glm::vec2 pos, glm::vec2 size, textureClass sprite, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color)
	: pos(pos), size(size), sprite(sprite), rotation(rotation), collOffsetXY(collOffsetXY), collOffsetZW(collOffsetZW), color(color)
{	
	
}


tileClass::~tileClass() { }


void tileClass::adjustPos(glm::vec2 adjust)
{
	pos.x += adjust.x;
	pos.y += adjust.y;
}

void tileClass::resetPos(glm::vec2 newPos)
{
	pos = newPos;
}

bool tileClass::operator==(tileClass &obj2)
{
	glm::vec4 box1 = this->calculateCollBox();
	glm::vec4 box2 = obj2.calculateCollBox();
	
	if ((box2.x >= box1.x && box2.x <= box1.z) || (box2.z >= box1.x && box2.z <= box1.z)) 
		if ((box2.y >= box1.y && box2.y <= box1.w) || (box2.w >= box1.y && box2.w <= box1.w)) 			
			return true;
	
	if ((box1.x >= box2.x && box1.x <= box2.z) || (box1.z >= box2.x && box1.z <= box2.z))
		if ((box1.y >= box2.y && box1.y <= box2.w) || (box1.w >= box2.y && box1.w <= box2.w))
			return true;

	return false;
}

glm::vec4 tileClass::calculateCollBox()
{
	glm::vec2 posXY = pos + collOffsetXY;
	glm::vec2 posZW = pos + size + collOffsetZW;

	return { posXY.x, posXY.y, posZW.x, posZW.y };
}


glm::vec2 tileClass::getSize()
{
	return size;
}

void tileClass::setRotation(float newRotation) 
{
	rotation = newRotation;
}

void tileClass::Draw(SpriteRenderer& renderer) // virtual function
{

}

bool tileClass::isOnScreen(glm::vec2 renderingPort) 
{
	if (pos.x > renderingPort.x- Constants::tileWidth && pos.x < renderingPort.x + Constants::windowWidth) // check if spawner is on screen
		if (pos.y > renderingPort.y- Constants::tileHeight && pos.y < renderingPort.y + Constants::windowHeight)
			return true;

	return false;
}