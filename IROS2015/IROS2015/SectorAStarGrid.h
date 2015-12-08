#pragma once

#include "../../libraries/Planning/AStarGrid.h"
#include <map>
// A manager class to handle different instances of grid astar for different sectors

class SectorAStarGrid
{
public:
	SectorAStarGrid(Matrix<int,2> membership_map, vector<pair<int,int> > edges);
	~SectorAStarGrid(void);

	int getMembership(XY p){
		return membership_map(p.x,p.y);
	}

	vector<XY> search(XY p1, XY p2){
		int memstart = getMembership(p1);
		int memnext = getMembership(p2);
		return m2astar[memstart][memnext]->get_solution_path(p1,p2);
	}

	// MAP FOR EVERYTHING
	Matrix<int, 2> membership_map;

private: 
	std::map<int,std::map<int,AStarGrid*> > m2astar; // lets you know which A* to access
};

