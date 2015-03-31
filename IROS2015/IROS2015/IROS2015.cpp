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

	sim.outputRewardLog("stat_results/weighted_reward-"+to_string(calls)+".txt");
	sim.outputMetricLog("stat_results/weighted_conflictlog-"+to_string(calls)+".txt");
	delete ((ATFMSectorDomain*)domain);
}

void program2(int calls){
	srand(time(NULL));
	ATFMSectorDomain* domain = new ATFMSectorDomain();
	SimTypeNE sim(domain, MultiagentTypeNE::CROSSWEIGHTED);
	sim.runExperiment();

	sim.outputRewardLog("stat_results/crossweighted_reward-"+to_string(calls)+".txt");
	sim.outputMetricLog("stat_results/crossweighted_conflictlog-"+to_string(calls)+".txt");
	delete ((ATFMSectorDomain*)domain);
}

void program3(int calls){
	srand(time(NULL));
	ATFMSectorDomain* domain = new ATFMSectorDomain();
	SimTypeNE sim(domain, MultiagentTypeNE::MULTIMIND);
	sim.runExperiment();

	sim.outputRewardLog("stat_results/multimind_reward-"+to_string(calls)+".txt");
	sim.outputMetricLog("stat_results/multimind_conflictlog-"+to_string(calls)+".txt");
	delete ((ATFMSectorDomain*)domain);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i=0;
	//for (int i=0; i<5; i++){
		printf("************* RUN %i STARTING ***********\n",i);
		//program(i);
		program2(i);
		//program3(i);
	//}
//	_CrtDumpMemoryLeaks(); // memory leak checking
	system("pause");
	return 0;
}

