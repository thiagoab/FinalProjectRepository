#pragma once

#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <cmath>
#include <ctime>
#include "tileClass.h"
#include "Constants.h"




const int dir = 8; // number of possible directions to go at any position
				   // if dir==4
				   //static int dx[dir]={1, 0, -1, 0};
				   //static int dy[dir]={0, 1, 0, -1};
				   // if dir==8

static int dy[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };

static int dx[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };


class AStarTree
{
	// current position
	int xPos;
	int yPos;
	// total distance already travelled to reach the node
	int level;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority

public:
	AStarTree(int xp, int yp, int d, int p);
	~AStarTree() {};
	int getxPos() const { return xPos; }
	int getyPos() const { return yPos; }
	int getLevel() const { return level; }
	int getPriority() const { return priority; }
	void updatePriority(const int & xDest, const int & yDest);
	void nextLevel(const int & i);
	const int estimate(const int & xDest, const int & yDest) const;
};

void printMap(int map[Constants::mapHeightInTiles][Constants::mapWidthInTiles]);
void printMap(std::vector<std::vector<char>> &map);
std::string pathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish, std::vector<std::vector<char>> &map);

