// Sandbox.cpp : Defines the entry point for the console application.
//

#include "../../libraries/FileIO/easyio/easyio.h"
#include "stdafx.h"

#include "AStar_grid.h"

int main (int argc, char const *argv[]){

	matrix2d m = FileManip::readDouble("C:/Users/Carrie/Documents/Visual Studio 2012/Projects/projects/IROS2015/IROS2015/agent_map/obstacle_map.csv");
	matrix2d mems = FileManip::readDouble("C:/Users/Carrie/Documents/Visual Studio 2012/Projects/projects/IROS2015/IROS2015/agent_map/membership_map.csv");
	matrix2d conns = FileManip::readDouble("C:/Users/Carrie/Documents/Visual Studio 2012/Projects/projects/IROS2015/IROS2015/agent_map/connections.csv");
	
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
	//AStar_grid a(obstacles,members,0,1);

	std::map<int,std::map<int,AStar_grid> > m2astar;
	for (int i=0; i<conns.size(); i++){
		for (int j=i+1; j<conns[i].size(); j++){
			if (conns[i][j]>0){
				m2astar[i].insert(std::make_pair(j,AStar_grid(obstacles,members,i,j)));
			}
		}
		std::cout << i <<std::endl;
	}

	printf("%i Astars created!!",m2astar.size());


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
