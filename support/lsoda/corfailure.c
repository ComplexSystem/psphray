#include "lsoda.h"
#include "common.h"
#include "lsoda_internal.h"
#include <math.h>

int corfailure(struct common_t * common, int neq, double *told, double *rh, int *ncf, double hmin)
{
	int             j, i1, i;

	ncf++;
	_C(rmax) = 2.;
	_C(tn) = *told;
	for (j = _C(nq); j >= 1; j--)
		for (i1 = j; i1 <= _C(nq); i1++) {
			for (i = 1; i <= neq; i++)
				_C(yh)[i1][i] -= _C(yh)[i1 + 1][i];
		}
	if (fabs(_C(h)) <= hmin * 1.00001 || *ncf == MXNCF) {
		return 2;
	}
	*rh = 0.25;
	_C(ipup) = _C(miter);
	return 1;
}

