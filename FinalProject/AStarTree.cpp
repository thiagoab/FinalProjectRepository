#include "AStarTree.h"



AStarTree::AStarTree(int xp, int yp, int d, int p) : xPos(xp), yPos(yp), level(d), priority(p)
{

}


void AStarTree::updatePriority(const int & xDest, const int & yDest)
{
	priority = level + estimate(xDest, yDest) * 10; //A*
}


// give better priority to going strait instead of diagonally
void AStarTree::nextLevel(const int & i) // i: direction
{
	level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
}


// Estimation function for the remaining distance to the goal.
const int AStarTree::estimate(const int & xDest, const int & yDest) const
{
	int xd, yd, d;
	xd = xDest - xPos;
	yd = yDest - yPos;

	// Euclidian Distance
	d = static_cast<int>(sqrt(xd*xd + yd*yd));

	// Manhattan distance
	//d=abs(xd)+abs(yd);

	// Chebyshev distance
	//d=max(abs(xd), abs(yd));
	
	return d;
}


// Determine priority (in the priority queue)
bool operator<(const AStarTree & a, const AStarTree & b)
{
	return a.getPriority() > b.getPriority();
}


// A-star algorithm.
// The route returned is a string of direction digits.
std::string pathFind(const int & xStart, const int & yStart, const int & xFinish, const int & yFinish, std::vector<std::vector<char>> &map)
{
	int closed_nodes_map[Constants::mapHeightInTiles][Constants::mapWidthInTiles]; // map of closed (tried-out) nodes
	int open_nodes_map[Constants::mapHeightInTiles][Constants::mapWidthInTiles]; // map of open (not-yet-tried) nodes
	int dir_map[Constants::mapHeightInTiles][Constants::mapWidthInTiles]; // map of directions
	
	std::priority_queue<AStarTree> pq[2]; // list of open (not-yet-tried) nodes
	int pqi = 0; // pq index
	AStarTree* n0;
	AStarTree* m0;
	int i, j, x, y, xdx, ydy;
	char c;

	// reset the node maps
	for (x = 0; x< Constants::mapHeightInTiles; x++)
	{
		for (y = 0; y< Constants::mapWidthInTiles; y++)
		{
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
		}
	}
	//std::cout << "map" << std::endl;
	//printMap(map);
	//std::cout << "closed_nodes_map" << std::endl;
	//printMap(closed_nodes_map);
	//std::cout << "open_nodes_map" << std::endl;
	//printMap(open_nodes_map);
	//std::cout << "dir_map" << std::endl;
	//printMap(dir_map);

	// create the start node and push into list of open nodes
	n0 = new AStarTree(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);

	open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map
	//open_nodes_map[xStart][yStart] = n0->getPriority(); // mark it on the open nodes map

	delete n0;

	// A* search
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority from the list of open nodes
		n0 = new AStarTree(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(), pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); 
		y = n0->getyPos();

		pq[pqi].pop(); // remove the node from the open list

		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if (x == xFinish && y == yFinish)
		{
			// generate the path from finish to start
			// by following the directions
			std::string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty()) pq[pqi].pop();
			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i<dir; i++)
		{
			xdx = x + dx[i]; 
			ydy = y + dy[i];

			if (!(xdx<0 || xdx>Constants::mapHeightInTiles - 1 || ydy<0 || ydy>Constants::mapWidthInTiles - 1 || map[xdx][ydy] != '.' || closed_nodes_map[xdx][ydy] == 1))
			{
				// generate a child node
				m0 = new AStarTree(xdx, ydy, n0->getLevel(), n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0)
				{
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					delete m0;
					// mark its parent node direction
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx][ydy]>m0->getPriority())
				{
					// update the priority info
					open_nodes_map[xdx][ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx && pq[pqi].top().getyPos() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}

					pq[pqi].pop(); // remove the wanted node

								   // empty the larger size pq to the smaller one
					if (pq[pqi].size()>pq[1 - pqi].size()) 
						pqi = 1 - pqi;

					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
					delete m0;
				}
				else delete m0; // garbage collection
			}
			//std::cout << "closed_nodes_map" << std::endl;
			//printMap(closed_nodes_map);
			//std::cout << "open_nodes_map" << std::endl;
			//printMap(open_nodes_map);
			//std::cout << "dir_map" << std::endl;
			//printMap(dir_map);
		}
		delete n0; // garbage collection
	}
	return ""; // no route found
}


void printMap(int map[Constants::mapHeightInTiles][Constants::mapWidthInTiles])
{
	for (int x = 0; x < Constants::mapHeightInTiles; x++)
	{
		std::cout << std::setw(3) << x + 1; // line number
		for (int y = 0; y < Constants::mapWidthInTiles; y++)
			std::cout << std::setw(3) << map[x][y];
		std::cout << std::endl;
	}
}


void printMap(std::vector<std::vector<char>> &map)
{
	for (int x = 0; x < Constants::mapHeightInTiles; x++)
	{
		std::cout << std::setw(3) << x + 1; // line number
		for (int y = 0; y < Constants::mapWidthInTiles; y++)
			std::cout << std::setw(3) << map[x][y];
		std::cout << std::endl;
	}
}