#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>

using namespace std;
int main(int argc, char* argv[]){
	string filename = "1000000peak_valleys2.txt";
	int N = 1000000;
	
	ofstream file;
	file.open(filename);
	
	int tempVal = 0;
	
	int val1 = 40;
	int val2 = 50;
		
	bool flag = true;
	
	srand(static_cast<int>(time(0)));
	for(int i=0; i<N; i++){
		tempVal = rand() % val2 + val1; 
		file << tempVal << ","<<i<<endl;
		//cout << "tempVal " << tempVal << endl;
		
		if( i == 0){
			//where to go?
			flag = tempVal > val1 + 0.5 * val2 ? false  : true;
		}
		
		//cout << " numbers from " << val1 << " to " << (val2 + val1)  << " TEMPVAL " << tempVal << " flag is " << flag << endl;
		if(flag == false){
			val1 = 40;
			val2 = tempVal - val1;
			flag = true;
		}else{
			
			val1 = tempVal + 1;
			val2 = 90 - val1;
			flag = false;
		}
		//cout << "rand number between "<<val1<< " and " << val2 << endl;
	}

	file.close();
return 0;

}
