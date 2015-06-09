// Bar.cpp : Defines the entry point for the console application.
//
#include <vector>
#include "stdafx.h"

#define NSTATES 1
#define NACTIONS 3
#define QINIT 0.0
#define gamma 0.9

// 3-element bar problem


using namespace std;
class Agent{
public:
	int s;
	int a;
	int sprime;
	Agent()
	{
		Q = vector<vector<double> >(NSTATES);
		for (int i=0; i<NSTATES; i++) Q[i] = vector<double>(NACTIONS,QINIT);
	}
	vector<vector<double> > Q;
	void qupdate(double alpha, double R){
		Q[s][a] = (1-alpha)*Q[s][a]+alpha*(R+gamma*maxQ(sprime));
	}
	double maxQ(int sprime){
		double m=Q[sprime][0];
		for (int i=1; i<Q[sprime].size(); i++){
			if (m<Q[sprime][i]){
				m=Q[sprime][i];
			}
		}
	}
};

vector<int> beaches(NSTATES,0);


int _tmain(int argc, _TCHAR* argv[])
{
	

	return 0;
}

