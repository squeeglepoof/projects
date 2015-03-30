// IROS2015.cpp : Defines the entry point for the console application.
//

/******************************************************************************
| This contains the code used for the submission to IROS 2015 entitled		  |
| "Learning to Trick Cost-Based Planners into Cooperative Behavior"		      |
|																			  |
| Redistributables are also included, as well as past experimental data.	  |
|																			  |
| Create a new stat_results file for output if desired.						  |
|																			  |
******************************************************************************/

#include "stdafx.h"


// warning disabling
//#pragma warning(push) 
//#pragma warning(disable:4996) 

// Standard includes
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

// Project-specific includes
//#include "../../../libraries/Simulation/SimNE.h"
#include "../../../libraries/Simulation/SimTypeNE.h"
#include "../../../libraries/Domains/ATFMSectorDomain/ATFMSectorDomain.h"

// for memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


void program(int calls){
	srand(time(NULL));
	ATFMSectorDomain* domain = new ATFMSectorDomain();
	SimTypeNE sim(domain, MultiagentTypeNE::WEIGHTED);
	sim.runExperiment();

	sim.outputRewardLog("stat_results/reward-"+to_string(calls)+".txt");
	sim.outputMetricLog("stat_results/conflictlog-"+to_string(calls)+".txt");
	delete ((ATFMSectorDomain*)domain);
}

int _tmain(int argc, _TCHAR* argv[])
{
	for (int i=0; i<1; i++){
		printf("************* RUN %i STARTING ***********\n",i);
		program(i);
	}
//	_CrtDumpMemoryLeaks(); // memory leak checking
	system("pause");
	return 0;
}

