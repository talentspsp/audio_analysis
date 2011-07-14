/* Lambert W function, -1 branch. 

   original written by
   K M Briggs 2001 Apr 09
   Keith dot Briggs at bt dot com

   revised a little

   Returned value W(z) satisfies W(z)*exp(W(z))=z
   Valid input range: -1/e <= z    < 0
   Output range:      -1   >= W(z) > -infinity

   See LambertW.c for principal branch.
   Together these form the only purely real branches.

   Test: 
     gcc -DTESTW LambertW1.c -o LambertW1 -lm && LambertW1
   Library:
     gcc -O3 -c LambertW1.c 
*/

#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace std;

double LambertW1(const double z) {
  int i; 
  const double eps=4.0e-16, em1=0.3678794411714423215955237701614608; 
  double p,e,t,w,l1,l2;
  if (z<-em1 || z>=0.0|| isinf(z) || isnan(z) ) { 
    fprintf(stderr,"LambertW1: bad argument %g, exiting.\n",z); 
    return 1; 
  }
  /* initial approx for iteration... */
  if (z<-1e-6) { /* series about -1/e */
    p=-sqrt(2.0*(2.7182818284590452353602874713526625*z+1.0));
    w=-1.0+p*(1.0+p*(-0.333333333333333333333+p*0.152777777777777777777777)); 
  } else { /* asymptotic near zero */
    l1=log(-z);
    l2=log(-l1);
    w=l1-l2+l2/l1;
  }
  if (fabs(p)<1e-4) return w;
  for (i=0; i<10; i++) { /* Halley iteration */
    e=exp(w); 
    t=w*e-z;
    p=w+1.0;
    t/=e*p-0.5*(p+1.0)*t/p; 
    w-=t;
    if (fabs(t)<eps*(1.0+fabs(w))) return w; /* rel-abs error */
  }
  /* should never get here */
  // fprintf(stderr,"LambertW1: No convergence at z=%g, final value is %g.\n",z,w); 
  return w;
}


double LambertW(const double z) {
  int i; 
  const double eps=4.0e-16, em1=0.3678794411714423215955237701614608; 
  double p,e,t,w;
  if (z<-em1 || isinf(z) || isnan(z)) { 
    fprintf(stderr,"LambertW: bad argument %g, exiting.\n",z);
    return -1;
  }
  if (0.0==z) return 0.0;
  if (z<-em1+1e-4) { // series near -em1 in sqrt(q)
    double q=z+em1,r=sqrt(q),q2=q*q,q3=q2*q;
    return 
     -1.0
     +2.331643981597124203363536062168*r
     -1.812187885639363490240191647568*q
     +1.936631114492359755363277457668*r*q
     -2.353551201881614516821543561516*q2
     +3.066858901050631912893148922704*r*q2
     -4.175335600258177138854984177460*q3
     +5.858023729874774148815053846119*r*q3
     -8.401032217523977370984161688514*q3*q;  // error approx 1e-16
  }
  /* initial approx for iteration... */
  if (z<1.0) { /* series near 0 */
    p=sqrt(2.0*(2.7182818284590452353602874713526625*z+1.0));
    w=-1.0+p*(1.0+p*(-0.333333333333333333333+p*0.152777777777777777777777)); 
  } else 
    w=log(z); /* asymptotic */
  if (z>3.0) w-=log(w); /* useful? */
  for (i=0; i<10; i++) { /* Halley iteration */
    e=exp(w); 
    t=w*e-z;
    p=w+1.0;
    t/=e*p-0.5*(p+1.0)*t/p; 
    w-=t;
    if (fabs(t)<eps*(1.0+fabs(w))) return w; /* rel-abs error */
  }
  /* should never get here */
  /*fprintf(stderr,"LambertW: No convergence at z=%g, exiting.\n",z); 
    exit(1);*/
  return w;
}


//this function calculates the lambert function w(z) for z=-exp(x), when x<-500
double lambert_arg_outof_range(const double x)
{
  const double eps=4.0e-16;
  double w,w0;
  w=x;
#ifdef TESTW
  int iter=0;
#endif
  while(1)
    {
      w0=w;
      w=x-log(fabs(w));
      if(fabs((w-w0)/w0)<eps)
	break;
#ifdef TESTW
      iter++;
#endif
    }
#ifdef TESTW
  printf("iteration for lambert_arg_outof_range: %d\n",iter);
#endif

  return w;
}


#ifdef TESTW
/* test program...  */
int main() {
  /*int i;
  double z,w,err;
  for (i=0; i<100; i++) {
    z=i/100.0-0.3678794411714423215955; 
    if (z>=0.0) break;
    w=LambertW1(z); 
    err=exp(w)-z/w;
    printf("W(%8.4f)=%22.16f, check: exp(W(z))-z/W(z)=%e\n",z,w,err);
  }
  z=-1.0e-2; 
  for (i=0; i<100; i++) {
    z/=10000.0; 
    w=LambertW1(z); 
    err=exp(w)-z/w;
    printf("W(%8.4g)=%22.16f, check: exp(W(z))-z/W(z)=%e\n",z,w,err);
  }
  printf("test lamber out of range\n");
  double x=-500;
  while(x>-1000)
    {
      z=-exp(x);
      w=lambert_arg_outof_range(x);
      err=exp(w)-z/w;
      printf("W(%8.4g)=%22.16f, check: exp(W(z))-z/W(z)=%e\n",z,w,err);
      x-=50;
      }*/
  double z=-0.36415;
  double r=LambertW1(z);
  printf("result is %f\n",r);
  
  return 0;
}
#endif
