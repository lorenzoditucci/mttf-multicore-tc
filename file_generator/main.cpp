#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>

using namespace std;
int main(int argc, char* argv[]){
	string filename = "10000peaks_valleys2.txt";
	int N = 10000;
	
	ofstream file;
	file.open(filename);
	
	int tempVal = 0;
	
	int val1 = 1;
	int val2 = RAND_MAX - val1;
		
	bool flag = true;
	
	srand(static_cast<int>(time(0)));
	for(int i=0; i<N; i++){
		//srand(time(0));
		tempVal = rand() % val2 + val1; 
		//tempVal = rand() % 0 + RAND_MAX;
		file << tempVal << ","<<i<<endl;
		/*if(tempVal == RAND_MAX){
			flag = false;
			cout << "uguale a rand_max.." << endl;
		}else if(tempVal == 0){
			flag = true;
		} 
		*/
		//cout << "tempVal " << tempVal << endl;
		if(flag == false){
			val1 = 1;
			val2 = tempVal - val1;
			flag = true;
		}else{
			//if(tempVal == RAND_MAX){
			//	val1 = RAND_MAX/4;
			//}else{
			//	val1 = tempVal;
			//}
			
			val1 = tempVal;
			val2 = RAND_MAX - val1;
			flag = false;
		}
		//cout << "rand number between "<<val1<< " and " << val2 << endl;
	}

	file.close();
return 0;

}
