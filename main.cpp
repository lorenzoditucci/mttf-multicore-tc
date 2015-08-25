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
*	The Algorithm Works also in a Dynamic Case, calculating the R as stated in paper [4], formula (5)
*		R = e ^ -(sum j=1 to i (Tj/ aj(T)))
*
*
*	[1] Reinforcement Learning-Based Inter- and Intra-Application Thermal Optimization for Lifetime Improvement of Multicore Systems
*	[2] S. Downing and D. Socie. Simple Rainflow Counting Algorithms. International Journal of Fatigue, 1982.
*	[3] Steady-State Dynamic Temperature Analysis and Reliability Optimization for Embedded Multiprocessor Systems Ukhov I. et al.
*	[4] A Lightweight and Open-source Framework for the Lifetime Estimation of Multicore Systems, Bolchini C., et Al.
* 	Lorenzo Di Tucci <personal AT lorenzoditucci.com>
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
#include <thread>
#include "rainflow.h"

using namespace std;

typedef struct TempTimes{
	vector<float> temperatures;
	vector<float> times;
}TempTimes;

streampos check_data_from_file(vector<float> *temperatures, vector<float> *times, streampos  position, string filename, bool *firstRead);	
void check_input_routine(vector<float> *e,vector<float> *t, string *filename);
void printNumbers(TempTimes *tempTimesp);
int run_dynamic(string filename, vector<float> temperature, vector<float> times);
int run_static(string filename, vector<float> temperature, vector<float> times);
int load_data_from_file(string filename, vector<float> *temperature, vector<float> *times);
float coffin_manson(Cycles cycle);
float miner_rule(list<float> Ntci, list<Cycles> cycles);
void show_usage(string exename);





int main(int argc, char *argv[]){
	cout << "start" << endl;
	string filename;
	char version = 'N';
	//int N = 8;
	//int temperatures[10] = {30,25,8,50,3,5,28,45,1,12};
	//int temperatures[19] = {8,6,3,2,5,10,7,5,6,7,8,10,8,4,2,3,6,0,-10};
   // int temperatures[8] = {10,2,12,4,12,2,4,-10};
    //int temperatures[29] = { 0, 5, 6, 5, 3, 2, 6, 8, 7, 6, 8, 9, 7, 5, 8, 9,1, 8, 4, 3, 4, 5, 6, 7, 5, 4, 6, 2, 0};
    //int temperatures[30] = {10,8,24,54,76,99,76,44,52,63,23,56,21,78,43,21,4,5,6,2,1,8,6,5,4,23,72,54,76,100};
		
	if (argc < 4) {
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
		}else if(arg == "-d" || arg == "--dynamic" || arg == "-s" || arg == "--static"){
			if(arg[1] == '-'){
				version = arg[2];
			}else{
				version = arg[1];
			}
		}else{
			show_usage(argv[0]);
			return 1;
		}
	}

	cout << "filename " << filename << " version "<< version << endl;
	
	vector<float> temperature;
	vector<float> times;
	
	if(version == 's'){
		//static version
		if(run_static(filename, temperature, times) != 0)
		{
			cerr << "ERROR: execution failed for previous error!" << endl;
			return 1;
		}
		return 0;	
	}else if(version == 'd'){
		if(run_dynamic(filename, temperature, times) != 0){
			cerr << "ERROR: execution failed due to previous errors!" << endl;
			return 1;
		}
		return 0;
	}else{//not necessary
		cerr << "ERROR: no version specified" << endl;
		show_usage(argv[0]);
		return 1;
	}



	
}

int run_dynamic(string filename, vector<float> temperature, vector<float> times){
	
	TempTimes tempTimes =  TempTimes();
	
	vector<float> *e = new vector<float>;
	vector<float> *t = new vector<float>;
	thread thread_pv(check_input_routine, e, t, &filename);
	//cout << "first is executing..." << endl;
	thread_pv.join();
	
	//cout << "main function " << tempTimes.temperatures.at(1) << endl;
	

	return 0;
}

void check_input_routine(vector<float> *e,vector<float> *t, string *filename){
	//read value
	//check if new value, for each new value...
	
	vector<float> temperatures;
	vector<float> times;
	short trend = 0;
	bool first = true;
	bool firstRead = true;
	bool end = false;
	int index = 1;
	int indexE = 0; //index for e...
	
	float tempVal = 0.0;
	int oldSize = 0;
	streampos position;
	int temperatureSize = 0;
	bool check = false;
	while(end != true){
		
		position = check_data_from_file(&temperatures,&times,  position, filename[0], &firstRead);	
		cout << "temperatures size is " <<temperatures.size() << endl;
		
		if(temperatures.size() < 2){
			cout << "size less then 2 .." << endl;
			//continue;
		}else if(first) {
			oldSize = 2;
			check = true;
			cout << "first is true " << endl;
			first = false;
			if(temperatures.at(index-1) > temperatures.at(index)){
				trend = 2; //then we give this to the algorithm...
			}else if(temperatures.at(index-1) < temperatures.at(index)){
				trend = 1;
			}else{
				trend = 0;
				//continue;
			}
		}

		for(int i = oldSize -1 ; i < temperatures.size() && temperatures.size() >=2 && oldSize >=2 && check; i++){
			
			cout << "inside for, i is " << i << "passing these vlues to function " << temperatures.at(i-1) << " - " << temperatures.at(i) << " trend " << trend << endl;
			//e[indexE] = read_next_peak_valley_bw(temperatures.at(index-1), temperatures.at(index), &trend);
			tempVal = read_next_peak_valley_bw(temperatures.at(i-1), temperatures.at(i), &trend);
			cout << "tempval is " << tempVal;
			
			if(temperatures.at(i) == 999){ //fake stop signal
				if(temperatures.at(i-1) != (*e).at((*e).size()-1)){
					(*e).push_back(temperatures.at(i-1));	
					(*t).push_back(times.at(i-1));
				}
				cout << " STOP signal received... terminating..." <<endl;
				end = true;
				continue;
			}	
			//update time at index...
			if(tempVal != -2){
				(*e).push_back(tempVal);
				(*t).push_back(times.at(index));
				//t[indexE] = times.at(index);
				indexE++;
			}
			oldSize++;
		}
		
		cout << "e: ";
		for(int i = 0; i< (*e).size(); i++){
			cout <<" " << (*e).at(i);
		}
		cout << endl;
		
		//index++;
		temperatureSize = temperatures.empty() ? 0 : temperatures.size();
		if(temperatureSize == oldSize - 1){
			check = false;
		}else{
			check = true;
			//oldSize = temperatureSize;
		}
		cout << "old size now is " << oldSize << endl; 
		cout << "sleep for 5 seconds.... " << endl;
		this_thread::sleep_for (std::chrono::seconds(5));

	}		
}

/*
new function for reading the file, gives back the position that will be used to start over next time(arabo)
*/
streampos check_data_from_file(vector<float> *temperatures, vector<float> *times, streampos  position, string filename, bool *first){
	int counter = 1; //1st temp, 2nd time
	vector<float> temp;
	
	ifstream inputFile (filename, ios::in);
	
	//streampos startPos = inputFile.tellg();
	if((*first) == true){
		position = inputFile.tellg(); //position is the beginning of the file
		(*first) = false;
	}
	if(inputFile.is_open()){
		inputFile.seekg(position);
		string tempString;
		while( getline(inputFile, tempString)){
			//cout << "string  " << tempString << endl;
			istringstream ss(tempString );
			while (ss)
    			{
    				 string s;
      				 if (!getline( ss, s, ',' )) break;
					
				if(counter % 2 ==0){
					times[0].push_back(atof(s.c_str()));
				}else{
					temperatures[0].push_back(atof(s.c_str()));
				}
				counter++;
    			}
			position = inputFile.tellg();	
		}
		
		inputFile.close();
	}else{
		cerr << "Error while Opening " << filename << endl;
		return -1;
	}	
	/*
	for(int i=0; i<temperature.size(); i++){
		cout << " temp && time " << endl;
		for(int j = 0; j < temperature.at(i).size(); j++)
			cout << " " << temperature.at(i).at(j);
		cout << endl;
	}
	*/
	counter --;
	if(counter % 2 != 0){
		cerr << "mismatch in temperature/time, the number is not even " << endl;
		return -1;
	}



	return position;
}
//used to debug, to remove...
void printNumbers(TempTimes *tempTimes){
	//TempTimes *tempTimes = static_cast<TempTimes*>(tempTimesP);
	for(int i = 0; i< 10; i++){
		cout << "thread" << " print " << i << endl ;
	}
	(*tempTimes).temperatures.push_back(2.0);
	cout << "pusho dentro funzione thread..." << endl;
}

int run_static(string filename, vector<float> temperature, vector<float> times){
	if(load_data_from_file(filename, &temperature, &times) == -1){
		cerr << "error while loading the data! " << endl;
		return -1;
	}	
	
	for(int i=0; i<temperature.size(); i++){
		cout << "temperature "<<temperature.at(i) << "time "<<times.at(i) << endl;
	}
	
	cout << endl;
		
	list<Cycles> cycles = rainflow_algorithm(temperature,times, temperature.size());
	list<float> Ntci;

	if(cycles.back().getTemp1() == -1 && cycles.back().getTemp2() == -1 && cycles.back().getRange() == -1){
		printf("error in cycle calculation!");
		return -1;
	}
	
	cout << cycles.size() << "cycles" << endl;
	
	
	for (std::list<Cycles>::iterator it=cycles.begin(); it != cycles.end(); it++){
		cout << "range " << (*it).getRange();
		cout << " time " << (*it).getTime() << endl;
		//fake time
		//(*it).setTime(3.57);
		
		Ntci.push_back(coffin_manson(*it));
		//cout.precision(20);
		cout << "Ntci "<<Ntci.back()<<endl;
	}

	float MTTF = miner_rule(Ntci, cycles);

	cout << "MTTF " << MTTF << endl;	
	
	return 0;
}

int load_data_from_file(string filename, vector<float> *temperature, vector<float> *times){
	
	int counter = 1; //1st temp, 2nd time
	vector<float> temp;
	ifstream inputFile (filename, ios::in);
	if(inputFile.is_open()){
		string tempString;
		while(getline(inputFile, tempString)){
			//cout << "string  " << tempString << endl;
			istringstream ss(tempString );
			while (ss)
    			{
    				 string s;
      				 if (!getline( ss, s, ',' )) break;
					
				if(counter % 2 ==0){
					times[0].push_back(atof(s.c_str()));
				}else{
					temperature[0].push_back(atof(s.c_str()));
				}
				counter++;
    			}
		}
		inputFile.close();
	}else{
		cerr << "Error while Opening " << filename << endl;
		return -1;
	}	
	/*
	for(int i=0; i<temperature.size(); i++){
		cout << " temp && time " << endl;
		for(int j = 0; j < temperature.at(i).size(); j++)
			cout << " " << temperature.at(i).at(j);
		cout << endl;
	}
	*/
	counter --;
	if(counter % 2 != 0){
		cerr << "mismatch in temperature/time, the number is not even " << endl;
		return -1;
	}
	
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
		MTTF += (*it).getTime();
	}

	MTTF = Ntc * MTTF / m;

	return MTTF;

}
float coffin_manson(Cycles cycle){
	//Ntc[i] = Atc(dT[i] - Tth)^-b * e^(Eatc/(KTmax[i]))

	//the variable have been defined following section 9.2 of the paper [3]
	//variable definition
	int Atc = 1; //***empirically determinedconstant
	float dTi = abs(cycle.getTemp1() - cycle.getTemp2()); //amplitude of the ith thermal cycle
	int Tth = 0; //**temperature at which elastic deformation begins
	int b = 6; //**Coffin-Manson exponent constant
	float Eatc = 0.5; //**activation energy
	float Tmaxi = max(cycle.getTemp1(), cycle.getTemp2()); //maximum temperature in the ith thermal cycle.
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
	cout << "-f | --file specify input file: <temperature1, time1,........,temperatureN, timeN> " <<endl; //to-do specify type of input!
	cout << "-d | --dynamic | -s | --static choose the version of the algorithm" << endl;
	return;
}
