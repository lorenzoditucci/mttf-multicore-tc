#include "rainflow.h"

using namespace std;


static const int READ1 = 1;
static const int FORM_RANGED_X_Y2 = 2;
static const int READ6 = 6;
/*
Rainflow algorithm implementation, given an array of temperatures, returns the number of cycles. (implementation II of the paper [2])
*/
int rainflow_algorithm(int *temperatures, int *N){
	int i = 0;
	int *j = 0;
	int s = 0; //starting peak/valley
	return 0;
	
	list<int> e;
	list<int> X;
	list<int> Y;
	//int *e = (int *)malloc(sizeof(int) * N[0]); //vector of peak/valleys
	//int *X = (int *)malloc(sizeof(int) * N[0]); //range under consideration
	//int *Y = (int *)malloc(sizeof(int) * N[0]); //previous range adjacent to X

	int count = 0; //number of cycles counted

	int curr_state = 1; 

	while(true){
		switch(curr_state){
			case READ1 : //Read the next peak or valley (if out of data, go to Step 6)
						e.push_back(read_next_peak_valley(temperatures, j, N[0])); 
						if(e.front() == -1){
							curr_state = READ6;
							break;
						}
						if(i == 0){
							s = e.front();
						}
						curr_state = FORM_RANGED_X_Y2;
						break;

			case FORM_RANGED_X_Y2: //Form ranges X and Y(if the vector contains less than 2 points past the starting point, go to Step I)
						if(X.size() == 2){

						}else{
							X.size();
						}
						if(e.size() < 3){
							curr_state = READ1;
							break;
						}




		}
	}
	

}


int read_next_peak_valley(int *temperatures, int *startingIndex, int N){
	if(startingIndex[0] == 0){
		return temperatures[startingIndex[0]++];
	}

	if(startingIndex[0] == N){
		return -1; //out of data!
	}


	//first
	if(temperatures[startingIndex[0]-1] > temperatures[startingIndex[0]]){
		//previously I found a peak, now I search a valley
		while(startingIndex[0] < N-1){
			if(temperatures[startingIndex[0]] < temperatures[startingIndex[0] +1 ]){
				return temperatures[startingIndex[0]++];
			}else{
				startingIndex[0]++;
			}
		}
		return temperatures[startingIndex[0]++];

	}

	if(temperatures[startingIndex[0] -1 ] < temperatures[startingIndex[0]]){
		while(startingIndex[0] < N - 1){
			if(temperatures[startingIndex[0]] > temperatures[startingIndex[0] +1 ]){
				return temperatures[startingIndex[0]++];
			}else{
				startingIndex[0]++;
			}
		}

		return temperatures[startingIndex[0]++];
	}else{
		return read_next_peak_valley(temperatures, startingIndex, N);
	}

	return -1;
}