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
*	[3] Steady-State Dynamic Temperature Analysis and Reliability Optimization for Embedded Multiprocessor Systems Ukhov I. et al.
* 	Lorenzo Di Tucci <personal AT lorenzoditucci.com>
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include "rainflow.h"

using namespace std;


float coffin_manson(Cycles cycle);
float miner_rule(list<float> Ntci, list<Cycles> cycles);
void show_usage(string exename);

int main(int argc, char *argv[]){
	cout << "start" << endl;
	string filename;
	//int N = 8;
	//int temperatures[10] = {30,25,8,50,3,5,28,45,1,12};
	//int temperatures[19] = {8,6,3,2,5,10,7,5,6,7,8,10,8,4,2,3,6,0,-10};
   // int temperatures[8] = {10,2,12,4,12,2,4,-10};
    //int temperatures[29] = { 0, 5, 6, 5, 3, 2, 6, 8, 7, 6, 8, 9, 7, 5, 8, 9,1, 8, 4, 3, 4, 5, 6, 7, 5, 4, 6, 2, 0};
    //int temperatures[30] = {10,8,24,54,76,99,76,44,52,63,23,56,21,78,43,21,4,5,6,2,1,8,6,5,4,23,72,54,76,100};
		
	if (argc < 3) {
        show_usage(argv[0]);
        return 1;
    	}
	std::string arg;
	for(int i = 1; i < argc; i = i + 2){
		arg = argv[i];
		//cout << "arg " << arg << endl;
		if(arg == "-h" || arg == "--help"){
			show_usage(argv[0]);
			return 1;
		}
		else if(arg == "-f" || arg == "--file"){
			filename = argv[i+1];
		}else{
			show_usage(argv[0]);
			return 1;
		}
	}

	cout << "filename " << filename << endl;
	
	vector<vector<float>> temperature;
	int counter = 1; //1st temp, 2nd time
	vector<float> temp;
	ifstream inputFile (filename, ios::in);
	if(inputFile.is_open()){
		string tempString;
		while(getline(inputFile, tempString)){
			cout << "string  " << tempString << endl;
			istringstream ss(tempString );
			while (ss)
    			{
    				 string s;
      				 if (!getline( ss, s, ',' )) break;
					
				if(counter % 2 ==0){
					temp.push_back(atof(s.c_str()));
					temperature.push_back(temp);
				}else{
					temp.clear();
					temp.push_back(atof(s.c_str()));
				}
				counter++;
    			}
		}
		inputFile.close();
	}else{
		cerr << "Error while Opening " << filename << endl;
		return -1;
	}	

	for(int i=0; i<temperature.size(); i++){
		cout << " temp && time " << endl;
		for(int j = 0; j < temperature.at(i).size(); j++)
			cout << " " << temperature.at(i).at(j);
		cout << endl;
	}
	counter --;
	if(counter % 2 != 0){
		cerr << "mismatch in temperature/time, the number is not even " << endl;
		return 1;
	}
	/*
	cout << endl;
	
	list<Cycles> cycles = rainflow_algorithm(temperature, temperature.size());
	list<float> Ntci;

	if(cycles.back().temp1 == -1 && cycles.back().temp2 == -1 && cycles.back().range == -1){
		printf("error in cycle calculation!");
		return -1;
	}
	
	cout << cycles.size() << "cycles" << endl;

	for (std::list<Cycles>::iterator it=cycles.begin(); it != cycles.end(); it++){
		cout << "range " << (*it).range << endl;

		//fake time
		(*it).setTime(3.57);
		
		Ntci.push_back(coffin_manson(*it));
		//cout.precision(20);
		cout << "Ntci "<<Ntci.back()<<endl;
	}

	float MTTF = miner_rule(Ntci, cycles);

	cout << "MTTF " << MTTF << endl;	
	*/
	return 0;
}

float miner_rule(list<float> Ntci, list<Cycles> cycles){
	int m = cycles.size();

	float Ntc = 0.0f;
	for (std::list<float>::iterator it=Ntci.begin(); it != Ntci.end(); it++){
		Ntc += (1.0/(*it));
	}
	//cout << " Ntc " << Ntc << endl;
	Ntc = m/Ntc;

	//cout << "Ntc is "<<Ntc <<endl;

	float MTTF = 0.0;
	for (std::list<Cycles>::iterator it=cycles.begin(); it != cycles.end(); ++it){
		MTTF += (*it).time;
	}

	MTTF = Ntc * MTTF / m;

	return MTTF;

}
float coffin_manson(Cycles cycle){
	//Ntc[i] = Atc(dT[i] - Tth)^-b * e^(Eatc/(KTmax[i]))

	//the variable have been defined following section 9.2 of the paper [3]
	//variable definition
	int Atc = 1; //***empirically determinedconstant
	float dTi = abs(cycle.temp1 - cycle.temp2); //amplitude of the ith thermal cycle
	int Tth = 0; //**temperature at which elastic deformation begins
	int b = 6; //**Coffin-Manson exponent constant
	float Eatc = 0.5; //**activation energy
	float Tmaxi = max(cycle.temp1, cycle.temp2); //maximum temperature in the ith thermal cycle.
	int K = 1;

	float Ntci = 0.0;
	Ntci += Atc * (dTi - Tth);
	//cout << Ntci << " t maxi " << Tmaxi;
	Ntci = pow(Ntci, -b) * exp(Eatc/(K*Tmaxi));
	//cout << " " <<Ntci<<endl;

	return Ntci;

}

void show_usage(string exename){
	//show how to use the tool
	
	cout << "Model for Evaluating MTTF due to Temperature Cycle " << endl;
	cout << "Course Dependable Systems @ Politecnico di Milano, 2015 "<< endl;
	cout << "Lorenzo Di Tucci <personal AT lorenzoditucci.com >" << endl;
	cout << "Usage : " << endl;
	cout << exename << endl;
	cout << "-h | --help show this help" <<endl;
	cout << "-f | --file specify input file: <TYPE OF INPUT> " <<endl; //to-do specify type of input!

	return;
}
