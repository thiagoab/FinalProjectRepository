#include "creatureClass.h"



creatureClass::creatureClass()
{
	health = 0;
	speed = 0.0f;	
	power = 0;		
}

creatureClass::creatureClass(textureClass sprite, int health, float speed, int power, glm::vec2 pos, glm::vec2 size, float rotation , glm::vec2 collOffsetXY, glm::vec2 collOffsetZW, glm::vec3 color)
	: tile(pos, size, sprite, rotation, collOffsetXY, collOffsetZW, color), health(health), speed(speed), power(power) { }

creatureClass::~creatureClass() { }

void creatureClass::incrementHealth(int increase)
{
	health += increase;
}

int creatureClass::getHealth()
{
	return health;
}

void creatureClass::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(tile.sprite, tile.getPos(), tile.getSize(), 0.0f, tile.getColor());
}

float creatureClass::getSpeed()
{
	return speed;
}

void creatureClass::getDirection(int & x, int & y)
{
	x = dirX;
	y = dirY;
}

void creatureClass::setDirection()
{
	int rotat = int(tile.getRotation());
	
	dirX = round(cos(rotat*3.141592 /180)); // angle has to be converted to radians ( 180 degrees = 1 pi radians)
	dirY = round(sin(rotat*3.141592 /180))* -1;
	

}

