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

// for memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// warning disabling
//#pragma warning(push) 
//#pragma warning(disable:4996) 

// Parallelization
//#include <omp.h>

// Standard includes
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

// Project-specific includes
//#include "../../../libraries/Simulation/SimNE.h"
#include "../../../libraries/Simulation/SimTypeNE.h"
#include "../../../libraries/Domains/ATFMSectorDomain/ATFMSectorDomain.h"





void program(int calls, MultiagentTypeNE::TypeHandling sim_mode, std::string rwd_name, std::string conflict_name){
	srand(time(NULL));
	ATFMSectorDomain* domain = new ATFMSectorDomain();

	// FOR DEBUGGING
	

	NeuroEvoParameters* NE_params = new NeuroEvoParameters(domain->n_state_elements,domain->n_control_elements);
	MultiagentNE* MAS = new MultiagentNE(domain->n_agents, NE_params);
	// END FOR DEBUGGING

	SimTypeNE sim(domain, MAS, sim_mode); // FOR DEBUGGING
	sim.runExperiment();

	sim.outputRewardLog(rwd_name+to_string(calls)+".txt");
	sim.outputMetricLog(conflict_name+to_string(calls)+".txt");
	delete ((ATFMSectorDomain*)domain);

}

void metaprog(){
std::string rwd_names[MultiagentTypeNE::TypeHandling::NMODES] = {
	"stat_results/weighted_reward-",
	"stat_results/crossweighted_reward-",
	"stat_results/multimind_reward-",
	"stat_results/blind_reward-"
};

std::string conflict_names[MultiagentTypeNE::TypeHandling::NMODES] = {
	"stat_results/weighted_conflict-",
	"stat_results/crossweighted_conflict-",
	"stat_results/multimind_conflict-",
	"stat_results/blind_conflict-"
};

	for (int r=0; r<5; r++){
		printf("************* RUN %i STARTING ***********\n",r);
	#pragma omp parallel for
		for (int i=0; i<MultiagentTypeNE::NMODES; i++){
			printf("mode type %i started. ", i);
			program(r,MultiagentTypeNE::TypeHandling(i), rwd_names[i], conflict_names[i]);
		}
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	metaprog();
	_CrtDumpMemoryLeaks(); // memory leak checking
	system("pause");
	return 0;
}

