#include "timer.h"
#include <math.h>
#include <time.h>

#define DELTA 0.001

milli ms_time() { return (1000. * clock() / CLOCKS_PER_SEC); }
