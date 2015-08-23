#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "cycles.h"

void Cycles::setTemp1(float temp){
	temp1 = temp;
}

void Cycles::setTemp2(float temp){
	temp2 = temp;
}

void Cycles::setTime(float time_n){
	time = time_n;
}

void Cycles::setRange(float temp){
	range = temp;
}

float Cycles::getTemp1(){
	return temp1;
}

float Cycles::getTemp2(){
	return temp2;
}

float Cycles::getTime(){
	return time;
}

float Cycles::getRange(){
	return range;
}
