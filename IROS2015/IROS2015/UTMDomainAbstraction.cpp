#include "UTMDomainAbstraction.h"


UTMDomainAbstraction::UTMDomainAbstraction(bool deterministic):
	ATFMSectorDomain(deterministic, true) 
{

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
	
	matrix1d D;
	/*
	matrix3d l = getLoads();
	matrix1d D = matrix1d(n_agents);
	matrix3d c = connection_capacity;
	
	for (int i=0; i<n_agents; i++){
		double oc1 = 0.0;
		double oc2 = 0.0;
		for (int a=0; a<n_agents; a++){
			for (int t=0; t<UAV::NTYPES; t++){
				if (l[a][i][t]>c[a][i][t])
					oc1 += l[a][i][t] - c[a][i][t];
				if (sectors->at(i).average_load(a,t)>c[a][i][t])
					oc2 += sectors->at(i).average_load(a,t);// - c[a][i][t];
			}
		}
		D[i] = -(oc1*oc1 + oc2*oc2);
	}*/

	/*vector<Demographics> oldLoads = getLoads(); // get the current loads on all sectors
	vector<vector<Demographics> > allloads = vector<vector<Demographics> >(n_agents); // agent removed, agent for load, type
	for (unsigned int i=0; i< allloads.size(); i++){
		allloads[i] = oldLoads;
		allloads[i][i] = Demographics(UAV::NTYPES,0); // traffic removed, added back in later
	}

	// Count the adjusted load
	for(Sector s: *sectors){
		
		
		// build the map with the blacked-out sector
		planners->blockSector(s.sectorID);

		//for (std::shared_ptr<UAV> &u: s.toward){
		for (std::shared_ptr<UAV> &u : UAVs){
			// plan a path using a generic A* with modified weights
			int sid = s.sectorID;
			int oldsector = u->nextSectorID();
			if (sid!=oldsector) continue;

			int newnextsector = u->getBestPath(membership_map->at(u->loc.x,u->loc.y), membership_map->at(u->end_loc.x,u->end_loc.y)).front();
			allloads[s.sectorID][newnextsector][u->type_ID]++;
		}
		planners->unblockSector(); // reset the cost maps
		
	}
	

	// Calculate D from counterfactual
	vector<Demographics> C = vector<Demographics>(n_agents);// capacities[agent, type]
	for (Demographics &c: C){
		c = Demographics(UAV::NTYPES,0);
	}
	matrix1d D = matrix1d(n_agents);
	double G_reg = G(oldLoads,C);

	conflict_count = G_reg;

	for (int i=0; i<n_agents; i++){
		double G_c = G(allloads[i],C);
		D[i] = G_reg-G_c;
	}
	*/ 

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
	for (int i=0; i<cap.size(); i++){
		for (int j=0; j<cap[i].size(); j++){
			if (cap[i][j]<0){ // only count the amount over the capacity
				conflict_count += abs(cap[i][j]);
			}
		}
	}
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
	 // LEFT BLANK
}

void UTMDomainAbstraction::reset(){
	UAVs.clear();
	planners->reset();
	conflict_count = 0;
}