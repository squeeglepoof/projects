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
	double run_g;
	matrix1d run_d;
	matrix3d connection_capacity; // capacity for each type of UAV [connection][connection][type]
	matrix1d differenceReward();
	enum {GLOBAL, DIFFERENCE};
	int rewardType;
	matrix2d sectorTraffic; //[sector][epoch]

	virtual matrix1d getPerformance();
	virtual matrix1d getRewards();
	virtual void incrementUAVPath();
	virtual void detectConflicts();
	virtual void getPathPlans();
	virtual void getPathPlans(std::list<std::shared_ptr<UAV> > &new_UAVs);
	virtual void exportLog(std::string fid, double G);
	virtual void reset();
};

