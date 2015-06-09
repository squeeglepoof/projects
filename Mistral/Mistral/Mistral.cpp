// Mistral.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include "../../../libraries/FileIO/easyio/easyio.h"

#define AGENTS 50
#define NIGHTS 5
#define STEPS 500 // steps to move from bar to bar
#define C 4.0 // capacity
#define GAMMA 0.7
#define ALPHA 0.03
#define EPSILON 0.03
enum ACTIONS{LEFT, STAY, RIGHT, NACTIONS};
enum TYPES{LEFTONLY, BI, RIGHTONLY, NTYPES};
enum REWARDS{LOCAL, GLOBAL, DIFFERENCE, NREWARDS};
#define NSTATES NTYPES*NIGHTS
//#define NSTATES NIGHTS

using namespace std;

// agents

class Agent{
public:
	Agent(){
		static int calls = 0;

		type = calls%NTYPES;
		calls++;


		action = -1;
		//state_old = -1;
		state_old = 0;
		state = 0;
		//state = -1;
		position = -1;
		reward = -1.0;
		qtable = vector<vector<double> >(NSTATES);
		for (int i=0; i<NSTATES; i++){
			qtable[i] = vector<double>(NACTIONS,-1.0);
		}
		count=vector<int>(NACTIONS);
	}
	double * qold(){
		int sold = state_old;
		/*if (sold !=1){
			printf("illegal");
			system("pause");
		}*/
		return &qtable[state_old][action];
	}

	vector<int> count;
	int type;
	int action;
	int state_old;
	int state;
	vector<vector<double> > qtable; //qtable[NSTATES][NACTIONS];
	int position;
	double reward;
};

vector<Agent> agents;
vector<int> nights;

/*
int types[AGENTS]; // 0 = bi, 1 = left only, 2 = right only
int actions[AGENTS];
int states_old[AGENTS]; // states at last timestep
int states[AGENTS]; // size = NTYPES*NIGHTS // state = agent location, agent type (ignore this for blind)
double qtables[AGENTS][NSTATES][NACTIONS];
int positions[AGENTS]; // this is for rewards
double rewards[AGENTS];*/


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
	
	for (int i=0; i<AGENTS; i++){
		agents[i].state_old = agents[i].state;
		agents[i].state = agents[i].position;
		// some kind of type indication here? -- nothing to fix

		//int sold = agents[i].state_old;
		//int scur = agents[i].state;
		//int blah=1;
	}
	
}

void setOnBeaches(){
	// places agents on beaches
	for (int i=0; i<AGENTS; i++){
		if (i<AGENTS/2){
			agents[i].position = 1;
		} else {
			agents[i].position = 3;
		}
	}

	for (int i=0; i<NIGHTS; i++){
		//if(i==1) nights[i]=AGENTS;
		if (i==1 || i==3) nights[i] = AGENTS/2;
		else nights[i]=0;
	}
}


void initialize(){
	agents = vector<Agent>(AGENTS);
	nights = vector<int>(NIGHTS);


	// set agents on nights
	alpha=ALPHA;
	epsilon=EPSILON;
	setOnBeaches();

	// initialize Q
	for (int i=0; i<AGENTS; i++){
		for (int s=0; s<NSTATES; s++){
			for (int a=0; a<NACTIONS; a++){
				agents[i].qtable[s][a] = 0.0;
			}
		}
	}
	G_log.clear();
}

double maxQ(int a, int s, int &ind){
	double best_val = -double(INT_MAX);
	vector<int> best_inds;
	for (int i=0; i<NACTIONS; i++){
		if (agents[a].type==LEFTONLY && i==RIGHT) continue;
		if (agents[a].type==RIGHTONLY && i==LEFT) continue;


		if (agents[a].qtable[s][i]>best_val){
			best_val = agents[a].qtable[s][i];
			best_inds.clear();
			best_inds.push_back(i);
		} else if (agents[a].qtable[s][i]==best_val){
			best_inds.push_back(i);
		}
		agents[a].qtable[s][i];
	}
	ind = best_inds[rand()%best_inds.size()];
	return best_val;
}

void getActions(int t){
	// find best action
	vector<double> percentages(NACTIONS,0.0);
	for (int i=0; i<AGENTS; i++){
		
		int k;
		double mycoin = COIN_FLOOR0;
		if (epsilon<mycoin && t>=100){
			double best_val = -double(INT_MAX);
			int j = agents[i].state;
			//int j=0;
			maxQ(i,j,k); // gets the index for the best action k, given i and j
		} else{
			if (agents[i].type==LEFTONLY){
				k = rand()%2;
			} else if (agents[i].type==RIGHTONLY){
				k=rand()%2+1;
			} else {
				k = rand()%NACTIONS;
			}
		}
		agents[i].action = k;
		agents[i].count[k]++;
		percentages[k]+=1.0/double(AGENTS);
		if (i==0){
			//printf("here");
		}
	}

		//printf("(%f | %f | %f), ",percentages[0],percentages[1],percentages[2]);


}

void move(int a, int dir){
	// remove from current position

	//nights[agents[a].position]--;
	//nights[dir]++;
	//agents[a].position = dir;

	
	nights[agents[a].position]--;

	int adj = (dir - 1); // turns it into a motion
	int newpos = agents[a].position+adj;

	if (newpos>=NIGHTS) newpos-=NIGHTS;
	else if (newpos<0) newpos+=NIGHTS;
		
	nights[newpos]++;
	agents[a].position = newpos;
	//*/
}

void enactActions(){
	for (int i=0; i<AGENTS; i++){
		move(i,agents[i].action);
	}
}

double getGlobalReward(vector<double> &L, vector<double> &D){
	double G = 0;
	for (int i=0; i<AGENTS; i++){
		double x = double(nights[agents[i].position]);
		// get the reward for the night
		L[i] = x*exp(-x/C);
		G+=exp(-x/C);//L[i];

		double xc = x-1.0;
		D[i] = x*exp(-x/C)-xc*exp(-xc/C);
	}
	return G;
}

void getRewards(){
	// LOCAL REWARD

	//e^-x/c;
	vector<double> L(AGENTS,0.0);
	vector<double> D(AGENTS,0.0);
	double G = getGlobalReward(L,D);

	for (int i=0; i<AGENTS; i++){
		if (reward_mode==GLOBAL){
			agents[i].reward = G;
		} else if (reward_mode==LOCAL){
			agents[i].reward = L[i];
		} else if (reward_mode==DIFFERENCE){
			agents[i].reward = D[i];
		} else {
			printf("unrecognized reward type");
			system("pause");
		}
	}

	G_log.push_back(G);
}

void updateQ(){
	
	for (int i=0; i<AGENTS; i++){
		double* Qold = agents[i].qold();
		double R = agents[i].reward;
		int max_index; // dummy
		//double Qnew = Qold + alpha*(R+GAMMA*maxQ(i,agents[i].state,max_index)-Qold);
		double discQ = (*Qold)*(1-alpha); // discounted old value
		//double futQ = alpha*(R+GAMMA*maxQ(i,agents[i].state,max_index));
		double futQ = alpha*(R+GAMMA*maxQ(i,0,max_index));
		double Qnew = discQ+futQ;

		(*Qold)=Qnew;
	}


}

int _tmain(int argc, _TCHAR* argv[])
{

	// gather rewards
	// print out rewards
	// plot rewards
	// blind + not blind
//	double alpha_decay  = 0.9999;
//	double eps_decay = 0.9999;

	double alpha_decay  = 1.0;
	double eps_decay = 1.0;
	vector<vector<double> > allGs;
	for (int r=0; r<NREWARDS; r++){
		reward_mode = r;
		initialize();
		for (int i=0; i<STEPS; i++){
			int end_timestep = 1;
			setOnBeaches();
			getStates();
			for (int t=0; t<end_timestep; t++){
				getActions(i);
				enactActions();
				getStates();
				getRewards();
				updateQ();
				for (int n=0; n<NIGHTS; n++){
					printf("%i\t",nights[n]);
				}
				printf ("%\t %f",G_log.back());
				printf("\n");
			}
			alpha*=alpha_decay;
			epsilon*=eps_decay;
		}
		allGs.push_back(G_log);
		//PrintOut::toFile1D(G_log,"G_log.txt");

		/*
		if (r==0) PrintOut::toFile2D(allGs,"Local_agent.csv");
		if (r==1) PrintOut::toFile2D(allGs,"Global_agent.csv");
		if (r==2) PrintOut::toFile2D(allGs,"Difference_agent.csv");
		*/
	}


	PrintOut::toFile2D(allGs,"G_log.txt");

	return 0;
}

