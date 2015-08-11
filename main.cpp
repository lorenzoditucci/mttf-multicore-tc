/*
*	Given a Temperature Trend, outputs the MTTF due to Thermal Cycle
*	
*	The MTTF due to thermal Cycle is caclulated in Three steps [1]:
*	
*	1) Calculation of the thermal Cycles from a thermal profile using Downing simple rainbow counting algorithm[2]
*	2) Calculation, for each thermal cycle, the number of cycles to failure using Coffin-Mansion's rule:
*			Ntc[i] = Atc(dT[i] - Tth)^-b * e^(Eatc/(KTmax[i]))
*	3) Calculate MTTF using Miner's rule:
*			MTTF = (1/m)Ntc*sum,i=1 to m (ti)
*
*
*
*
*
*
*	[1] Reinforcement Learning-Based Inter- and Intra-Application Thermal Optimization for Lifetime Improvement of Multicore Systems
*	[2] S. Downing and D. Socie. Simple Rainflow Counting Algorithms. International Journal of Fatigue, 1982.
*
* 	Lorenzo Di Tucci <personal AT lorenzoditucci.com>
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "rainflow.h"

using namespace std;

int main(int argc, char *argv[]){
	printf("start");
	int N = 8;
	//int temperatures[10] = {30,25,8,50,3,5,28,45,1,12};
	//int temperatures[19] = {8,6,3,2,5,10,7,5,6,7,8,10,8,4,2,3,6,0,-10};
    int temperatures[8] = {10,2,12,4,12,2,4,-10};
    //int temperatures[29] = { 0, 5, 6, 5, 3, 2, 6, 8, 7, 6, 8, 9, 7, 5, 8, 9,1, 8, 4, 3, 4, 5, 6, 7, 5, 4, 6, 2, 0};
    //int temperatures[30] = {10,8,24,54,76,99,76,44,52,63,23,56,21,78,43,21,4,5,6,2,1,8,6,5,4,23,72,54,76,100};
	int i[1];
	i[0] = 0;

	list<Cycles> cycles = rainflow_algorithm(temperatures, N);

	if(cycles.back().temp1 == -1 && cycles.back().temp2 == -1 && cycles.back().range == -1){
		printf("error in cycle calculation!");
		return -1;
	}
	//printf("Number of Cycles : %d \n", Ntc);

	/*
	used for debugging
	printf("%d\n", i[0]);
	while(true){
		int res = read_next_peak_valley(temperatures, i, 10);

		if(res != -1){
			printf("%d \n", res);
		}else{
			printf("END\n");
			return 0;
		}
	}
	*/
	
	
	return 0;
}