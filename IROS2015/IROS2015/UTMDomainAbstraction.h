#pragma once
#include "c:\users\carrie\documents\visual studio 2012\projects\libraries\domains\atfmsectordomain\atfmsectordomain.h"
class UTMDomainAbstraction :
	public ATFMSectorDomain
{
public:
	UTMDomainAbstraction(bool deterministic);
	~UTMDomainAbstraction(void);
	std::vector<std::vector<int> > edge_time; // time that you need to be on an edge
	matrix2d connection_time;
	std::vector<std::vector<std::vector<int> > > connection_capacity; // capacity for each type of UAV [connection][connection][type]

	matrix1d getPerformance();
	matrix1d getRewards();
	void incrementUAVPath();
	void detectConflicts();
	void getPathPlans();
	void getPathPlans(std::list<std::shared_ptr<UAV> > &new_UAVs);
};

