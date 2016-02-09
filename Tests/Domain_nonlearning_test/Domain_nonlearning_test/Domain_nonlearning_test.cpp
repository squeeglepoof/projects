// Domain_nonlearning_test.cpp : Defines the entry point for the console application.
//

// TODO: get this in an actual testing file

#include "stdafx.h"

#include "../../../../libraries/Domains/UTM/UTMModesAndFiles.h"

#include "../../../../libraries/Domains/UTM/UAV.h"

#include "../../../../libraries/Domains/UTM/Link.h"

#include "../../../../libraries/Domains/UTM/Sector.h"

#include "../../../../libraries/Domains/UTM/Fix.h"

#include "../../../../libraries/Domains/UTM/UTMDomainAbstract.h"

int _tmain(int argc, _TCHAR* argv[])
{
	UTMModes * u = new UTMModes();
	u->getRewardModeName();
	u->get_conflict_thresh();
	u->get_dist_thresh();
	u->get_flat_capacity();
	u->get_gen_rate();
	u->get_n_agents();
	u->get_n_control_elements();
	u->get_n_links();
	u->get_n_sectors();
	u->get_n_state_elements();
	u->get_n_steps();
	u->get_n_types();
	u->get_p_gen();

	UTMFileNames* u1 = new UTMFileNames();
//	u1->createDomainDirectory(); // non-functional
	u1->createExperimentDirectory();

	UTMDomainAbstract d(u);
	int* step = new int(0);
	d.synch_step(step);
	d.simulateStep(easymath::zeros(d.n_agents,d.n_state_elements));

	system("pause");

	return 0;
}