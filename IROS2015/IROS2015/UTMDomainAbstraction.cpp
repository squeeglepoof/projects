#include "UTMDomainAbstraction.h"


UTMDomainAbstraction::UTMDomainAbstraction(bool deterministic):
	ATFMSectorDomain(deterministic) 
{
	pathTraces = new vector<vector<XY> >(); // traces the path (reset each epoch)

	// Object creation
	sectors = new vector<Sector>();
	fixes = new vector<Fix>();

	// inheritance elements: constant
	//n_control_elements=4; // 4 outputs for sectors (cost in cardinal directions) (no types)
	
	n_control_elements=4*UAV::NTYPES;
	n_state_elements=4; // 4 state elements for sectors ( number of planes traveling in cardinal directions)
	n_steps=100; // steps of simulation time
	n_types=UAV::NTYPES;

	// Read in files for sector management
	Load::load_variable(&membership_map,"agent_map/membership_map.csv");
	matrix2d agent_coords = FileManip::readDouble("agent_map/agent_map.csv");
	matrix2d connection_map = FileManip::readDouble("agent_map/connections.csv");


	// matrix2d fix_locs = FileManip::readDouble("agent_map/fixes.csv"); // removed, abstraction
	
	// ABSTRACTION MODE DIFF BEGIN
	matrix2d fix_locs = agent_coords; // if using an abstraction, have only the centers phsyically located
	//END
	

	// Add sectors
	agent_locs = vector<XY>(agent_coords.size()); // save this for later Astar recreation
	for (unsigned int i=0; i<agent_coords.size(); i++){
		sectors->push_back(Sector(XY(agent_coords[i][0],agent_coords[i][1]),i));
		agent_locs[i] = sectors->back().xy;
	}

	// Adjust the connection map to be the edges
	// preprocess boolean connection map
	vector<AStarManager::Edge> edges;
	for (unsigned int i=0; i<connection_map.size(); i++){
		for (unsigned int j=0; j<connection_map[i].size(); j++){
			if (connection_map[i][j] && i!=j){
				edges.push_back(AStarManager::Edge(i,j));
			}
		}
	}

	planners = new AStarManager(UAV::NTYPES, edges, membership_map, agent_locs);

	// initialize fixes
	for (unsigned int i=0; i<fix_locs.size(); i++){
		fixes->push_back(Fix(XY(fix_locs[i][0],fix_locs[i][1]),i,is_deterministic,planners));
	}

	n_agents = sectors->size(); // number of agents dictated by read in file
	fixed_types=vector<int>(n_agents,0);

	conflict_count = 0; // initialize with no conflicts
	conflict_count_map = new ID_grid(planners->obstacle_map->dim1(), planners->obstacle_map->dim2());

	connection_capacity = vector<vector<vector<int> > > (n_agents, vector<vector<int> >(n_agents, vector<int> (UAV::NTYPES,10)));
	edge_time = vector<vector<int> >(n_agents,vector<int>(n_agents,10));
	for_each_pairing(agent_locs, agent_locs, connection_time, manhattan_dist);
}


UTMDomainAbstraction::~UTMDomainAbstraction(void)
{
}

matrix1d UTMDomainAbstraction::getPerformance(){
	return getRewards();
}

matrix1d UTMDomainAbstraction::getRewards(){
	// Calculate loads
	vector<Demographics> oldLoads = getLoads(); // get the current loads on all sectors
	vector<vector<Demographics> > allloads = vector<vector<Demographics> >(n_agents); // agent removed, agent for load, type
	for (unsigned int i=0; i< allloads.size(); i++){
		allloads[i] = oldLoads;
		allloads[i][i] = Demographics(UAV::NTYPES,0); // traffic removed, added back in later
	}

	// Count the adjusted load
	for(Sector s: *sectors){
		// build the map with the blacked-out sector
		planners->blockSector(s.sectorID);

		for (std::shared_ptr<UAV> &u: s.toward){
			// plan a path using a generic A* with modified weights
			int newnextsector = u->getBestPath().front();
			allloads[s.sectorID][newnextsector][u->type_ID]++;
		}
		planners->unblockSector(); // reset the cost maps

	}

	// Calculate D from counterfactual
	vector<Demographics> C = vector<Demographics>(n_agents);// capacities[agent, type]
	for (Demographics &c: C){
		c = Demographics(UAV::NTYPES,10);
	}
	matrix1d D = matrix1d(n_agents);
	for (int i=0; i<n_agents; i++){
		double G_reg = G(oldLoads,C);
		double G_c = G(allloads[i],C);
		D[i] = G_reg-G_c;
	}
	// 

	return D; // global reward
}

void UTMDomainAbstraction::incrementUAVPath(){
	// in abstraction mode, move to next center of target
	for (std::shared_ptr<UAV> &u: UAVs){
		if (u->time_left_on_edge <=0){
			u->loc = sectors->at(*u->high_path_prev.begin()).xy;
			u->high_path_prev.pop_front();
		} else {
			u->time_left_on_edge--;
		}
	}
}

void UTMDomainAbstraction::detectConflicts(){
	// intentionally left blank - conflict counting done in the reward and based on overcap
}

void UTMDomainAbstraction::getPathPlans(std::list<std::shared_ptr<UAV> > &new_UAVs){
	for (std::shared_ptr<UAV> &u : UAVs){
		u->planAbstractPath(connection_time); // sets own next waypoint
	}
}