#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <list>
#include "cycles.h"

std::list<Cycles> rainflow_algorithm(int *temperatures, int N);
int read_next_peak_valley(int *temperatures, int *startingIndex, int N);
int clean_reorganize(int *e, int i);
void print_vector(int *e, int N, int s);
Cycles create_error_cycle(Cycles cycle);