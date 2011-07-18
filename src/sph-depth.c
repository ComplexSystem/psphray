#include <math.h>

static double _kernel_depth[] = {
#include "kernel.akline"

};


/* returns the sph kernel W * h**3 */ 
float sph_Wh3(const float r_h) {
	static const float norm = 8.0 / M_PI;
	const float r_h2 = r_h * r_h;

	if(r_h <= 0.5) {
		return norm * (1.0 - 6.0 * r_h2+ 6.0 * r_h2 * r_h);
	}
	if(r_h <= 1.0) {
		const float dd = 1.0 - r_h;
		return norm * 2.0 * dd * dd * dd;
	}
	return 0.0;

}

/* */
float sph_depth(const float r_h) {
	static const N = sizeof(_kernel_depth) / sizeof(double);

	int n = N * r_h;
	if(n < N && n >=0) return _kernel_depth[n];
	else return 0.0;

}
