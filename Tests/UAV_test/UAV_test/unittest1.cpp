#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../../libraries/Planning/AStarManager.h"
//#include "../../../../libraries/Domains/ATFMSectorDomain/UAV.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UAV_test
{		
	// FIRST WE NEED TO TEST OUR ASTAR CLASS
	TEST_CLASS(AStarManager)
	{
	public:
		TEST_METHOD(TestCreation)
		{
			int n_types; // the number of types a network should accommodate... rename to layers?
			std::vector<AStarManager::Edge> edges;
			Matrix<int,2>* membership_map;
			vector<XY> agent_locs;
			AStarManager a = ;
		}

	};
}