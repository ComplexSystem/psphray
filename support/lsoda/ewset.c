
#include <math.h>
#include <stdio.h>
int ewset(double * ewt, int itol, double *rtol, double *atol, double *ycur, int n)
{
	int             i;

	switch (itol) {
	case 1:
		for (i = 1; i <= n; i++)
			ewt[i] = rtol[1] * fabs(ycur[i]) + atol[1];
		break;
	case 2:
		for (i = 1; i <= n; i++)
			ewt[i] = rtol[1] * fabs(ycur[i]) + atol[i];
		break;
	case 3:
		for (i = 1; i <= n; i++)
			ewt[i] = rtol[i] * fabs(ycur[i]) + atol[1];
		break;
	case 4:
		for (i = 1; i <= n; i++)
			ewt[i] = rtol[i] * fabs(ycur[i]) + atol[i];
		break;
	}
	for (i = 1; i <= n; i++) {
		if (ewt[i] <= 0.) {
			fprintf(stderr, "[lsoda] ewt[%d] = %g <= 0.\n", i, ewt[i]);
			return 0;
		}
		ewt[i] = 1. / ewt[i];
	}
	return 1;
}				/* end ewset   */
