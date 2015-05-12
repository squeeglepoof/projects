// Sandbox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>

#include "../../libraries/FileIO/easyio/easyio.h"
#include "AStar_grid.h"

int main (int argc, char const *argv[]){

	matrix2d m = FileManip::readDouble("C:/Users/Carrie/Documents/Visual Studio 2012/Projects/projects/IROS2015/IROS2015/agent_map/obstacle_map.csv");
	matrix2d mems = FileManip::readDouble("C:/Users/Carrie/Documents/Visual Studio 2012/Projects/projects/IROS2015/IROS2015/agent_map/membership_map.csv");
	matrix2d conns = FileManip::readDouble("C:/Users/Carrie/Documents/Visual Studio 2012/Projects/projects/IROS2015/IROS2015/agent_map/connections.csv");
	matrix2d agentxy = FileManip::readDouble("C:/Users/Carrie/Documents/Visual Studio 2012/Projects/projects/IROS2015/IROS2015/agent_map/agent_map.csv");


	std::vector<std::vector<bool> > * obstacles = new std::vector<std::vector<bool> > (m.size());
	std::vector<std::vector<int> > * members = new std::vector<std::vector<int> > (mems.size());
	for (int i=0; i<m.size(); i++){
		obstacles->at(i) = std::vector<bool>(m[i].size());
		for (int j=0; j<m[i].size(); j++){
			obstacles->at(i)[j] = m[i][j]>0.0;
		}
	}



	for (int i=0; i<mems.size(); i++){
		members->at(i) = std::vector<int>(mems[i].size());
		for(int j=0; j<mems[i].size(); j++){
			members->at(i)[j] = mems[i][j];
		}
	}
	AStar_grid a(obstacles);

	/*
	a.m.solve(0,29,agentxy[0][0],agentxy[0][1]);
	std::cout << "Soln length" << a.m.m_solution_length << std::endl;


	std::cout << a.m << std::endl;

	system("pause");
	exit(1);
	/**/
	matrix2d voronoi_mems(obstacles->size());
	for (int i=0; i<obstacles->size(); i++){
		voronoi_mems[i] = matrix1d(obstacles->at(i).size(),-1); // -1 indicates no membership (should only be obstacles
		for (int j=0; j<obstacles->at(i).size(); j++){
			if(!obstacles->at(i)[j]){
				printf("(%i,%i),",i,j);
				std::vector<double> dist(agentxy.size());
				for (int k=0; k<agentxy.size(); k++){
					dist[k] = a.m.solve(i,j,agentxy[k][1],agentxy[k][0]); // swapping the xy coordinates for agentxy: may be in different reference frame
				}
				// find the shortest distance
				std::vector<double>::iterator shortest_dist = min_element(dist.begin(),dist.end());
				if (*shortest_dist == DBL_MAX){
					voronoi_mems[i][j] = -2; // open space, but it can't get to any other sector
				} else {
					voronoi_mems[i][j] = std::distance(dist.begin(), shortest_dist);
				}
				printf("%f,%f\n",*shortest_dist,voronoi_mems[i][j]);
				
			}
		}
	}

	PrintOut::toFile2D(voronoi_mems,"voronoi_mems.csv");


	//AStar_grid a(obstacles,members,0,1);

	// make an A* grid: base
	// filter based on given set



	/*
	std::map<int,std::map<int,AStar_grid> > m2astar;
	for (int i=0; i<conns.size(); i++){
	for (int j=i+1; j<conns[i].size(); j++){
	if (conns[i][j]>0){
	m2astar[i].insert(std::make_pair(j,AStar_grid(obstacles,members,i,j)));
	}
	}
	std::cout << i <<std::endl;
	}
	*/

	//printf("%i Astars created!!",m2astar.size());


	/*
	std::vector<AStar_grid> astars;
	for (int i=0; i<1000; i++){
	astars.push_back(AStar_grid(obstacles));
	std::cout << i << std::endl;
	}
	*/

	//	a.m.solve(0,0,1,1);

	// The default maze size is 20x10.  A different size may be specified on
	// the command line.
	/*
	if (a.m.solve(0,0,1,1))
	std::cout << "Solved the maze." << std::endl;
	else
	std::cout << "The maze is not solvable." << std::endl;
	std::cout << a.m << std::endl;*/

	system("pause");
	return 0;
}
