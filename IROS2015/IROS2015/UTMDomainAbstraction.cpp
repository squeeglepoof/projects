#include "UTMDomainAbstraction.h"


UTMDomainAbstraction::UTMDomainAbstraction(bool deterministic):
	ATFMSectorDomain(deterministic, true) 
{
	for (int i=0; i<sectors->size(); i++){
		sectors->at(i).conflicts = vector<int>(4,0);
	}
	rewardType = GLOBAL;
	
	// Planning
	planners = new AStarManager(UAV::NTYPES, edges,membership_map, agent_locs,true); //NOTE: MAY NOT HAVE TO MAKE A DIFFERENT ONE FOR ABSTRACTION???

	// initialize fixes
	for (unsigned int i=0; i<sectors->size(); i++){
		fixes->push_back(Fix(sectors->at(i).xy,i,is_deterministic,planners));
	}
	
	connection_capacity = matrix3d(n_agents, matrix2d(n_agents, matrix1d (UAV::NTYPES,2.0))); // note: unused
	sector_capacity = matrix2d(n_agents,matrix1d(UAV::NTYPES,2.0) ); // arbitrary/flat sector capacity assignment

	edge_time = vector<vector<int> >(n_agents,vector<int>(n_agents,10));
	for_each_pairing(agent_locs, agent_locs, connection_time, manhattan_dist); // gets the connection time: note this is also calculated for unconnected links
}


UTMDomainAbstraction::~UTMDomainAbstraction(void)
{
}

double UTMDomainAbstraction::getGlobalReward(){
	return -conflict_count; // REPLACE THIS LATER
}

matrix1d UTMDomainAbstraction::getPerformance(){
	return matrix1d(n_agents,getGlobalReward());
}

matrix1d UTMDomainAbstraction::getDifferenceReward(){
	// REMOVE THE AGENT FROM THE SYSTEM
	
	matrix1d D(n_agents,0.0);
	
	// METHOD 1: INFINITE LINK COSTS (*resim)
	// METHOD 2: STATIC LINK COSTS (*resim)
	// METHOD 3: HAND-CODED LINK COSTS (*resim)
	// METHOD 4: DOWNSTREAM EFFECTS REMOVED
	// METHOD 5: UPSTREAM AND DOWNSTREAM EFFECTS REMOVED
	// METHOD 6: RANDOM TRAFFIC REALLOCATION
	// METHOD 7: CONFLICTS AVERAGED OVER THE NODE'S HISTORY
	// METHOD 8: LOCAL REWARD (DIFFERENCE FROM LINEAR REWARD)
	for (int i=0; i<sectors->size(); i++){
		for (int j=0; j<UAV::NTYPES; j++){
			D[i] += sectors->at(i).conflicts[j];
		}
	}
	
	return D;
}

matrix1d UTMDomainAbstraction::getRewards(){
	// Calculate loads
	if (rewardType==GLOBAL){
		return matrix1d(n_agents, getGlobalReward());
	} else if (rewardType==DIFFERENCE){
		return getDifferenceReward();
	} else {
		printf("No reward type set.");
		system("pause");
		exit(1);
	}
}

void UTMDomainAbstraction::incrementUAVPath(){
	// in abstraction mode, move to next center of target
	for (std::shared_ptr<UAV> &u: UAVs){
		if (u->t <=0){
			u->high_path_prev.pop_front();
			u->loc = sectors->at(u->high_path_prev.front()).xy;
		} else {
			u->t--;
		}
	}
}

void UTMDomainAbstraction::detectConflicts(){
	// count the over capacity here

	matrix2d cap = sector_capacity;
	for (std::shared_ptr<UAV> &u: UAVs){
		cap[getSector(u->loc)][u->type_ID]--;
	}

	int current_conflicts = 0;
	for (int i=0; i<cap.size(); i++){
		for (int j=0; j<cap[i].size(); j++){
			if (cap[i][j]<0){ // only count the amount over the capacity
				conflict_count += abs(cap[i][j]);
				current_conflicts += abs(cap[i][j]);
				sectors->at(i).conflicts[j]++;
			}
		}
	}
	
	//printf("conflicts=%f\n",double(conflict_count));

	traffic_log.push_back(current_conflicts);
}

void UTMDomainAbstraction::getPathPlans(){
	
	for (std::shared_ptr<UAV> &u : UAVs){
		int memstart = membership_map->at(u->loc.x,u->loc.y);
		int memend = membership_map->at(u->end_loc.x,u->end_loc.y);
		u->planAbstractPath(connection_time,memstart, memend);
	}
}

void UTMDomainAbstraction::getPathPlans(std::list<std::shared_ptr<UAV> > &new_UAVs){

	for (std::shared_ptr<UAV> &u : new_UAVs){
		int memstart = membership_map->at(u->loc.x,u->loc.y);
		int memend = membership_map->at(u->end_loc.x,u->end_loc.y);
		u->planAbstractPath(connection_time,memstart, memend); // sets own next waypoint
	}
}

void UTMDomainAbstraction::exportLog(std::string fid, double G){
	PrintOut::toFile1D(traffic_log,"traffic_log.csv");
	exit(1);
}

void UTMDomainAbstraction::reset(){
	UAVs.clear();
	planners->reset();
	conflict_count = 0;
	for (int i=0; i<sectors->size(); i++){
		sectors->at(i).conflicts = vector<int>(4,0);
	}
}