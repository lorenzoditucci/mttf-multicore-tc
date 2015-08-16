#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <vector>
#include <cmath>
#include "cycles.h"

using namespace std;

std::list<Cycles> rainflow_algorithm(vector<float> temperatures, int N);
float read_next_peak_valley(vector<float> temperatures, int *startingIndex, int N);
int clean_reorganize(float *e, int i);
void print_vector(int *e, int N, int s);
Cycles create_error_cycle(Cycles cycle);
