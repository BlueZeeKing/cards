#include "randomutils.h"
#include <cstdlib>
#include <ctime>
using namespace std;

void init_random_seed() { srand(time(NULL)); }

int random_between(int min, int max) { return rand() % (max - min + 1) + min; }
