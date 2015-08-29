#include "gameClass.h"
#include <iostream>

#define PLAYER_SPEED 150
#define FOOD_VALUE 100
#define TREASURE_VALUE 100
#define SPAWN_RATE 3.0
#define ENEMY_HEALTH 10
#define ENEMY_SPEED 100
#define ENEMY_POWER 10
#define BULLET_SPEED 200
#define BULLET_POWER 10
#define FIRE_RATE 0.15

gameClass::gameClass()
{
	spawnTimer = 0.0f;
	projectileTimer = 0.0f;	
	activeProjectile = new projectileClass();
	renderingPort = { 0.0f, 0.0f };
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
	resourceManagerClass::LoadShader("shaders/text.vs", "shaders/text.frag", nullptr, "text");


	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WINDOW_WIDTH), static_cast<GLfloat>(WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);
	glm::mat4 projectionText = glm::ortho(0.0f, static_cast<GLfloat>(WINDOW_WIDTH), 0.0f, static_cast<GLfloat>(WINDOW_HEIGHT), -1.0f, 1.0f);

	resourceManagerClass::GetShader("sprite").Use().SetInteger("sprite", 0);
	resourceManagerClass::GetShader("sprite").SetMatrix4("projection", projection);
	resourceManagerClass::GetShader("text").Use().SetInteger("text", 0);
	resourceManagerClass::GetShader("text").SetMatrix4("projection", projectionText);


	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Stone Block Tall.png", GL_TRUE, "wall");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Dirt Block2.png", GL_TRUE, "floor");	
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Key.png", GL_TRUE, "key");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Heart.png", GL_TRUE, "food");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Gem Blue.png", GL_TRUE, "treasure");
	resourceManagerClass::LoadTexture("textures/spawn.png", GL_TRUE, "spawn");
	resourceManagerClass::LoadTexture("textures/skull.png", GL_TRUE, "enemy");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Tree Short.png", GL_TRUE, "projectile");
	
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Character Boy.png", GL_TRUE, "player");

	renderer = new SpriteRenderer(resourceManagerClass::GetShader("sprite"));
	textRenderer = new TextRenderer(resourceManagerClass::GetShader("text"));
	
	mapLevel1.loadMapInfo("maps/map2.txt");

	//                       (texture,                              health, speed, power,       starting pos,      size, rotation, collisionOffsetXY, collisionOffsetZW, color)
	player = new playerClass(resourceManagerClass::GetTexture("player"), 100, PLAYER_SPEED, 7, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 }, { 55, 110 }, 0, { 10, 80 }, { -10, 0 }, { 1, 1, 1 });
		
}

void gameClass::render()
{
	//renderer->DrawSprite()
	//renderer->DrawSprite(resourceManagerClass::GetTexture("floor"), glm::vec2(0, 0), glm::vec2(width, height));
	//renderer->camera();

	
	mapLevel1.Draw(*renderer);
	player->Draw(*renderer);

	for (int i = 0; i <enemies.size(); i++) {
		if (enemies[i].tile.isOnScreen(renderingPort))
		enemies[i].Draw(*renderer);
	}

	for (int i = 0; i <projectiles.size(); i++) {		
		projectiles[i].Draw(*renderer);
	}
	//renderer->DrawSprite(resourceManagerClass::GetTexture("floor"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	textRenderer->DrawText(player->getPlayerType(), 10, WINDOW_HEIGHT  - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(player->getPlayerType(), 11, WINDOW_HEIGHT - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	
	textRenderer->DrawText("Score:", 150, WINDOW_HEIGHT - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Score:", 151, WINDOW_HEIGHT - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

	textRenderer->DrawText(std::to_string(player->getTreasure()), 233, WINDOW_HEIGHT - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(std::to_string(player->getTreasure()), 234, WINDOW_HEIGHT - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

	textRenderer->DrawText("Health:", 300, WINDOW_HEIGHT - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Health:", 301, WINDOW_HEIGHT - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	
	textRenderer->DrawText(std::to_string(player->getHealth()), 396, WINDOW_HEIGHT - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(std::to_string(player->getHealth()), 397, WINDOW_HEIGHT - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

	textRenderer->DrawText("Level:", WINDOW_WIDTH - 150, WINDOW_HEIGHT - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Level:", WINDOW_WIDTH - 149, WINDOW_HEIGHT - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

}


void gameClass::update(float deltaTime) {
		
	renderer->getCam(renderingPort.x, renderingPort.y);
	renderingPort *= -1;

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
		
		if (mapLevel1.spawns[i].tile.isOnScreen(renderingPort)) {
			
			int spawnX = mapLevel1.spawns[i].tile.getPos().x;
			int spawnY = mapLevel1.spawns[i].tile.getPos().y;		
				
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


void gameClass::moveEnemies(float deltaTime)
{
	for (int i = 0; i < enemies.size(); i++) {

		glm::vec2 oldPos = enemies[i].tile.getPos();	

		if (enemies[i].tile.isOnScreen(renderingPort)) {

			float velocity = enemies[i].getSpeed() * deltaTime;			
			float xFactor = 0.0f, yFactor = 0.0f;

			// calculations which let enemy approach player in direct line rather than at 45 degree angles
			float distanceX, distanceY, distance, timeD;
			distanceX = player->tile.getPos().x - oldPos.x;
			distanceY = player->tile.getPos().y + 40 - oldPos.y;
			distance = sqrt( distanceX * distanceX + distanceY * distanceY);
			timeD = distance / velocity;
			xFactor = distanceX / timeD;
			yFactor = distanceY / timeD;

			/*if (player->tile.getPos().x - oldPos.x > 5)
				xFactor = velocity * 1;
			else if (player->tile.getPos().x - oldPos.x < -5)
				xFactor = velocity * -1;

			if (player->tile.getPos().y + 40 - oldPos.y > 5)
				yFactor = velocity * 1;
			else if (player->tile.getPos().y + 40 - oldPos.y < -5)
				yFactor = velocity * -1;*/

			glm::vec2 newPos = { 1 * xFactor, 0 };			
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
				newPos = { 0, 1 * yFactor };				
			}
		}
	}
}

void gameClass::processInput(float deltaTime) // deltaTime = time between frames
{
	float velocity = player->getSpeed() * deltaTime; // adjust player velocity to compensate for fluctuations in frame rate in order to keep player movement smooth
	glm::vec2 oldPos = player->tile.getPos();
	glm::vec2 newPos = { 0.0f, 0.0f };
	int rotat = 0;
	int nrKeys = 0;	

	if (keys[GLFW_KEY_UP]) {
		newPos ={newPos.x + 0, newPos.y + velocity*-1 };
		rotat += 90;
		nrKeys++;
		
	}
	
	if (keys[GLFW_KEY_DOWN]) {		
		newPos = { newPos.x + 0, newPos.y + velocity*1 };
		rotat += 270;
		nrKeys++;
		
	}

	if (keys[GLFW_KEY_RIGHT]) {
		newPos = { newPos.x + velocity, newPos.y + 0 };
		keys[GLFW_KEY_UP] ? rotat += 0 : rotat += 360;
		nrKeys++;
	}
		
	if (keys[GLFW_KEY_LEFT]) {
		newPos = { newPos.x + velocity * -1, newPos.y + 0 };
		rotat += 180;
		nrKeys++;
		
	}

	if (nrKeys > 0)
		player->tile.setRotation(rotat / nrKeys);

	player->tile.adjustPos(newPos);
	bool eraseFlag = false;

	if (checkCollision(player, eraseFlag))
		player->tile.resetPos(oldPos);
	else {
		if (player->tile.getPos().x >= WINDOW_WIDTH / 2 && player->tile.getPos().x <= MAP_WIDTH - WINDOW_WIDTH / 2)
			renderer->setCam(newPos.x * -1.0f, 0.0f);
		if (player->tile.getPos().y >= WINDOW_HEIGHT / 2 && player->tile.getPos().y <= MAP_HEIGHT - WINDOW_HEIGHT / 2)
			renderer->setCam(0.0f, newPos.y * -1.0f);
	}

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
		if (checkCollision(activeProjectile, eraseFlag)  ||  !projectiles[i].tile.isOnScreen(renderingPort)) {
			projectiles.erase(projectiles.begin() + i);			
		}
		
	}
}

