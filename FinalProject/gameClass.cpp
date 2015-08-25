#include "gameClass.h"
#include <iostream>

#define PLAYER_SPEED 150
#define FOOD_VALUE 100
#define TREASURE_VALUE 100
#define SPAWN_RATE 3.0
#define ENEMY_HEALTH 10
#define ENEMY_SPEED 75
#define ENEMY_POWER 10
#define BULLET_SPEED 200
#define BULLET_POWER 10
#define FIRE_RATE 0.15

gameClass::gameClass(int width, int height) : width(width), height(height)
{
	spawnTimer = 0.0f;
	projectileTimer = 0.0f;
	viewPort = { 0,0 };
	activeProjectile = new projectileClass();
}

gameClass::~gameClass()
{
	delete renderer;
	delete player;
	delete activeProjectile;
}

void gameClass::initializeGame()
{
	resourceManagerClass::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
	resourceManagerClass::GetShader("sprite").Use().SetInteger("sprite", 0);
	resourceManagerClass::GetShader("sprite").SetMatrix4("projection", projection);

	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Stone Block Tall.png", GL_TRUE, "wall");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Dirt Block.png", GL_TRUE, "floor");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Character Boy.png", GL_TRUE, "player");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Key.png", GL_TRUE, "key");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Heart.png", GL_TRUE, "food");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Gem Blue.png", GL_TRUE, "treasure");
	resourceManagerClass::LoadTexture("textures/spawn.png", GL_TRUE, "spawn");
	resourceManagerClass::LoadTexture("textures/skull.png", GL_TRUE, "enemy");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Tree Short.png", GL_TRUE, "projectile");
	

	renderer = new SpriteRenderer(resourceManagerClass::GetShader("sprite"));
	
	mapLevel1.loadMapInfo("maps/map2.txt", width, height);

	//                       (texture,                              health, speed, power,       starting pos,      size, rotation, collisionOffsetXY, collisionOffsetZW, color)
	player = new playerClass(resourceManagerClass::GetTexture("player"), 100, PLAYER_SPEED, 7, { width / 2, height / 2 }, { 55, 110 }, 0, { 10, 80 }, { -10,0 }, { 1,1,1 });	
		
}

void gameClass::render()
{
	//renderer->DrawSprite()
	//renderer->DrawSprite(resourceManagerClass::GetTexture("floor"), glm::vec2(0, 0), glm::vec2(width, height));
	mapLevel1.Draw(*renderer);

	player->Draw(*renderer);

	for (int i = 0; i <enemies.size(); i++) {
		enemies[i].Draw(*renderer);
	}

	for (int i = 0; i <projectiles.size(); i++) {
		projectiles[i].Draw(*renderer);
	}
	//renderer->DrawSprite(resourceManagerClass::GetTexture("floor"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void gameClass::update(float deltaTime) {
	
	spawnTimer += deltaTime;
	projectileTimer += deltaTime;

	moveEnemies(deltaTime);

	if (spawnTimer > SPAWN_RATE) {
		spawnEnemies();
		spawnTimer = 0;
	}

	moveProjectiles(deltaTime);
}

void gameClass::spawnEnemies()
{
	for (int i = 0; i <mapLevel1.spawns.size(); i++) {
		
		int spawnX = mapLevel1.spawns[i].tile.getPos().x;
		int spawnY = mapLevel1.spawns[i].tile.getPos().y;

		if (isOnScreen(spawnX, spawnY) /*&& enemies.size() < 1*/) {		
				
			glm::vec2 size(mapLevel1.getTileWidth(), mapLevel1.getTileHeigth());				
				
			enemyClass enemy(resourceManagerClass::GetTexture("enemy"), // sprite
				ENEMY_HEALTH,								// health
				ENEMY_SPEED,								// speed
				ENEMY_POWER,								// attack power
				{ spawnX - 35, spawnY },					// starting position
				{ size.x+10 , size.y+10 },						// size
				0,											// rotation
				{ 8, 30 },									// collisionOffsetXY
				{ -8, 5 },									// collisionOffsetZW
				{1,1,1});									// color
				
			enemies.push_back(enemy);			
		}
	}
}

bool gameClass::isOnScreen(int objX, int objY) {

	if (objX > viewPort.x && objX < viewPort.x + width) // check if spawner is on screen
		if (objY > viewPort.y && objY < viewPort.y + height)
			return true;
	
	return false;
}


void gameClass::moveEnemies(float deltaTime)
{
	
	for (int i = 0; i < enemies.size(); i++) {

		glm::vec2 oldPos = enemies[i].tile.getPos();	

		if (isOnScreen(oldPos.x, oldPos.y)) {

			float velocity = enemies[i].getSpeed() * deltaTime;			
			int xFactor = 0, yFactor = 0;
			/*float distanceX, distanceY;

			distanceX = player->tile.getPos().x - oldPos.x;
			distanceY = player->tile.getPos().y - oldPos.y;*/

			if (player->tile.getPos().x - oldPos.x > 0)
				xFactor++;
			else if (player->tile.getPos().x - oldPos.x < 0) 
				xFactor--;

			(player->tile.getPos().y + 40 - oldPos.y > 0) ? yFactor++ : yFactor--;

			glm::vec2 newPos = { velocity * xFactor, 0 };			
			for (int j = 0; j > -2; j--) {   // iterate twice for movement along x and movement along y to allow enemies to slide along walls which they run into				
			
				enemies[i].tile.adjustPos(newPos);
				bool eraseFlag = false;
				
				if (checkCollision(&(enemies[i]), eraseFlag)) {
					if (eraseFlag) {
						enemies.erase(enemies.begin() + i);
						break;
					}
					else {
						enemies[i].tile.resetPos(oldPos);												
					}
				}
				else if (j == 0) {
					oldPos.x += newPos.x;											
				}
				newPos = { 0, velocity * yFactor };
				
				
			}
		}
	}
}

void gameClass::processInput(float deltaTime) // deltaTime = time between frames
{
	float velocity = player->getSpeed() * deltaTime; // adjust player velocity to compensate for fluctuations in frame rate in order to keep player movement smooth
	glm::vec2 oldPos = player->tile.getPos();
	int rotat = 0;
	int nrKeys = 0;

	if (keys[GLFW_KEY_UP]) {
		player->tile.adjustPos({ 0, velocity*-1 });	
		rotat += 90;
		nrKeys++;
	}
	
	if (keys[GLFW_KEY_DOWN]) {
		player->tile.adjustPos({ 0, velocity });
		rotat += 270;
		nrKeys++;
	}
	if (keys[GLFW_KEY_RIGHT]) {
		player->tile.adjustPos({ velocity, 0 });
		
		keys[GLFW_KEY_UP] ? rotat += 0 : rotat += 360;
		nrKeys++;
	}
		
	if (keys[GLFW_KEY_LEFT]) {
		player->tile.adjustPos({ velocity*-1, 0 });
		rotat += 180;
		nrKeys++;
	}
	if (nrKeys > 0)
		player->tile.setRotation(rotat / nrKeys);

	bool eraseFlag = false;
	if (checkCollision(player, eraseFlag))
		player->tile.resetPos(oldPos);

	if (keys[GLFW_KEY_SPACE]) {		
				
		if (projectileTimer > FIRE_RATE) {
			fireProjectile();
			projectileTimer = 0;
		}		
	}			
}

bool gameClass::checkCollision( creatureClass *creature, bool &eraseFlag) {   // *creature can be used for either player or enemy

	for (int i = 0; i < mapLevel1.walls.size(); i++) {  // check for collision with walls
		if (creature->tile == mapLevel1.walls[i].tile) { // operator == overload			
			return true;			
		}
	}

	for (int i = 0; i < mapLevel1.pickups.size(); i++) { // check for collision with pickup items: food, keys and treasure
		if (mapLevel1.pickups[i].tile == creature->tile) { // operator == overload	
			
			if (creature != player) // if enemy collides with pickup item
				return true;

			else if (creature == player) {  // else if it was player who collided with item
				playerPickedUpItem(mapLevel1.pickups[i]);
				mapLevel1.pickups.erase(mapLevel1.pickups.begin() + i);
				break;
			}
		}
	}

	for (int i = 0; i < mapLevel1.spawns.size(); i++) {  // check for collision with spawn points
		if (creature->tile == mapLevel1.spawns[i].tile) { // operator == overload
			if (creature == activeProjectile)
				mapLevel1.spawns.erase(mapLevel1.spawns.begin() + i);
			return true;			
		}
	}

	for (int i = 0; i < enemies.size(); i++) {		// check for collision with enemies if 1) player is moving and 2) an enemy is moving 3) bullet hits enemy		
		
		if (creature != &(enemies[i]) && creature->tile == enemies[i].tile )  { // check that enemy does not collide with itself	
			if (creature == player) {
				player->incrementHealth(-10);
				enemies.erase(enemies.begin() + i);
			}
			else if (creature == activeProjectile) {
				enemies.erase(enemies.begin() + i);
				player->incrementKills();
			}
			return true;			
		}
	}

	if (creature != player && creature != activeProjectile)						// check for collision with player if enemy is moving
		if (creature->tile == player->tile) {
			player->incrementHealth(-10);
			eraseFlag = true;						// set flag to erase enemy from vector
			return true;
		}

	return false;
}

void gameClass::playerPickedUpItem(mapElementClass item)
{
	switch (item.content) {
	case food: // food
		player->incrementKeys(1);
		break;

	case keyTile: // keyTile
		player->incrementHealth(FOOD_VALUE);
		break;

	case treasure: // treasure
		player->increaseTreasure(TREASURE_VALUE);
		break;
	}
}

void gameClass::fireProjectile()
{
	int spawnX = player->tile.getPos().x;
	int spawnY = player->tile.getPos().y;
	
	glm::vec2 size(mapLevel1.getTileWidth(), mapLevel1.getTileHeigth());

	projectileClass projectileT(resourceManagerClass::GetTexture("projectile"), // sprite
		1,											// health
		BULLET_SPEED,								// speed
		BULLET_POWER,								// attack power
		{ spawnX+20 , spawnY+50 },						// starting position
		{ size.x -20 , size.y -25 },				// size
		player->tile.getRotation(),					// rotation
		{ 0, 0 },									// collisionOffsetXY
		{ 0, 0 },									// collisionOffsetZW
		{ 1,0,0 });									// color

		projectiles.push_back(projectileT);
	
}

void gameClass::moveProjectiles(float deltaTime)
{
	for (int i = 0; i < projectiles.size(); i++) {

		int dirX, dirY;
		projectiles[i].getDirection(dirX, dirY);

		glm::vec2 oldPos = projectiles[i].tile.getPos();
		float velocity = projectiles[i].getSpeed() * deltaTime;
		
		glm::vec2 newPos = { velocity * dirX, velocity * dirY };
		projectiles[i].tile.adjustPos(newPos);
		
		bool eraseFlag = false;

		*activeProjectile = projectiles[i];
		if (checkCollision(activeProjectile, eraseFlag)) {
			projectiles.erase(projectiles.begin() + i);			
		}
		
	}
}

