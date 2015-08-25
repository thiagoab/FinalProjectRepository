#include "mapElementClass.h"




mapElementClass::mapElementClass(glm::vec2 pos, glm::vec2 size, textureClass sprite, tileContent content,  glm::vec3 color, float rotation, glm::vec2 collOffsetXY, glm::vec2 collOffsetZW)
	:  tile(pos, size, sprite, rotation, collOffsetXY,  collOffsetZW, color), content(content)
{
	setAccess(content);	
}


mapElementClass::~mapElementClass()
{
}

void mapElementClass::setAccess(tileContent content)
{
	switch (content) {
	case 0:						// wall
	case 1:						// door
		isAccessible = false;
		break;
	default:					// case 2-7 : entrance, exit, food, key, treasure
		isAccessible = true;
	}
}

void mapElementClass::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(tile.sprite, tile.getPos(), tile.getSize(), 0.0f, tile.getColor());
}


