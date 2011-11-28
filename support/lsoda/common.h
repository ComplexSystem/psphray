
#define max( a , b )  ( (a) > (b) ? (a) : (b) )
#define min( a , b )  ( (a) < (b) ? (a) : (b) )

#define ETA 2.2204460492503131e-16
#define SQRTETA 1.4901161193847656e-08
/* newly added static variables */

extern int      imxer;
extern int      mord[3];
extern double   *yp1, *yp2;
extern double   sm1[13];

/* static variables for lsoda() */

extern double   ccmax, el0, h, hu, rc, tn;
extern int      illin, init, nhnil, ntrep, nslast, ierpj, iersl,
                jcur, l, meth, mused, maxord, maxcor, msbp, mxncf, n, nq, nst,
                nfe, nje, nqu, miter;
extern double   tsw, pdnorm;

/* no static variable for prja(), solsy() */
/* static variables for stoda() */

extern double   conit, crate, el[14], elco[13][14], hold, rmax, tesco[13][4];
extern int      ialth, ipup, lmax, nslp;
extern double   pdest, pdlast, ratio, cm1[13], cm2[6];
extern int      icount, irflag;

/* static variables for various vectors and the Jacobian. */

extern void * memory;
extern struct vec_t vec;
