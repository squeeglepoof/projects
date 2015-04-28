// Sandbox.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "../../../libraries/FileIO/easyio/easyio.h"
#include "../../../libraries/Planning/AStar_easy.h"

#define OBSTACLES "../../IROS2015/IROS2015/agent_map/obstacle_map.csv"
#define MEMBERS  "../../IROS2015/IROS2015/agent_map/membership_map.csv"

using namespace std;

/*
class Edge{
// pass in a bunch of pointers to costs, linked to the relevant membership costs
Edge(int x, int y, int m1, int m2, double* c): x(x),y(y),m1(m1),m2(m2),c(c){};
~Edge(){};
const int x, y, m1, m2; // xy location, membership
double* c; // cost of traversal
};

void astar_test(){
matrix2d obstacles_double = FileManip::readDouble(OBSTACLES);
vector<vector<bool> > obstacles(obstacles_double.size());
for (int i=0; i<obstacles_double.size(); i++){
obstacles[i] = vector<bool>(obstacles_double[i].size());
for (int j=0; j<obstacles_double[i].size(); j++){
obstacles[i][j] = (obstacles_double[i][j]>0)?true:false;
}
}
matrix2d members = FileManip::readDouble(MEMBERS);

int directions[15][15]{

}

// MAKE A SINGLE LOW-LEVEL FULLY-CONNECTED MAP (TO STORE EDGES)
vector<Edge> edges; // [xy 1, xy 2, cost]
for (int i=0; i<obstacles.size(); i++){
for (int j=0; j<obstacles[i].size(); j++){
edges.push_back(Edge(i,j,members[i][j],weights[members[i][j]]);
}
}
}
*/


vector<XY> get8GridNeighbors(int x, int y, vector<vector<bool> >* obstacle_map){
	int XDIM = obstacle_map->size();
	int YDIM = obstacle_map->at(0).size();
	vector<XY> neighbors(8);
	int n = 0;

	if (x>0			&&	y>0			&&	!obstacle_map->at(x-1)[y-1])	neighbors[n++] = XY(x-1,y-1);
	if (x>0			&&					!obstacle_map->at(x-1)[y])		neighbors[n++] = XY(x-1,y);
	if (x>0			&&	y<YDIM-1	&&	!obstacle_map->at(x-1)[y+1])	neighbors[n++] = XY(x-1,y+1);
	if (				y>0			&&	!obstacle_map->at(x)[y-1])		neighbors[n++] = XY(x,y-1);
	if (				y<YDIM-1	&&	!obstacle_map->at(x)[y+1])		neighbors[n++] = XY(x,y+1);
	if (x<XDIM-1	&&	y>0			&&	!obstacle_map->at(x+1)[y-1])	neighbors[n++] = XY(x+1,y-1);
	if (x<XDIM-1	&&					!obstacle_map->at(x+1)[y])		neighbors[n++] = XY(x+1,y);
	if (x<XDIM-1	&&	y<YDIM-1	&&	!obstacle_map->at(x+1)[y+1])	neighbors[n++] = XY(x+1,y+1);

	neighbors.resize(n);

	return neighbors;
}

/*int sub2ind(const int row,const int col,const int cols,const int rows)
{
   return (row-1)*cols+col;
}

void ind2sub(const int sub,const int cols,const int rows,int &row,int &col)
{
   row=sub/cols;
   col=sub%cols;
}*/

int sub2ind(int r, int c, int m, int n){ 
	// 1-index!
	r++;
	c++;
	return (c-1)*m+r;
}

void ind2sub(int cols, int ind, int &r, int &c){
	//0-indexed: ind-1 always called
	r = (ind-1)%cols;
	c = floor((ind-1)/cols);
}


int _tmain(int argc, _TCHAR* argv[])
{
	matrix2d obstacles_double = FileManip::readDouble(OBSTACLES);
	vector<vector<bool> > obstacles(obstacles_double.size());
	for (int i=0; i<obstacles_double.size(); i++){
		obstacles[i] = vector<bool>(obstacles_double[i].size());
		for (int j=0; j<obstacles_double[i].size(); j++){
			obstacles[i][j] = obstacles_double[i][j]>0.0;
		}
	}
	matrix2d members = FileManip::readDouble(MEMBERS);

	// Get the fully-connected version, then save this to a file

	matrix2d edge_array;

	// create connections on map
	int XDIM = obstacles.size();
	int YDIM = obstacles[0].size();
	for (int x=0; x<XDIM; x++){
		for (int y=0; y<YDIM; y++){
			if (!obstacles[x][y]){
				vector<XY> neighbors = get8GridNeighbors(x,y, &obstacles);
				for (int i=0; i<neighbors.size(); i++){
					matrix1d edge(2); // edge is 1-indexed (matlab interfacing...)
					edge[0] = sub2ind(x,y,YDIM,XDIM);
					edge[1] = sub2ind(neighbors[i].x,neighbors[i].y,YDIM,XDIM);
					edge_array.push_back(edge);
				}
			}
		}
	}

	/*PrintOut::toFile2D(edge_array,"edges.csv");
	
	for (int i=0; i<edge_array.size(); i++){
		int r, c;
		ind2sub(256, edge_array[i][0], r,c);
		if (obstacles[r][c]>0){
			printf("Fail! %i,%i",r,c);
			system("pause");
			exit(1);
		}
	}*/


	system("pause");
	return 0;
}

