// Mistral.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include "../../../libraries/FileIO/easyio/easyio.h"

#define AGENTS 100
#define NIGHTS 5
#define STEPS 10000 // steps to move from bar to bar
#define C 7.0 // capacity
#define GAMMA 0.9
#define ALPHA 0.1
#define EPSILON 0.05
#define COIN double(rand())/double(RAND_MAX+1)
enum ACTIONS{LEFT, STAY, RIGHT, NACTIONS};
enum TYPES{LEFTONLY, BI, RIGHTONLY, NTYPES};
enum REWARDS{LOCAL, GLOBAL, DIFFERENCE, NREWARDS};
//#define NSTATES NTYPES*NIGHTS
#define NSTATES NIGHTS

using namespace std;

// agents
int nights[NIGHTS];
int types[AGENTS]; // 0 = bi, 1 = left only, 2 = right only
int actions[AGENTS];
int states_old[AGENTS]; // states at last timestep
int states[AGENTS]; // size = NTYPES*NIGHTS // state = agent location, agent type (ignore this for blind)
double qtables[AGENTS][NSTATES][NACTIONS];
int positions[AGENTS]; // this is for rewards
double rewards[AGENTS];
int reward_mode = GLOBAL;
double alpha, epsilon;

vector<double> G_log;

static int sub2ind(int r, int c, int m, int n){ 
	// 1-index!
	r++;
	c++;
	return ((c-1)*m+r)-1; // zero indexing
}
static void ind2sub(int cols, int ind, int &r, int &c){
	//0-indexed: ind-1 always called
	r = (ind-1)%cols;
	c = (ind-1)/cols; // floor function removed
}

void getStates(){
	memcpy(states_old,states,AGENTS*sizeof(int)); // copy over old states
	for (int i=0; i<AGENTS; i++){ // tally up other agents
		//states[i] = sub2ind(types[i],positions[i],NTYPES,NIGHTS);
		states[i] = positions[i];
	}
}

void setOnBeaches(){
	// places agents on beaches
	for (int i=0; i<AGENTS; i++){
		if (i<AGENTS/2){
			positions[i] = 1;
		} else {
			positions[i] = 3;
		}
	}

	for (int i=0; i<NIGHTS; i++){
		if (i==1 || i==3) nights[i] = AGENTS/2;
		else nights[i]=0;
	}
}


void initialize(){
	// set agents on nights
	alpha=ALPHA;
	epsilon=EPSILON;
	setOnBeaches();

	// initialize Q
	for (int i=0; i<AGENTS; i++){
	//	int n = rand()%NIGHTS;
	//	nights[n]++;
	//	positions[i] = n;
		for (int s=0; s<NSTATES; s++){
			for (int a=0; a<NACTIONS; a++){
				qtables[i][s][a] = -1.0;
			}
		}
	}
	getStates(); // initial states populates
	G_log.clear();
}

double maxQ(int a, int s, int &ind){
	double best_val = -double(INT_MAX);
	vector<int> best_inds;
	for (int i=0; i<NACTIONS; i++){
		if (qtables[a][s][i]>best_val){
			best_val = qtables[a][s][i];
			best_inds.clear();
			best_inds.push_back(i);
		} else if (qtables[a][s][i]==best_val){
			best_inds.push_back(i);
		}
		qtables[a][s][i];
	}
	ind = best_inds[rand()%best_inds.size()];
	return best_val;
}

void getActions(){
	// find best action
	for (int i=0; i<AGENTS; i++){
		int k;
		if (COIN>epsilon){
			double best_val = -double(INT_MAX);
			int j = states[i];
			maxQ(i,j,k); // gets the index for the best action k, given i and j
		} else{
			k = rand()%NACTIONS;
		}
		actions[i] = k;
	}
}

void move(int a, int dir){
	// remove from current position
	nights[positions[a]]--;

	int adj = (dir - 1); // turns it into a motion
	int newpos = positions[a]+adj;

	if (newpos>=NIGHTS) newpos-=NIGHTS;
	else if (newpos<0) newpos+=NIGHTS;
		
	nights[newpos]++;
	positions[a] = newpos;
}

void enactActions(){
	for (int i=0; i<AGENTS; i++){
		move(i,actions[i]);
	}
}

double getGlobalReward(double L[], double D[]){
	double G = 0;
	for (int i=0; i<AGENTS; i++){
		double x = double(nights[positions[i]]);
		// get the reward for the night
		L[i] = x*exp(-x/C);
		G+=exp(-x/C);//L[i];
		D[i] = L[i] - (x-1)*exp(-(x-1)/C);
	}
	return G;
}

void getRewards(){
	// LOCAL REWARD

	//e^-x/c;
	double L[AGENTS];
	double D[AGENTS];
	double G = getGlobalReward(L,D);

	for (int i=0; i<AGENTS; i++){
		if (reward_mode==GLOBAL){
			rewards[i] = G;
		} else if (reward_mode==LOCAL){
			rewards[i] = L[i];
		} else if (reward_mode==DIFFERENCE){
			rewards[i] = D[i];
		} else {
			printf("unrecognized reward type");
			system("pause");
		}
	}

	G_log.push_back(G);
}

void updateQ(){
	for (int i=0; i<AGENTS; i++){
		double Qold = qtables[i][states_old[i]][actions[i]];
		double R = rewards[i];
		int max_index; // dummy
		double Qnew = Qold + alpha*(R+GAMMA*maxQ(i,states[i],max_index)-Qold);
		qtables[i][states_old[i]][actions[i]]=Qnew;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	// gather rewards
	// print out rewards
	// plot rewards
	// blind + not blind
	double alpha_decay  = 0.9999;
	double eps_decay = 0.9999;
	vector<vector<double> > allGs;
	for (int r=0; r<NREWARDS; r++){
		reward_mode = r;
		initialize();
		for (int i=0; i<STEPS; i++){
			int end_timestep = 1;
			setOnBeaches();
			getStates();
			for (int t=0; t<end_timestep; t++){
				getActions();
				enactActions();
				getRewards();
				getStates();
				updateQ();
				for (int i=0; i<NIGHTS; i++){
					printf("%i\t",nights[i]);
				}
				printf ("%\t %f",G_log.back());
				printf("\n");
			}
			alpha*=alpha_decay;
			epsilon*=eps_decay;
		}
		allGs.push_back(G_log);
		//PrintOut::toFile1D(G_log,"G_log.txt");
	}
	PrintOut::toFile2D(allGs,"G_log.txt");

	return 0;
}

