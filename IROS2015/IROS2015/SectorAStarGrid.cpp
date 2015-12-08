#include "SectorAStarGrid.h"


SectorAStarGrid::SectorAStarGrid(Matrix<int,2> membership_map, vector<pair<int,int> > edges):
	membership_map(membership_map)
{
	// Gets a grid astar map for each connection
	for (unsigned int i=0; i<edges.size(); i++){
		pair<int,int> e = edges[i];
		m2astar[e.first][e.second] = new AStarGrid(membership_map, e.first, e.second);
	}
}


SectorAStarGrid::~SectorAStarGrid(void)
{
}
