/*
This is an abstraction of the sector problem. There are several types of 
traffic in the system. They carry some amount of risk in conflicting with one
another. Agents must route them in order to minimize the number of conflicts 
arising in the domain.

chance of spatial paths crossing?

*/

class Traffic{
	Traffic(int type_id): type_id(type_id){
	}

	int type_id;
	double risk;
};


#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

