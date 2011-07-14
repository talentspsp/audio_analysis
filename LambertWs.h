#ifndef _LAMBERTW1_H
#define _LAMBERTW1_H

//-1 branch of lambert function, if fail, return 1.(the -1 branch should never return positive number)
double LambertW1(const double z);

//0 branck of lambert funcion. if fail, return -1.
double LambertW(const double z);
double lambert_arg_outof_range(const double x);

#endif
