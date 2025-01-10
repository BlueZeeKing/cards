#pragma once
#include <cstdlib>
#include <ctime>
using namespace std;

// this should only be called once at the beginning of the program
void init_random_seed();

int random_between(int min, int max);