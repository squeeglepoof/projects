// UtmAirspaceGenTest_with_outputs.cpp : Defines the entry point for the console application.
//

/**
This project will test the airspace generation for the UTM project.
Particularly it will test the ability of the code to write out to a 
file the domain, and to load that domain after writing.
*/

#include "stdafx.h"

#include "../../../../libraries/Planning/SectorGraphManager.h"
#include "../../../../libraries/Planning/LinkGraph.h"
#include "../../../../libraries/Planning/TypeGraphmanager.h"

//using namespace easymath;

int _tmain(int argc, _TCHAR* argv[])
{
	
	TypeGraphManager t(10,1,100,100);
	for (int i=0; i<10; i++){
		for (int j=0; j<10; j++){
			if (i==j) continue;
			std::list<int> b = t.astar(i,j,0);
			printf("size=%i\n",b.size());
			if (b.size()==1){
				printf("bad!");
			}
		}
	}
	printf("blah");

	//t.print_graph("");

	return 0;
}

