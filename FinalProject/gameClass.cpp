#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "gameClass.h"

gameClass::gameClass()
{
	spawnTimer = 0.0f;
	projectileTimer = 0.0f;	
	activeProjectile = new projectileClass();
	renderingPort = { 0.0f, 0.0f };
	animationTime = 0.0f;
	isPlayerDead = false;

	srand(time(NULL));
}

gameClass::~gameClass()
{
	delete renderer;
	delete player;
	delete activeProjectile;
}

void gameClass::initializeGame()
{
	SoundEngine = irrklang::createIrrKlangDevice();

	SoundEngine->play2D("audio/02_-_Gauntlet_-_NES_-_Stages_1.ogg", GL_TRUE);

	resourceManagerClass::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	resourceManagerClass::LoadShader("shaders/text.vs", "shaders/text.frag", nullptr, "text");


	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Constants::windowWidth), static_cast<GLfloat>(Constants::windowHeight), 0.0f, -1.0f, 1.0f);
	glm::mat4 projectionText = glm::ortho(0.0f, static_cast<GLfloat>(Constants::windowWidth), 0.0f, static_cast<GLfloat>(Constants::windowHeight), -1.0f, 1.0f);

	resourceManagerClass::GetShader("sprite").Use().SetInteger("sprite", 0);
	resourceManagerClass::GetShader("sprite").SetMatrix4("projection", projection);
	resourceManagerClass::GetShader("text").Use().SetInteger("text", 0);
	resourceManagerClass::GetShader("text").SetMatrix4("projection", projectionText);

	//load textures for objects without animation
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Stone Block Tall.png", GL_TRUE, "wall");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Dirt Block2.png", GL_TRUE, "floor");	
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Wall Block.png", GL_TRUE, "door");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Wall Block Top.png", GL_TRUE, "doorTop");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Key.png", GL_TRUE, "key");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Heart.png", GL_TRUE, "food");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Gem Blue.png", GL_TRUE, "treasure");
	resourceManagerClass::LoadTexture("textures/PlanetCute PNG/Selector.png", GL_TRUE, "entrance");
	resourceManagerClass::LoadTexture("textures/spawn.png", GL_TRUE, "spawn");
	
	//load map information from ascii files
	for (int i = 0; i < Constants::nrOfMaps; i++) {
		mapClass tempMap;		
		string mapPath = "maps/map" + to_string(i);

		mapPath = mapPath + ".txt";
		tempMap.loadMapInfo(mapPath);
		mapLevel.push_back(tempMap);
		
	}

	//create player object
	glm::vec2 pos = mapLevel[0].getEntrancePos();
	player = new playerClass("dwarf", 										// creature name
		2,																	// nr of actions which require animation
		resourceManagerClass::GetTexture("player"),	// texture    
		100,																// health
		Constants::playerSpeed,												// speed
		7,																	// attack power
		{ pos.x - 45, pos.y - 30 },		// starting position
		{ 120, 110 },														// size in pixels (width, height)
		0,																	// rotation (0 - 360 degrees, in 45 degree increments
		{ 30, 70 },															// collisionOffsetXY : adjustment to collision box (left upper corner)
		{ -30, 0 },														// collisionOffsetZW : adjustment to collision box (right lower corner)
		{ 1, 1, 1 });														// color
		
	renderer = new SpriteRenderer(resourceManagerClass::GetShader("sprite"), player->tile.getPos(), player->tile.getSize());
	textRenderer = new TextRenderer(resourceManagerClass::GetShader("text"));	
	
	//load textures for objects with animation. Second parameter specifies number of actions which are animated
	loadCreatureTextures("dwarf", 4);
	loadCreatureTextures("axe", 1);
	loadCreatureTextures("skull", 1);
}

void gameClass::loadCreatureTextures(char creature[], int nrActions) {
	int nrFrames = 0;

	char filepathCreature[100] = "";

	strcat(filepathCreature, "textures/");
	strcat(filepathCreature, creature);
	strcat(filepathCreature, "/");
	char filepathAction[100] = "";

	for (int act = 0; act < nrActions; act++) {
		int nrFrames = 0;

		strcpy(filepathAction, filepathCreature);
		strcat(filepathAction, creatureClass::determineFramesPerAction(creature, act, nrFrames));
		char filepathDirection[100] = "";

		for (int direction = 0; direction < 8; direction++) {

			strcpy(filepathDirection, filepathAction);
			char d[3] = "";
			strcpy(d, creatureClass::getWindDirection(direction * 45));

			strcat(filepathDirection, d);
			strcat(filepathDirection, "000");
			char filepathFinal[100] = "";

			for (int i = 0; i < nrFrames; i++) {
				strcpy(filepathFinal, filepathDirection);
				char index[3] = { i + 48 };

				strcat(filepathFinal, index);
				strcat(filepathFinal, ".png");

				resourceManagerClass::LoadTexture(filepathFinal, GL_TRUE, filepathFinal);
			}
		}
	}
 }

void gameClass::render()
{		
	//render map background / map items
	mapLevel[currentLevel].Draw(*renderer);

	//render player
	player->Draw(*renderer);

	//render enemies
	for (auto enemy : enemies) {
		if (enemy.tile.isOnScreen(renderingPort))
			 enemy.Draw(*renderer);
	}

	//render active projectiles, eg axes being thrown by the player
	for (auto i : projectiles) {
		i.Draw(*renderer);
	}
	
	// Text Rendering
	// Player type
	textRenderer->DrawText(player->getPlayerType(), 10, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(player->getPlayerType(), 11, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	
	// Score
	textRenderer->DrawText("Score:", 150, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Score:", 151, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	textRenderer->DrawText(std::to_string(player->getTreasure()), 243, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(std::to_string(player->getTreasure()), 244, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));

	// Health
	textRenderer->DrawText("Health:", 330, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Health:", 331, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	textRenderer->DrawText(std::to_string(player->getHealth()), 436, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(std::to_string(player->getHealth()), 437, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	
	// Keys
	textRenderer->DrawText("Keys:", 510, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Keys:", 511, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	textRenderer->DrawText(std::to_string(player->getKeys()), 583, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(std::to_string(player->getKeys()), 584, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	
	// Level
	textRenderer->DrawText("Level:", Constants::windowWidth - 150, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText("Level:", Constants::windowWidth - 149, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	textRenderer->DrawText(std::to_string(currentLevel + 1), Constants::windowWidth - 65, Constants::windowHeight - 30, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
	textRenderer->DrawText(std::to_string(currentLevel + 1), Constants::windowWidth - 64, Constants::windowHeight - 29, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
	
	// Game Over Message
	if(isPlayerDead)
	{
		textRenderer->DrawText("GAME OVER", 190, Constants::windowHeight - 330, 1.5f, glm::vec3(1.0, 1.0f, 1.0f));
		textRenderer->DrawText("GAME OVER", 187, Constants::windowHeight - 327, 1.5f, glm::vec3(0.0, 0.0f, 0.0f));
	}
}


void gameClass::update(float deltaTime) {
		
	if (newLevel)
		{
		currentLevel++;
		enemies.clear();
		projectiles.clear();
		player->tile.resetPos(mapLevel[currentLevel].getEntrancePos());
		renderer->resetCam(player->tile.getPos(), player->tile.getSize());
		newLevel = false;
	}

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
	
	if(player->getHealth() <= 0)
	{
		if(!isPlayerDead)
		{
			animationTime = 0.0f;
			player->resetIndex();
			isPlayerDead = true;
			player->setCurrentAction("tipping over");
			SoundEngine->play2D("audio/Sound Effect (31).wav", GL_FALSE);
		}

		animationTime += deltaTime;

		if (animationTime >= Constants::animationFrameTime) 
		{
			animationTime = 0.0f;
			player->increaseIndex();
		}
	}

	if (player->getCurrentAction() == "been hit")
	{
		animationTime += deltaTime;

		if (animationTime >= Constants::animationFrameTime)
		{
			animationTime = 0.0f;
			player->increaseIndex();
		}
	}
}

void gameClass::spawnEnemies()
{
	for (int i = 0; i < mapLevel[currentLevel].spawns.size(); i++) {
		int counter = 0;
		
		if (mapLevel[currentLevel].spawns[i].tile.isOnScreen(renderingPort)) {
			
			bool eraseFlag = true;
			while (eraseFlag == true && counter < 10) { // try to spawn an enemy 10 times, if not possible (because of collisions), then stop attempt to spawn	

				int spawnX = mapLevel[currentLevel].spawns[i].tile.getPos().x;
				int spawnY = mapLevel[currentLevel].spawns[i].tile.getPos().y;
				glm::vec2 size(mapLevel[currentLevel].getTileWidth(), mapLevel[currentLevel].getTileHeigth());

				enemyClass enemy("skull", 									// creature name
					1,														// nr of actions which require animation
					resourceManagerClass::GetTexture("skull"),				// sprite
					Constants::enemyHealth,									// health
					Constants::enemySpeed,									// speed
					Constants::enemyPower,									// attack power
					{ spawnX + 10, spawnY },								// starting position
					{ size.x + 10 , size.y + 10 },								// size
					270,													// rotation
					{ 8, 30 },												// collisionOffsetXY
					{ -8, 5 },												// collisionOffsetZW
					{ 1,1,1 }													// color
				);

				eraseFlag = false;
				for (int i = 0; i < enemies.size(); i++) {

					if (enemy.tile == enemies[i].tile) {
						eraseFlag = true;
						counter++;
						break;

					}

				}
				if (!eraseFlag) {
					enemies.push_back(enemy);
					//SoundEngine->play2D("audio/Sound Effect (3).wav", GL_FALSE);
				}
			}
		}
	}
}


void gameClass::moveEnemies(float deltaTime)
{
	for (int i = 0; i < enemies.size(); i++) {

		glm::vec2 oldPos = enemies[i].tile.getPos();	

		if (enemies[i].tile.isOnScreen(renderingPort) && !isPlayerDead) 
		{
			float velocity = enemies[i].getSpeed() * deltaTime;			
			float xFactor = 0.0f, yFactor = 0.0f;

			// calculations which let enemy approach player in direct line rather than at 45 degree angles
			float distanceX, distanceY, distance, timeD;
			distanceX = player->tile.getPos().x + 20- oldPos.x;
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

			bool eraseFlag = false;
			for (int j = 0; j > -2; j--) // iterate twice for movement along x and movement along y to allow enemies to slide along walls which they run into				
			{   
				enemies[i].tile.adjustPos(newPos);
				
				
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
			if (eraseFlag == false) {
				enemies[i].increaseAnimationTime(deltaTime);

				if (enemies[i].getAnimationTime() >= Constants::animationFrameTime) {
					enemies[i].increaseIndex();
					enemies[i].resetAnimationTime();
					enemies[i].tile.setRotation( enemies[i].calculateRotation(distanceX,  distanceY) );
				}
			}
		} // if enemy is on screen
	} // for  : iterate thourgh all enemies
			
	
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

void gameClass::processInput(float deltaTime) // deltaTime = time between frames
{
	float velocity = player->getSpeed() * deltaTime; // adjust player velocity to compensate for fluctuations in frame rate in order to keep player movement smooth
	glm::vec2 oldPos = player->tile.getPos();
	glm::vec2 newPos = { 0.0f, 0.0f };
	int rotat = 0;
	int nrKeys = 0;	
	animationTime += deltaTime;

	if(!isPlayerDead)
	{
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

		if (nrKeys > 0) { // if an arrow key has been pressed;
		
			float oldRotation = player->tile.getRotation(); // get current rotation
			player->tile.setRotation(rotat / nrKeys); // determine new rotation based on which combination of arrow keys have been pressed

			if (oldRotation != player->tile.getRotation()){ // if rotation has changed
				player->resetIndex();						//reset the animation index and set animation action to walking
				player->setCurrentAction("walking");
				animationTime = 0.0f;
			
			}
			else if (animationTime >= Constants::animationFrameTime) {	// if rotation has not changed and if it is time for the next animation
				animationTime = 0.0f;
				player->increaseIndex();			
			}		
		}
		else if (player->getCurrentAction() == "attack" && (animationTime >= Constants::animationFrameTime)) {
			animationTime = 0.0f;
			player->increaseIndex();
		
		}

		player->tile.adjustPos(newPos);
		bool eraseFlag = false;

		if (checkCollision(player, eraseFlag))
			player->tile.resetPos(oldPos);
		else {// if player does not collide with anything, adjust camera position
			if (player->tile.getPos().x >= Constants::windowWidth / 2 - player->tile.getSize().x / 2 
				&& player->tile.getPos().x <= Constants::mapWidth - Constants::windowWidth / 2 - player->tile.getSize().x / 2) 
			{				
				renderer->setCam(newPos.x * -1.0f, 0.0f);
			}

			if (player->tile.getPos().y >= Constants::windowHeight / 2 - player->tile.getSize().y / 2
				&& player->tile.getPos().y <= Constants::mapHeight - Constants::windowHeight / 2 - player->tile.getSize().y / 2)
			{
				renderer->setCam(0.0f, newPos.y * -1.0f);
			}
		}	

		if (keys[GLFW_KEY_SPACE] && !keysPressed[GLFW_KEY_SPACE]) {		
				
			setKeysPressed(GLFW_KEY_SPACE, true);

			if (projectileTimer > Constants::fireRate) {
				fireProjectile();
				if (player->getCurrentAction() != "attack") { // if attack animation is not playing allready
					player->setCurrentAction("attack");
					player->resetIndex();
					animationTime = 0.0f;
				}
				projectileTimer = 0;

			}
		}			
	}	
}

bool gameClass::checkCollision( creatureClass *creature, bool &eraseFlag) {   // *creature can be used for either player, enemy or projectile

	for (int i = 0; i < mapLevel[currentLevel].walls.size(); i++) {  // check for collision with walls
		if (creature->tile == mapLevel[currentLevel].walls[i].tile) { // operator == overload			
			return true;			
		}
	}

	for (int i = 0; i < mapLevel[currentLevel].doors.size(); i++) {  // check for collision with doors
		for (int j = 0; j < mapLevel[currentLevel].doors[i].size(); j++) {
			
				if (creature->tile == mapLevel[currentLevel].doors[i][j].tile) { // operator == overload			
				
					if (creature == player && player->getKeys() > 0) { // if player collides with door and if he has a key
					SoundEngine->play2D("audio/Sound Effect (18).wav", GL_FALSE);
					player->incrementKeys(-1);
					mapLevel[currentLevel].doors.erase(mapLevel[currentLevel].doors.begin() + i);
					return false;
					
				}
				else
					return true;				
			}			
		}		
	}

	for (int i = 0; i < mapLevel[currentLevel].pickups.size(); i++) { // check for collision with pickup items: food, keys and treasure
		if (mapLevel[currentLevel].pickups[i].tile == creature->tile) { // operator == overload	
			
			if (creature != player) // if enemy or projectile collides with pickup item
				return true;

			else if (creature == player) {  // else if it was player who collided with item				
				bool destroy = playerPickedUpItem(mapLevel[currentLevel].pickups[i]);
				if (destroy)
					mapLevel[currentLevel].pickups.erase(mapLevel[currentLevel].pickups.begin() + i);
				break;
			}
		}
	}

	if (creature == player || creature == activeProjectile){ // enemies do not collide with spawn points
		for (int i = 0; i < mapLevel[currentLevel].spawns.size(); i++) {  // check for collision with spawn points
			if (creature->tile == mapLevel[currentLevel].spawns[i].tile) { // operator == overload
				if (creature == activeProjectile)
					mapLevel[currentLevel].spawns.erase(mapLevel[currentLevel].spawns.begin() + i);
				return true;
			}
		}
	}

	for (int i = 0; i < enemies.size(); i++) {		// check for collision with enemies if 1) player is moving and 2) an enemy is moving 3) bullet hits enemy		
		
		if (creature != &(enemies[i]) && creature->tile == enemies[i].tile )  { // do not check for collision of enemy object with itself	
			if (creature == player) {
				if (player->getHealth() > 0)
					SoundEngine->play2D("audio/Sound Effect (16).wav", GL_FALSE);
				player->setCurrentAction("been hit");
				player->resetIndex();
				animationTime = 0.0f;
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

	if (creature != player && creature != activeProjectile) {					// check for collision with player if enemy is moving
		if (creature->tile == player->tile) {
			if(player->getHealth() > 0)
				SoundEngine->play2D("audio/Sound Effect (16).wav", GL_FALSE);
			player->setCurrentAction("been hit");
			player->resetIndex();
			animationTime = 0.0f;
			player->incrementHealth(-10);
			eraseFlag = true;						// set flag to erase enemy from vector
			return true;
		}
	}

	return false;
}

bool gameClass::playerPickedUpItem(mapElementClass item)
{
	switch (item.content) {
	case food: // food
		player->incrementHealth(Constants::foodValue);
		SoundEngine->play2D("audio/Sound Effect (10).wav", GL_FALSE);
		return true;
		break;

	case keyTile: // keyTile
		player->incrementKeys(1);
		SoundEngine->play2D("audio/Sound Effect (13).wav", GL_FALSE);
		return true;
		break;

	case treasure: // treasure
		player->increaseTreasure(Constants::treasureValue);
		SoundEngine->play2D("audio/Sound Effect (12).wav", GL_FALSE);
		return true;
		break;

	case mapExit: // exit
		SoundEngine->play2D("audio/Sound Effect (34).wav", GL_FALSE);
		newLevel = true;

	case mapEntrance:
		return false;

	}
}

void gameClass::fireProjectile()
{
	int spawnX = player->tile.getPos().x;
	int spawnY = player->tile.getPos().y;
	
	glm::vec2 size(mapLevel[currentLevel].getTileWidth(), mapLevel[currentLevel].getTileHeigth());

	projectileClass projectileT("axe", 										// creature name
		1,																	// nr of actions which require animation
		resourceManagerClass::GetTexture("axe"), // sprite
		1,											// health
		Constants::bulletSpeed,						// speed
		Constants::bulletPower,						// attack power
		{ spawnX+40 , spawnY+50 },						// starting position
		{ size.x -10 , size.y -15 },				// size
		player->tile.getRotation(),					// rotation
		{ 20, 50 },									// collisionOffsetXY
		{ -15, 0 },									// collisionOffsetZW
		{ 0.6f, 0.6f, 0.6f });						// color

		projectiles.push_back(projectileT);

		SoundEngine->play2D("audio/Sound Effect (6).wav", GL_FALSE);	
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
		else {
			projectiles[i].increaseAnimationTime(deltaTime);
			if (projectiles[i].getAnimationTime() >= Constants::animationFrameTime) {
				projectiles[i].increaseIndex();
				projectiles[i].resetAnimationTime();
			}
		}
		
	}
}

