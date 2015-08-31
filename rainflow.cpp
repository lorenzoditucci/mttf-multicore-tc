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

list<Cycles> rainflow_algorithm(vector<float> temperatures, vector<float> times,  int N){
	int i = 0; //index for e
    int i_6 = 0; //index for e when read from the beginning
	int *j = (int *)malloc(sizeof(int)); //index for temperatures
    j[0] = 0;
	int s_i = 0; //index for the head (S)

	bool end = false;

	Cycles cycle;
	
	//list<int> e;	
	list<int> X;
	list<int> Y;
	float *e = (float *)malloc(sizeof(float) * 2*N); //vector of peak/valleys
	float *t = (float *)malloc(sizeof(float) * 2*N); //vector for the times considered
	//int *X = (int *)malloc(sizeof(int) * N[0]); //range under consideration
	//int *Y = (int *)malloc(sizeof(int) * N[0]); //previous range adjacent to X
	int tempValX = 0;
	int tempValY = 0;

	int count = 0; //number of cycles counted

	int curr_state = 1;

	list<Cycles> cycles;

	while(true){
			//print_vector(e, i, s_i);
		switch(curr_state){
			case READ1 : //Read the next peak or valley (if out of data, go to Step 6)
						e[i] = read_next_peak_valley(temperatures, j, N);
						t[i] = times.at((*j) - 1);
						if(e[i] == -1){
							curr_state = READ6;
							e[i] = 0;
							t[i] = 0;
							//i--;
							break;
						}
						if(i == 0){
							//s = e[i];
							s_i = i;
						}
						i++;
						curr_state = FORM_RANGED_X_Y2;
						break;

			case FORM_RANGED_X_Y2: //Form ranges X and Y(if the vector contains less than 2 points past the starting point, go to Step I)
						X.clear();
						Y.clear();

						if(i - (s_i + 1) < 2){
							curr_state = READ1;
							//inutile assegnamento
							/*
							if(i == 2){
								X.push_back(i-2);
								X.push_back(i-1);
							}else if(i ==1){
								X.push_back(i-1);
							}
							*/
							break;
						}
						
						X.push_back(i-2);
						X.push_back(i-1);

						Y.push_back(i-3);
						Y.push_back(i-2);

						//otherwise continue
						curr_state = COMPARE3;
						break;
			case COMPARE3: //Compare ranges X and Y a. If X<Y, go to Step I 
												//	b. IfX=Yand YcontainsS,gotoStep 1
												//	c. If X > Y and Y containsS, go to Step 4
												//	d. If X >= Y and Y does not contain S, go to Step 5
						tempValX = abs(e[X.front()] - e[X.back()]);
						tempValY = abs(e[Y.front()] - e[Y.back()]);

						if(tempValX < tempValY){
							curr_state = READ1;
						}else if(tempValX == tempValY && (s_i == Y.front() || s_i == Y.back())){
							curr_state = READ1;
						}else if(tempValX > tempValY && (s_i == Y.front() || s_i == Y.back())){
							curr_state = MOVE_S4;
						}else if(tempValX >= tempValY && !(s_i == Y.front() || s_i == Y.back())){
							curr_state = COUNT_DISCARD5;
						}
						break;
			case MOVE_S4: //Move S to the next point in the vector Go to Step 1
						s_i++;
						//s = e[s_i];
                        //printf("STEP 4 - moving S, now is %d\n", s);
                        curr_state = READ1;
						break;
			case COUNT_DISCARD5: //Count range Y - Discard the peak and valley of Y Go to Step 2
						count++;
						//printf("\n5range %f", abs(e[Y.back()] - e[Y.front()]));

						cycle.setTemp1(e[Y.back()]);
						cycle.setTemp2(e[Y.front()]);
						cycle.setRange(abs(e[Y.back()] - e[Y.front()]));
						cycle.setTime(t[i-3]);//storing start time of the cycle

						cycles.push_back(cycle);

						//cout << " Data is :" << cycle.temp1;
						//cout << " and " << cycle.temp2 << endl;

						//discard mean remove from vector
						i = clean_reorganize(e, i);

						curr_state = FORM_RANGED_X_Y2;
						break;
			case READ6: //Read the next peak or valley from the beginning of the vector E(n)(if the starting point, S, has already been reread, STOP)
                        //ALWAYS FROM THE BEGINNING????????
                        e[i] = e[i_6];
			t[i] = t[i_6];
						if(i_6 == s_i){
							//STOP, end of program, return count!
							end = true;
							//return count;
						}
                        i++;
                        i_6++;

						curr_state = FORM_RANGED_X_Y7;
						break;
			case FORM_RANGED_X_Y7: //Form ranges X and Y(if the vector contains less than 2 points past the starting point, go to Step 6)
						
						X.clear();
						Y.clear();
						
						if(i - (s_i + 1) < 2){
							if(end == true){
								return cycles;
							}
							curr_state = READ6;
							if(i == 2){
								X.push_back(i-2);
								X.push_back(i-1);
							}else if(i ==1){
								X.push_back(i-1);
							}
							break;
						}
						
						X.push_back(i-2);
						X.push_back(i-1);

						Y.push_back(i-3);
						Y.push_back(i-2);

						curr_state = COMPARE8;
			case COMPARE8: //Compare ranges X and Y 
												//	a. If X < Y, gotoStep6
												//	b. If X >= Y, gotoStep9
						tempValX = abs(e[X.front()] - e[X.back()]);
						tempValY = abs(e[Y.front()] - e[Y.back()]);

						if(tempValX < tempValY){
							if(end == true){
								return cycles;
							}
							curr_state = READ6;
						}else{
							if(Y.front() == s_i || Y.back() == s_i){
								s_i--;
							}
							curr_state = COUNT_DISCARD9;
						}
						break;
			case COUNT_DISCARD9: //Count range Y Discard the peak and valley of Y Go to Step 7

						count++;
						//printf("\n9range %f", abs(e[Y.back()] - e[Y.front()]));

						cycle.setTemp1(e[Y.back()]);
						cycle.setTemp2(e[Y.front()]);
						cycle.setRange(abs(e[Y.back()] - e[Y.front()]));
						cycle.setTime(t[i-3]);
						
						cycles.push_back(cycle);

						//cout << " Data is :" << cycle.temp1;
						//cout << " and " << cycle.temp2 << endl;

						i = clean_reorganize(e, i);
						curr_state = FORM_RANGED_X_Y7;
						break;

		}
	}

	cycle = create_error_cycle(cycle);
	cycles.push_back(cycle);
	return cycles;
	

}

/*
Algorithm for the dynamic version. 

The read of peak and valleys is done in another module, here I just keep waiting for a new value and calculate everytime a cycle
*/
list<Cycles>* rainflow_algorithm_dynamic(vector<float> *e, vector<float> *t, list<Cycles> *cycles){
	int i = 0; //index for e
    int i_6 = 0; //index for e when read from the beginning
	int *j = (int *)malloc(sizeof(int)); //index for temperatures
    j[0] = 0;
	int s_i = 0; //index for the head (S)

	bool end = false;

	Cycles cycle;
	
	//list<int> e;	
	list<int> X;
	list<int> Y;
	//float *e = (float *)malloc(sizeof(float) * 2*N); //vector of peak/valleys
	//float *t = (float *)malloc(sizeof(float) * 2*N); //vector for the times considered
	//int *X = (int *)malloc(sizeof(int) * N[0]); //range under consideration
	//int *Y = (int *)malloc(sizeof(int) * N[0]); //previous range adjacent to X
	int tempValX = 0;
	int tempValY = 0;

	int count = 0; //number of cycles counted
	int check = 0; //used to check the stop value..

	int curr_state = 1;

	//list<Cycles> cycles;
	//cout << "RAINFLOW: start " << endl;
	while(true){
			//print_vector(e, i, s_i);
		switch(curr_state){
			case READ1 : //Read the next peak or valley (if out of data, go to Step 6)
						//e[i] = read_next_peak_valley(temperatures, j, N);
						//t[i] = times.at((*j) - 1);
						//cout << "RAINFLOW: READ1 - e size " << (*e).size() <<endl;
						check = i < (*e).size() ? i : (*e).size()-1;
						if( i != 0 && (*e).at(check) == -1){ 
							//cout << "RAINFLOW: termination simbol found, terminating...."<< endl;
							//curr_state = READ6;
							//e[i] = 0;
							//t[i] = 0;
							//i--;
							return cycles;
						}else if((*e).size() <= i){
							//need to wait for a new value...
							curr_state = READ1;
							//cout << "RAINFLOW: nothing new here, sleeping zzz size is " << (*e).size() << " i " << i << endl;
							this_thread::sleep_for (std::chrono::seconds(5));
						}else{
							//cout << "considering " << (*e).at(i) << endl;
							//cout << "RAINFLOW: increment i and go to 2... i " << i << endl;
							i++;
							curr_state = FORM_RANGED_X_Y2;
						}
						if(i == 0){
							//s = e[i];
							s_i = i;
						}
						break;

			case FORM_RANGED_X_Y2: //Form ranges X and Y(if the vector contains less than 2 points past the starting point, go to Step I)
						//cout << "RAINFLOW: FORM_RANGED_X_Y2" <<endl;
						X.clear();
						Y.clear();

						if(i - (s_i + 1) < 2){
							//cout << "not enough after the head..." << endl;
							curr_state = READ1;
							//inutile assegnamento
							/*
							if(i == 2){
								X.push_back(i-2);
								X.push_back(i-1);
							}else if(i ==1){
								X.push_back(i-1);
							}
							*/
							break;
						}
						
						X.push_back(i-2);
						X.push_back(i-1);

						Y.push_back(i-3);
						Y.push_back(i-2);
						
						//cout << " X1 : " << (*e).at(i-2) << endl;
						//cout << " X2 : " << (*e).at(i-1) << endl;

						//cout << " Y1 : " << (*e).at(i-3) << endl;
						//cout << " Y1 : " << (*e).at(i-2) << endl;
						
						//otherwise continue
						curr_state = COMPARE3;
						break;
			case COMPARE3: //Compare ranges X and Y a. If X<Y, go to Step I 
												//	b. IfX=Yand YcontainsS,gotoStep 1
												//	c. If X > Y and Y containsS, go to Step 4
												//	d. If X >= Y and Y does not contain S, go to Step 5
						//cout << "RAINFLOW: COMPARE3" <<endl;
						tempValX = abs((*e).at(X.front()) - (*e).at(X.back()));
						tempValY = abs((*e).at(Y.front()) - (*e).at(Y.back()));
						
						//cout << " tempValX : " << tempValX << " tempValY : " << tempValY << endl;
						
						if(tempValX < tempValY){
							curr_state = READ1;
						}else if(tempValX == tempValY && (s_i == Y.front() || s_i == Y.back())){
							curr_state = READ1;
						}else if(tempValX > tempValY && (s_i == Y.front() || s_i == Y.back())){
							curr_state = MOVE_S4;
						}else if(tempValX >= tempValY && !(s_i == Y.front() || s_i == Y.back())){
							curr_state = COUNT_DISCARD5;
						}
						break;
			case MOVE_S4: //Move S to the next point in the vector Go to Step 1
						//cout << "RAINFLOW: MOVE_S4" <<endl;
						s_i++;
						//s = e[s_i];
                       				//printf("STEP 4 - moving Si, now is %d\n", s_i);
                        			curr_state = READ1;
						break;
			case COUNT_DISCARD5: //Count range Y - Discard the peak and valley of Y Go to Step 2
						//cout << "RAINFLOW: COUNT_DISCARD5" <<endl;
						count++;
						//printf("\n 5range %f", abs((*e).at(Y.back()) - (*e).at(Y.front())));

						cycle.setTemp1((*e).at(Y.back()));
						cycle.setTemp2((*e).at(Y.front()));
						cycle.setRange(abs((*e).at(Y.back()) - (*e).at(Y.front())));
						cycle.setTime((*t).at(i-3));//storing start time of the cycle
						//cout << "RAINFLOW: ALL SET, NOW STORE - range " << cycle.getRange() << endl;
						(*cycles).push_back(cycle);

						//cout << " Data is :" << cycle.temp1;
						//cout << " and " << cycle.temp2 << endl;

						//discard mean remove from vector
						i = clean_reorganize(e,t,i);
						//cout << "i now is " << i << endl;
						curr_state = FORM_RANGED_X_Y2;
						break;
			/*case READ6: //Read the next peak or valley from the beginning of the vector E(n)(if the starting point, S, has already been reread, STOP)
                        //ALWAYS FROM THE BEGINNING????????
                        e[i] = e[i_6];
			t[i] = t[i_6];
						if(i_6 == s_i){
							//STOP, end of program, return count!
							end = true;
							//return count;
						}
                        i++;
                        i_6++;

						curr_state = FORM_RANGED_X_Y7;
						break;
			case FORM_RANGED_X_Y7: //Form ranges X and Y(if the vector contains less than 2 points past the starting point, go to Step 6)
						
						X.clear();
						Y.clear();
						
						if(i - (s_i + 1) < 2){
							if(end == true){
								return cycles;
							}
							curr_state = READ6;
							if(i == 2){
								X.push_back(i-2);
								X.push_back(i-1);
							}else if(i ==1){
								X.push_back(i-1);
							}
							break;
						}
						
						X.push_back(i-2);
						X.push_back(i-1);

						Y.push_back(i-3);
						Y.push_back(i-2);

						curr_state = COMPARE8;
			case COMPARE8: //Compare ranges X and Y 
												//	a. If X < Y, gotoStep6
												//	b. If X >= Y, gotoStep9
						tempValX = abs(e[X.front()] - e[X.back()]);
						tempValY = abs(e[Y.front()] - e[Y.back()]);

						if(tempValX < tempValY){
							if(end == true){
								return cycles;
							}
							curr_state = READ6;
						}else{
							if(Y.front() == s_i || Y.back() == s_i){
								s_i--;
							}
							curr_state = COUNT_DISCARD9;
						}
						break;
			case COUNT_DISCARD9: //Count range Y Discard the peak and valley of Y Go to Step 7

						count++;
						//printf("\n9range %d", abs(e[Y.back()] - e[Y.front()]));

						cycle.setTemp1(e[Y.back()]);
						cycle.setTemp2(e[Y.front()]);
						cycle.setRange(abs(e[Y.back()] - e[Y.front()]));
						cycle.setTime(t[i-3]);
						
						cycles.push_back(cycle);

						//cout << " Data is :" << cycle.temp1;
						//cout << " and " << cycle.temp2 << endl;

						i = clean_reorganize(e, i);
						curr_state = FORM_RANGED_X_Y7;
						break;
			*/

		}
	}

	cycle = create_error_cycle(cycle);
	(*cycles).push_back(cycle);
	return cycles;
	

}


int clean_reorganize(vector<float> *e, vector<float> *t, int i){
	//cout << "RAINBOW:CLEAN_REORGANIZE: size before " << (*e).size() << endl;
	float tempVal = (*e).at(i-1);
	(*e).erase(((*e).begin() + (i-1)));	
	(*e).erase(((*e).begin() + (i-2)));
	(*e).erase(((*e).begin() + (i-3)));
	(*e).insert((*e).begin() + (i-3), tempVal);

	tempVal = (*t).at(i-1);
	//(*t).erase(((*t).begin() + (i-1)));
	//(*t).erase(((*e).begin() + (i-2)));
	//(*t).erase(((*e).begin() + (i-3)));
	//(*t).insert((*e).begin() + (i-3), tempVal);

	//cout << "RAINBOW:CLEAN_REORGANIZE: size after " << (*e).size() << endl;
	i = i - 2;

	return i;
}

/*
remove the values from e and return the new index
*/
int clean_reorganize(float *e, int i){
    // removing the second to the least and the one before as are Y values
	
    e[i-3] = e[i-1];
    i = i-2;
    /*
    for(int k = 0; k<i; k++){
        printf("\n e : %d \n", e[k]);
    }
    */
	return i;
}

/*
Dynamic algorithm.

takes the new value and the trend, the trend indicate how was the previous value
0 -> equal //not used
1 -> greater (searching for a valley)
2 -> smaller (searching for a peak)
*/

float read_next_peak_valley_bw(float prevValue, float value, short *trend){
	if(value > prevValue && (trend[0] == 1 || trend[0] == 0)){
		//prevValue is a valley, now we search a peak
		trend[0] = 2;
		return prevValue;
	}

	if(value < prevValue && (trend[0] == 2 || trend[0] == 0)){
		//prevValue is a peak, now we search a valley
		trend[0] = 1;
		return prevValue;
	}

	if(value == prevValue){
		trend[0] = 0;
	}
	
	if(value == 999 || prevValue == 999){
		return 999;
	}

	cout << "no peak/valley found... " << endl;
	return -2; //no peak or valley found!
}

//int read_next_peak_valley(int *temperatures, int *startingIndex, int N){
float read_next_peak_valley(vector<float> temperatures, int *startingIndex, int N){
	if(startingIndex[0] == 0){
		return temperatures.at(startingIndex[0]++);
	}

	if(startingIndex[0] == N){
		return -1; //out of data!
	}


	//first
	if(temperatures.at(startingIndex[0]-1) > temperatures.at(startingIndex[0])){	
		//previously I found a peak, now I search a valley
		while(startingIndex[0] < N-1){
			if(temperatures.at(startingIndex[0]) < temperatures.at(startingIndex[0] +1 )){
				return temperatures.at(startingIndex[0]++);
			}else{
				startingIndex[0]++;
			}
		}
		return temperatures.at(startingIndex[0]++);

	}

	if(temperatures.at(startingIndex[0] -1) < temperatures.at(startingIndex[0])){
		while(startingIndex[0] < N - 1){
			if(temperatures.at(startingIndex[0]) > temperatures.at(startingIndex[0] +1 )){
				return temperatures.at(startingIndex[0]++);
			}else{
				startingIndex[0]++;
			}
		}

		return temperatures.at(startingIndex[0]++);
	}else{
		return read_next_peak_valley(temperatures, startingIndex, N);
	}

	return -1;
}


void print_vector(int *e, int N, int s){
    printf("the head is %d \n", e[s]);
    printf("\n");
    for (int i = 0; i < N; i++) {
        printf(" %d ", e[i]);
    }
    printf("\n");
}

Cycles create_error_cycle(Cycles cycle){
	cycle.setTemp1(-1);
	cycle.setTemp2(-1);
	cycle.setRange(-1);
	return cycle;
}
