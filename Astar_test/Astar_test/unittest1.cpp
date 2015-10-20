//#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../libraries/Planning/AStarManager.h"
#include "../../../libraries/Planning/AStar_easy.h"
#include "../../../libraries/Planning/AStar_grid.h"
#include "../../../libraries/FileIO/easyio/easyio.h"
#include "../../../libraries/FileIO/easyio/readin.h"
#include "../../../libraries/FileIO/easyio/printing.h"
#include "../../../libraries/Math/easymath.h"
#include "../../../libraries/Domains/ATFMSectorDomain/ATFMSectorDomain.h"
#include "../../../libraries/Domains/ATFMSectorDomain/UAV.h"
#include "../../../libraries/Domains/ATFMSectorDomain/Fix.h"
#include "../../../libraries/Domains/ATFMSectorDomain/Sector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Astar_test
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// Read in files for sector management
			Matrix<int,2>* membership_map;
			Load::load_variable(&membership_map,"C:\Users\Carrie\Documents\Visual Studio 2012\Projects\projects\IROS2015\IROS2015\agent_map/membership_map.csv");
			
			matrix2d agent_coords = FileManip::readDouble("C:\Users\Carrie\Documents\Visual Studio 2012\Projects\projects\IROS2015\IROS2015\agent_map/agent_map.csv");
			matrix2d connection_map = FileManip::readDouble("C:\Users\Carrie\Documents\Visual Studio 2012\Projects\projects\IROS2015\IROS2015\agent_map/connections.csv");

			// Add sectors
			vector<XY> agent_locs = vector<XY>(agent_coords.size()); // save this for later Astar recreation
			for (unsigned int i=0; i<agent_coords.size(); i++){
				agent_locs[i] = XY(agent_coords[i][0],agent_coords[i][1]);
			}

			// Adjust the connection map to be the edges
			// preprocess boolean connection map -- MOVE THIS TO ASTAR
			vector<AStarManager::Edge> edges;
			for (unsigned int i=0; i<connection_map.size(); i++){
				for (unsigned int j=0; j<connection_map[i].size(); j++){
					if (connection_map[i][j] && i!=j){
						edges.push_back(AStarManager::Edge(i,j));
					}
				}
			}

			AStarManager * planners = new AStarManager(1, edges, membership_map, agent_locs);

			for (int i=0; i<membership_map->dim1(); i++){
				for (int j=0; j<membership_map->dim2(); j++){
					for (int k=0; k<membership_map->dim1(); k++){
						for (int l=0; l<membership_map->dim2(); l++){
							// should handle same point
							if ((*membership_map)(i,j)<0 || (*membership_map)(k,l)<0) continue; // skip obstacles ... todo, handle obstacles
							list<int> plan = planners->search(0,XY(i,j),XY(k,l));
							printf("(%i,%i) to (%i,%i): %i\n",i,j,k,l,plan.size());
						}
					}
				}
			}
		}

	};
}