
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "cminpack.h"
#include "lmdif1.c"
#include "lmdif.c"
#include "dpmpar.c"
#include "enorm.c"
#include "fdjac2.c"
#include "qrfac.c"
#include "lmpar.c"
#include "qrsolv.c"

#define real __cminpack_real__

/* the following struct defines the data points */
typedef struct {
	int m;
	real* x;
	real* y;
} fcndata_t;

// please follow the exact funtion form, DO NOT change this
// lmdif1 has assume the form of the exact function pointer.
int fcn(void* p, int m, int n, const real* x, real* fvec, int iflag); 
// void* p -> fcndata_t *   intput data
// m 
// int n  number of variables
// const int realI(double)* x   array of variables, we have n variables
// real(double)* fvec  array of outputs
// int iflag :  working int, just leave it.

int main()
{
	int info, lwa, iwa[2];
	real tol, fnorm, x[2], fvec[9], wa[40]; 
	const int m = 9;
	const int n = 2;
	/* auxiliary data (e.g. measurements) */
	real yi[9] = { 58, 63, 57, 65, 62, 66, 58, 59, 62 };
	real xi[9] = { 160,165,158,172,159,176,160,162,171 };
	fcndata_t data;
	data.m = m;
	data.y = yi;
	data.x = xi;

	/* the following starting values provide a rough fit. */

	x[0] = 1.; // inital guess of parameters 
	x[1] = 1.;

	lwa = 40;  // working 

	/* set tol to the square root of the machine precision.  unless high
	   precision solutions are required, this is the recommended
	   setting. */

	tol = sqrt(__cminpack_func__(dpmpar)(1));

	info = __cminpack_func__(lmdif1)(fcn, &data, m, n, x, fvec, tol, iwa, wa, lwa);
	//tol auto generated in dpmpar, no need to change
	//iwa array of n                                                     working array, just allocate the memory for them
	//wa array of no less than n*m + 5n + m            working array, just allocate the memory for them
	//lwa len of wa                                                     working array, just allocate the memory for them

	fnorm = __cminpack_func__(enorm)(m, fvec);

	//  lmdif1 is only a minmize method, so f(X_i)  = Y_i  is a proper input function

	printf("      final l2 norm of the residuals%15.7g\n\n", (double)fnorm);
	printf("      exit parameter                %10i\n\n", info);
	printf("      final approximate solution\n\n %15.7g%15.7g\n",
		(double)x[0], (double)x[1]);
	return 0; 
}

int fcn(void* p, int m, int n, const real* x, real* fvec, int iflag)
{
	/* function fcn for lmdif1 example */
	//  p fcndata_t  here is the c lanuage version void* -> fcndata_t
	const real* xi = ((fcndata_t*)p)->x;
	const real* yi = ((fcndata_t*)p)->y;
	assert(m == 9 && n == 2);
	(void)iflag;

	for (int i = 0; i < 9; ++i)
	{
		fvec[i] = x[0] + x[1] * xi[i] - yi[i];
	}
	return 0;
}

