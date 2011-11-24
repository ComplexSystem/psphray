
#define max( a , b )  ( (a) > (b) ? (a) : (b) )
#define min( a , b )  ( (a) < (b) ? (a) : (b) )

#define ETA 2.2204460492503131e-16
typedef void    (*_lsoda_f) (double, double *, double *, void *);

extern int      g_nyh, g_lenyh;

/* newly added static variables */

extern int      ml, mu, imxer;
extern int      mord[3];
extern double   sqrteta, *yp1, *yp2;
extern double   sm1[13];

/* static variables for lsoda() */

extern double   ccmax, el0, h, hmin, hmxi, hu, rc, tn;
extern int      illin, init, mxstep, mxhnil, nhnil, ntrep, nslast, nyh, ierpj, iersl,
                jcur, jstart, kflag, l, meth, miter, maxord, maxcor, msbp, mxncf, n, nq, nst,
                nfe, nje, nqu;
extern double   tsw, pdnorm;
extern int      ixpr, jtyp, mused, mxordn, mxords;

/* no static variable for prja(), solsy() */
/* static variables for stoda() */

extern double   conit, crate, el[14], elco[13][14], hold, rmax, tesco[13][4];
extern int      ialth, ipup, lmax, nslp;
extern double   pdest, pdlast, ratio, cm1[13], cm2[6];
extern int      icount, irflag;

/* static variables for various vectors and the Jacobian. */

extern double **yh, **wm, *ewt, *savf, *acor;
extern int     *ipvt;
