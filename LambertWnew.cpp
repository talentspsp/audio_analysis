#include <cmath>
#include <cstdio>
#include "LambertWnew.h"
using namespace std;
// Halley iteration
double halley_iteration( double x, double w_initial, unsigned int max_iters)
{
	double w = w_initial;

	for( int i = 0; i < max_iters ; i++) {
		double tol;
		const double e = exp(w);
		const double p = w + 1.0;
		double t = w*e - x;

		if (w > 0)
			t = (t/p)/e;  // Newton iteration
		else
			t /= e*p - 0.5*(p + 1.0)*t/p;  // Halley iteration
		w -= t;

		if( fabs(t) < 1e-10)
			return w;
	}

	// Should never get here
	return w;
}


// series which appears for q near zero;
// only the argument is different for the different branches
double series_eval( double r)
{
	static const double c[12] = {
		-1.0,
		+2.331643981597124203363536062168,
		-1.812187885639363490240191647568,
		+1.936631114492359755363277457668,
		-2.353551201881614516821543561516,
		+3.066858901050631912893148922704,
		-4.175335600258177138854984177460,
		+5.858023729874774148815053846119,
		-8.401032217523977370984161688514,
		+12.250753501314460424,
		-18.100697012472442755,
		+27.029044799010561650
	};
	const double t_8 = c[8] + r*(c[9] + r*(c[10] + r*c[11]));
	const double t_5 = c[5] + r*(c[6] + r*(c[7]  + r*t_8));
	const double t_1 = c[1] + r*(c[2] + r*(c[3]  + r*(c[4] + r*t_5)));
	return c[0] + r*t_1;
}

// Primary branch
double lambert_W0( double x)
{
	const double one_over_E = 1.0/M_E;
	const double q = x + one_over_E;

	if( x == 0.0)
		return 0.0;

	else if( q < 0.0) {
		// Strictly speaking this is an error. But because of the
		// arithmetic operation connecting x and q, I am a little
		// lenient in case of some epsilon overshoot. The following
		// answer is quite accurate in that case.
		return -1.0;
	}
	else if( q == 0.0)
		return -1.0;

	else if( q < 1.0e-03)
		// series near -1/E in sqrt(q)
		return series_eval( sqrt( q));

	else {
		static const unsigned int MAX_ITERS = 10;
		double w;

		if ( x < 1.0) {
			// obtain initial approximation from series near x=0;
			// no need for extra care, since the Halley iteration
			// converges nicely on this branch
			const double p = sqrt( 2.0 * M_E * q);
			w = -1.0 + p*(1.0 + p*(-1.0/3.0 + p*11.0/72.0));
		}
		else {
			// obtain initial approximation from rough asymptotic
			w = log( x);
			if( x > 3.0)
				w -= log( w);
		}

		return halley_iteration( x, w, MAX_ITERS);
	}
}


// Secondary branch
double lambert_Wm1( double x)
{
	if(x > 0.0)
		return lambert_W0( x);

	else if( x == 0.0)
		return -HUGE_VAL;

	else {
		static const unsigned int MAX_ITERS = 32;
		const double one_over_E = 1.0/M_E;
		const double q = x + one_over_E;
		double w;

		if( q < 0.0)
			// As in the W0 branch above, return some reasonable answer anyway.
			return -1.0;

		if( x < -1.0e-6) {
			// Obtain initial approximation from series about q = 0,
			// as long as we're not very close to x = 0.
			// Use full series and try to bail out if q is too small,
			// since the Halley iteration has bad convergence properties
			// in finite arithmetic for q very small, because the
			// increment alternates and p is near zero.
			w = series_eval( -sqrt( q));
			if( q < 3.0e-3)
				// this approximation is good enough
				return w;
		}
		else {
			// Obtain initial approximation from asymptotic near zero.
			const double L_1 = log( -x);
			const double L_2 = log( -L_1);
			w = L_1 - L_2 + L_2/L_1;
		}

		return halley_iteration(x, w, MAX_ITERS);
	}
}

#ifdef TESTW
/* test program...  */
int main() {
  int i;
  double z,w,err;
  for (i=0; i<100; i++) {
    z=i/100.0-0.3678794411714423215955; 
    if (z>=0.0) break;
    w=lambert_Wm1(z); 
    err=exp(w)-z/w;
    printf("W(%8.4f)=%22.16f, check: exp(W(z))-z/W(z)=%e\n",z,w,err);
  }
  z=-1.0e-2; 
  for (i=0; i<100; i++) {
    z/=10000.0; 
    w=lambert_Wm1(z); 
    err=exp(w)-z/w;
    printf("W(%8.4g)=%22.16f, check: exp(W(z))-z/W(z)=%e\n",z,w,err);
  }
  /*printf("test lamber out of range\n");
  double x=-500;
  while(x>-1000)
    {
      z=-exp(x);
      w=lambert_arg_outof_range(x);
      err=exp(w)-z/w;
      printf("W(%8.4g)=%22.16f, check: exp(W(z))-z/W(z)=%e\n",z,w,err);
      x-=50;
      }*/
    /* double z=-0.36415;
  double r=LambertW1(z);
  printf("result is %f\n",r);*/
  
  return 0;
}
#endif
