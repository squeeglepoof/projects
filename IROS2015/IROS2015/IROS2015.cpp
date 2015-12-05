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
#include "../../../libraries/Domains/UTM/UTMDomainDetail.h"
#include "../../../libraries/Domains/UTM/UTMDomainAbstract.h"


void loopOverTypeHandling(){
	std::string rwd_names[MultiagentTypeNE::TypeHandling::NMODES] = {
		"stat_results/blind_reward-",
		"stat_results/weighted_reward-",
		"stat_results/crossweighted_reward-",
		"stat_results/multimind_reward-",
	};

	std::string conflict_names[MultiagentTypeNE::TypeHandling::NMODES] = {
		"stat_results/blind_conflict-",
		"stat_results/weighted_conflict-",
		"stat_results/crossweighted_conflict-",
		"stat_results/multimind_conflict-",
	};

	for (int i=0; i<MultiagentTypeNE::NMODES; i++){
		for (int r=0; r<5; r++){
			printf("************* RUN %i STARTING ***********\n",r);
			printf("mode type %i started. ", i);
			UTMDomainAbstract* domain = new UTMDomainAbstract();

			NeuroEvoParameters* NE_params = new NeuroEvoParameters(domain->n_state_elements,domain->n_control_elements);
			MultiagentTypeNE* MAS = new MultiagentTypeNE(domain->n_agents, NE_params, MultiagentTypeNE::TypeHandling(i),domain->n_types);

			SimTypeNE sim(domain, MAS, MultiagentTypeNE::TypeHandling(i));
			sim.runExperiment();

			sim.outputRewardLog(rwd_names[i]+to_string(r)+".csv");
			sim.outputMetricLog(conflict_names[i]+to_string(r)+".csv");
			delete ((UTMDomainAbstract*)domain);
		}
	}
}


vector<int> consecutive(int a, int b){
	// inclusive
	vector<int> v;
	for (; a<=b; a++) v.push_back(a);
	return v;
}

void loopOverDomainParameters(std::string* fileoutnames, void domainChanger(UTMDomainAbstract*, int val), int nparams){
	vector<int> vals = consecutive(0,nparams-1); // meant for use with enums
	for (int r=0; r<5; r++){
		for (int val : vals){
			printf("RUN %i STARING \n",r);
			srand(unsigned int(time(NULL)));
			UTMDomainAbstract* domain = new UTMDomainAbstract();
			domainChanger(domain, val);

			NeuroEvoParameters* NE_params = new NeuroEvoParameters(domain->n_state_elements,domain->n_control_elements);
			MultiagentTypeNE* MAS = new MultiagentTypeNE(domain->n_agents, NE_params, MultiagentTypeNE::BLIND,domain->n_types);

			SimTypeNE sim(domain, MAS, MultiagentTypeNE::BLIND);
			sim.runExperiment();

			sim.outputMetricLog(fileoutnames[val]+to_string(r)+".csv");
			delete ((UTMDomainAbstract*)domain);
		}
	}
}

void rwdChanger(UTMDomainAbstract* domain, int rwd){
	domain->_reward_mode = UTMDomainAbstract::RewardMode(rwd);
}

void loopOverRewardTypes(){
	std::string rwd_names[UTMDomainAbstract::RewardMode::NMODES] = {
		"stat_results/GLOBAL-", 
		"stat_results/DIFFERENCE_DOWNSTREAM-",
		"stat_results/DIFFERENCE_TOUCHED-",
		"stat_results/DIFFERENCE_REALLOC-",
		"stat_results/DIFFERENCE_AVG-",
	};

	loopOverDomainParameters(rwd_names, rwdChanger, UTMDomainAbstract::RewardMode::NMODES);
}


int _tmain(int argc, _TCHAR* argv[])
{
	loopOverRewardTypes();
	_CrtDumpMemoryLeaks(); // memory leak checking
	system("pause");
	return 0;
}

