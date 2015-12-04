#pragma once
#include <string>
#include <vector>
#include <agents.h>
#include "../../libraries/Math/easymath.h"
#include "../../libraries/FileIO/easyio/easyio.h"
#include "../../libraries/Planning/AStarAbstract.h"

using namespace std;
using namespace easymath;

class AirspaceMap
{
public:
	AirspaceMap(void);
	AirspaceMap(string agentFile, string edgeFile, int ntypes, double flatCapacity){
		// Read in files for sector management
		Load::loadVariable(agentLocs, agentFile);
		Load::loadVariable(edges, edgeFile);
		forEachPairing(agentLocs, agentLocs, connectionTime, manhattanDist); // gets the connection time: note this is also calculated for unconnected links
		sectorCapacity = matrix2d(agentLocs.size(),matrix1d(ntypes,flatCapacity) ); // Capacity for each type of agent
	}
	AirspaceMap(int nAgents, int ntypes, double flatCapacity){
		agentLocs = vector<XY>(nAgents);
		for (XY &l : agentLocs){
			l = XY(rand()%gridWorldSizeX,rand()%gridWorldSizeY);
		}

		// Construct a planar graph
		do {
			for (unsigned int i=0; i<2*agentLocs.size(); i++){ // scales with number of agents (tries to add 2x the number of agents before testing connectedness)
				// Add edge randomly
				int p1 = int(rand()%agentLocs.size());
				int p2 = int(rand()%agentLocs.size());
				pair<int,int> candidate = pair<int,int>(p1,p2);

				if (!intersectsExistingEdge(candidate)) edges.push_back(candidate);
			}
		} while (!fullyConnected());
		
		forEachPairing(agentLocs, agentLocs, connectionTime, manhattanDist); // gets the connection time: note this is also calculated for unconnected links
		sectorCapacity = matrix2d(agentLocs.size(),matrix1d(ntypes,flatCapacity) ); // Capacity for each type of agent
	}

	
	vector<XY> agentLocs;
	std::vector<std::pair<int,int> > edges;
	matrix2d connectionTime; // time of UAV for each connection (regardless of type)
	matrix2d sectorCapacity; // for the other type of reporting: [agent][type]
	
	static const int gridWorldSizeX=200;
	static const int gridWorldSizeY=200;

		
	bool intersectsExistingEdge(pair<int, int> candidate){
		for (pair<int,int> e:edges){
			if (intersects(pair<XY,XY>(agentLocs[e.first],agentLocs[e.second]),pair<XY,XY>(agentLocs[candidate.first],agentLocs[candidate.second]))){
				return true;
			}
		}
		return false;
	}

	bool fullyConnected(){
		AStarAbstract a = AStarAbstract(agentLocs,edges);
		
		for (int i=0; i<agentLocs.size(); i++){
			for (int j=0; j<agentLocs.size(); j++){
				list<AStarAbstract::vertex> path = a.search(i,j);
				if (path.empty()) return false;
			}
		}
		return true;
	}

	void printEdges(){
		string fileout = "agent_map/generatedEdges.csv";
		matrix2d edges_printable(edges.size());
		for (unsigned int i=0; i<edges.size(); i++){
			edges_printable[i].push_back(edges[i].first);
			edges_printable[i].push_back(edges[i].second);
		}

		PrintOut::toFile2D(edges_printable,fileout);
	}

	void printAgents(){
		string fileout = "agent_map/generatedAgents.csv";
		matrix2d agents_printable(agentLocs.size());
		for (unsigned int i=0; i<agentLocs.size(); i++){
			agents_printable[i].push_back(agentLocs[i].x);
			agents_printable[i].push_back(agentLocs[i].y);
		}

		PrintOut::toFile2D(agents_printable,fileout);
	}

	~AirspaceMap(void);
};

