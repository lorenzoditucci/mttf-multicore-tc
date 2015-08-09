#include "rainflow.h"


static const int READ1 = 1;
static const int FORM_RANGED_X_Y2 = 2;
static const int COMPARE3 = 3;
static const int MOVE_S4 = 4;
static const int COUNT_DISCARD5 = 5;
static const int READ6 = 6;
static const int FORM_RANGED_X_Y7 = 7;
static const int COMPARE8 = 8;
static const int COUNT_DISCARD9 = 9;
/*
Rainflow algorithm implementation, given an array of temperatures, returns the number of cycles. (implementation II of the paper [2])
*/

using namespace std;

int rainflow_algorithm(int *temperatures, int N){
	int i = 0; //index for e
    int i_6 = 0; //index for e when read from the beginning
	int *j = (int *)malloc(sizeof(int)); //index for temperatures
    j[0] = 0;
	int s = 0; //starting peak/valley
	int s_i = 0; //index for S
	
	//list<int> e;
	list<int> X;
	list<int> Y;
	int *e = (int *)malloc(sizeof(int) * 2*N); //vector of peak/valleys
	//int *X = (int *)malloc(sizeof(int) * N[0]); //range under consideration
	//int *Y = (int *)malloc(sizeof(int) * N[0]); //previous range adjacent to X
	int tempValX = 0;
	int tempValY = 0;

	int count = 0; //number of cycles counted

	int curr_state = 1; 

	while(true){
		switch(curr_state){
			case READ1 : //Read the next peak or valley (if out of data, go to Step 6)
						e[i] = read_next_peak_valley(temperatures, j, N);
                        printf("read %d, from e \n", e[i]);
						if(e[i] == -1){
							curr_state = READ6;
							e[i] = 0;
							i--;
							break;
						}
						if(i == 0){
							s = e[i];
							s_i = i;
						}
						i++;
						curr_state = FORM_RANGED_X_Y2;
						break;

			case FORM_RANGED_X_Y2: //Form ranges X and Y(if the vector contains less than 2 points past the starting point, go to Step I)

						X.clear();
						Y.clear();

						if(i < 3){
							curr_state = READ1;
							if(i == 2){
								X.push_back(e[i-2]);
								X.push_back(e[i-1]);
							}else if(i ==1){
								X.push_back(e[i-1]);
							}
							break;
						}
						
						X.push_back(e[i-2]);
						X.push_back(e[i-1]);

						Y.push_back(e[i-3]);
						Y.push_back(e[i-2]);

						//otherwise continue
						curr_state = COMPARE3;
						break;
			case COMPARE3: //Compare ranges X and Y a. If X<Y, go to Step I 
												//	b. IfX=Yand YcontainsS,gotoStep 1
												//	c. If X > Y and Y containsS, go to Step 4
												//	d. If X >= Y and Y does not contain S, go to Step 5
						tempValX = abs(X.front() - X.back());
						tempValY = abs(Y.front() - Y.back());

						if(tempValX < tempValY){
							curr_state = READ1;
							break;
						}

						if(tempValX == tempValY && (s == Y.front() || s == Y.back())){
							curr_state = READ1;
							break;
						}

						if(tempValX > tempValY && (s == Y.front() || s == Y.back())){
							curr_state = MOVE_S4;
							break;
						}

						if(tempValX >= tempValY && !(s == Y.front() || s == Y.back())){
							curr_state = COUNT_DISCARD5;
							break;
						}

						printf("ERROR!");
						return -1;
			case MOVE_S4: //Move S to the next point in the vector Go to Step 1
						s_i++;
                        if(s_i > N){
                            printf("ERROR! INDEX OUT OF BOUND s_i is %d \n", s_i);
                            return -1;
                        }
						s = e[s_i];
                        curr_state = READ1;
						break;
			case COUNT_DISCARD5: //Count range Y - Discard the peak and valley of Y Go to Step 2
						count++;
						//discard mean remove from vector
						i = clean_reorganize(e, i, Y.front(), Y.back());
						if(i == -1){
							printf("error, debug != 2\n");
							return -1;
						}
						curr_state = FORM_RANGED_X_Y2;
						break;
			case READ6: //Read the next peak or valley from the beginning of the vector E(n)(if the starting point, S, has already been reread, STOP)
                        e[i] = e[s_i];
                        printf("read %d, from e \n", e[i]);
						if(e[i] == s){
							//STOP, end of program, return count!
							return count;
						}
                        i++;
                        i_6++;

						curr_state = FORM_RANGED_X_Y7;
						break;
			case FORM_RANGED_X_Y7: //Form ranges X and Y(if the vector contains less than 2 points past the starting point, go to Step 6)
						
						X.clear();
						Y.clear();
						
						if(i < 3){
							curr_state = READ6;
							if(i == 2){
								X.push_back(e[i-2]);
								X.push_back(e[i-1]);
							}else if(i ==1){
								X.push_back(e[i-1]);
							}
							break;
						}
						
						X.push_back(e[i-2]);
						X.push_back(e[i-1]);

						Y.push_back(e[i-3]);
						Y.push_back(e[i-2]);

						curr_state = COMPARE8;
			case COMPARE8: //Compare ranges X and Y 
												//	a. If X < Y, gotoStep6
												//	b. If X >= Y, gotoStep9
						tempValX = abs(X.front() - X.back());
						tempValY = abs(Y.front() - Y.back());

						if(tempValX < tempValY){
							curr_state = READ6;
						}else{
							curr_state = COUNT_DISCARD9;
						}
						break;
			case COUNT_DISCARD9: //Count range Y Discard the peak and valley of Y Go to Step 7

						count++;
						i = clean_reorganize(e, i, Y.front(), Y.back());
						if(i == -1){
							printf("error, debug != 2");
							return -1;
						}
						curr_state = FORM_RANGED_X_Y7;
						break;

		}
	}

	return -1;
	

}

/*
remove the values from e and return the new index
*/
int clean_reorganize(int *e, int i, int val1, int val2){
	//s stays at the same place
	int debug = 0;
	for(int j= i-1; j>=0 && debug < 2; j--){
		if(e[j] == val1 || e[j] == val2){
			e[j] = e[j+1];
			debug++;
		}
	}

	if(debug != 2){
		return -1;
	}

	i = i-2;

	return i;
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