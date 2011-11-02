#ifndef _LAMBERT_COMPUTE_WITH_OFFSET
#define _LAMBERT_COMPUTE_WITH_OFFSET



#include "fastmath.h"
#include "LambertWs.h"
using namespace std;

FMmatrix<double> lambert_compute_with_offset(const FMmatrix<double>& in_omeg, double z, double lam_offset);

#endif
