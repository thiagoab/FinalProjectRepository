#include "gameClass.h"

gameClass::gameClass()
{
	spawnTimer = 0.0f;
	projectileTimer = 0.0f;	
	activeProjectile = new projectileClass();
	renderingPort = { 0.0f, 0.0f };
	animationTime = 0.0f;
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


	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Constants::windowWidth), static_cast<GLfloat>(Constants::windowHeight), 0.0f, -1.0f, 1.0f);
	glm::mat4 projectionText = glm::ortho(0.0f, static_cast<GLfloat>(Constants::windowWidth), 0.0f, static_cast<GLfloat>(Constants::windowHeight), -1.0f, 1.0f);

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
	
	renderer = new SpriteRenderer(resourceManagerClass::GetShader("sprite"));
	textRenderer = new TextRenderer(resourceManagerClass::GetShader("text"));
	
	mapLevel1.loadMapInfo("maps/map2.txt");

	resourceManagerClass::LoadTexture("textures/dwarf/walking s0004.png", GL_TRUE, "player");
	                       
	player = new playerClass(resourceManagerClass::GetTexture("player"),	// texture    
		100,																// health
		Constants::playerSpeed,												// speed
		7,																	// attack power
		{ Constants::windowWidth / 2, Constants::windowHeight / 2 },		// starting position
		{ 120, 110 },														// size in pixels (width, height)
		0,																	// rotation (0 - 360 degrees, in 45 degree increments
		{ 10, 80 },															// collisionOffsetXY : adjustment to collision box (left upper corner)
		{ -10, 0 },															// collisionOffsetZW : adjustment to collision box (right lower corner)
		{ 1, 1, 1 });														// color
		
	player->loadPlayerTextures();
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
	
	textRenderer->DrawText(player->getPlayerType(), 10, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(player->getPlayerType(), 11, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	
	textRenderer->DrawText("Score:", 150, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Score:", 151, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

	textRenderer->DrawText(std::to_string(player->getTreasure()), 233, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(std::to_string(player->getTreasure()), 234, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

	textRenderer->DrawText("Health:", 300, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Health:", 301, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	
	textRenderer->DrawText(std::to_string(player->getHealth()), 396, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(std::to_string(player->getHealth()), 397, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

	textRenderer->DrawText("Level:", Constants::windowWidth - 150, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Level:", Constants::windowWidth - 149, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

}


void gameClass::update(float deltaTime) {
		
	renderer->getCam(renderingPort.x, renderingPort.y);
	renderingPort *= -1;

	spawnTimer += deltaTime;
	projectileTimer += deltaTime;

	moveEnemies(deltaTime);

	if (spawnTimer > Constants::spawnRate) {
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
				Constants::enemyHealth,									// health
				Constants::enemySpeed,									// speed
				Constants::enemyPower,									// attack power
				{ spawnX - 35, spawnY },								// starting position
				{ size.x+10 , size.y+10 },								// size
				0,														// rotation
				{ 8, 30 },												// collisionOffsetXY
				{ -8, 5 },												// collisionOffsetZW
				{1,1,1});												// color
				
			enemies.push_back(enemy);			
		}
	}
}


void gameClass::moveEnemies(float deltaTime)
{
	for (int i = 0; i < enemies.size(); i++) {

		glm::vec2 oldPos = enemies[i].tile.getPos();	

		if (enemies[i].tile.isOnScreen(renderingPort)) 
		{
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

			glm::vec2 newPos = { xFactor, 0 };

//			std::string path = pathFind(oldPos.x / Constants::mapHeightInTiles, oldPos.y / (Constants::tileWidth - 25), player->tile.getPos().x / Constants::tileHeight, player->tile.getPos().y / (Constants::tileWidth - 25), mapLevel1.getMapInfo());


			//std::vector<std::vector<char>> map = mapLevel1.getMapInfo();
			//if (path.length()>0)
			//{
			//	int j; char c;
			//	int x = oldPos.x/Constants::tileWidth;
			//	int y = oldPos.y/ Constants::tileHeight;
			//	map[x][y] = 2;
			//	for (int i = 0; i<path.length(); i++)
			//	{
			//		c = path.at(i);
			//		j = atoi(&c);
			//		x = x + dx[j];
			//		y = y + dy[j];
			//		map[x][y] = 3;
			//	}
			//	map[x][y] = 4;

			//	// display the map with the route
			//	for (int y = 0; y<Constants::mapWidthInTiles; y++)
			//	{
			//		for (int x = 0; x<Constants::mapHeightInTiles; x++)
			//			if (map[x][y] == '.')
			//				cout << ".";
			//			else if (map[x][y] == 'w')
			//				cout << "w"; //obstacle
			//			else if (map[x][y] == 2)
			//				cout << "S"; //start
			//			else if (map[x][y] == 3)
			//				cout << "R"; //route
			//			else if (map[x][y] == 4)
			//				cout << "F"; //finish
			//			cout << endl;
			//	}
			//}


			for (int j = 0; j > -2; j--) // iterate twice for movement along x and movement along y to allow enemies to slide along walls which they run into				
			{   
				enemies[i].tile.adjustPos(newPos);
				bool eraseFlag = false;
				
				if (checkCollision(&(enemies[i]), eraseFlag)) 
				{
					if (eraseFlag) 
					{
						enemies.erase(enemies.begin() + i);
						break;
					}
					else 
						enemies[i].tile.resetPos(oldPos);												
				}
				else if (j == 0) 
					oldPos.x += newPos.x;											

				newPos = { 0, yFactor };				
			}

			//if (path.size() > 0)
			//{
			//	int x = stoi(path.substr(0,1));
			//	int y = stoi(path.substr(0, 1));

			//	newPos = { oldPos.x + dx[x], oldPos.y + dy[y]};
			//	enemies[i].tile.adjustPos(newPos);
	
			//	oldPos.x = newPos.x;
			//	oldPos.y = newPos.y;
			//	enemies[i].tile.resetPos(oldPos);
			//}

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

	animationTime += deltaTime;
	if (nrKeys > 0) {
		float oldRotation = player->tile.getRotation();
		player->tile.setRotation(rotat / nrKeys);

		if (oldRotation != player->tile.getRotation())
			 player->resetIndex();
		else if (animationTime >= Constants::animationFrameTime) {
			player->increaseIndex();
			animationTime = 0.0f;
			
		}
		
	}

	player->tile.adjustPos(newPos);
	bool eraseFlag = false;

	if (checkCollision(player, eraseFlag))
		player->tile.resetPos(oldPos);
	else {
		if (player->tile.getPos().x >= Constants::windowWidth / 2 && player->tile.getPos().x <= Constants::mapWidth - Constants::windowWidth / 2)
			renderer->setCam(newPos.x * -1.0f, 0.0f);
		if (player->tile.getPos().y >= Constants::windowHeight / 2 && player->tile.getPos().y <= Constants::mapHeight - Constants::windowHeight / 2)
			renderer->setCam(0.0f, newPos.y * -1.0f);
	}

	if (keys[GLFW_KEY_SPACE]) {		
				
		if (projectileTimer > Constants::fireRate) {
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
		player->incrementHealth(Constants::foodValue);
		break;

	case treasure: // treasure
		player->increaseTreasure(Constants::treasureValue);
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
		Constants::bulletSpeed,						// speed
		Constants::bulletPower,						// attack power
		{ spawnX+20 , spawnY+50 },					// starting position
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

