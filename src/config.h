#include <libconfig/libconfig.h>
#include <gsl/gsl_rng.h>

typedef struct {
	double redshift;
	double age;
	double duration;
	size_t ngas;
	size_t nticks;
	size_t nray;
	ssize_t nrec;

	const char * source;
	const char * snapshot;
	const char * format;
	struct {
		size_t * steps;
		size_t nsteps;
		size_t nfiles;
		const char * filename;
	} output;
} Epoch;

extern config_t CFG[];
extern int CFG_WRITE_INIT;
extern int CFG_ADIABATIC;
extern int CFG_ISOTHERMAL;
extern int CFG_ON_THE_SPOT;
extern int CFG_COMOVING;
extern int CFG_H_ONLY;
extern int CFG_NO_PHOTON;
extern int CFG_DUMP_HOTSPOTS;
extern int CFG_SECONDARY_IONIZATION;
extern int CFG_TRACE_ONLY;
extern int CFG_OCTTREE_PPC;
extern int CFG_DISABLE_LTE;
extern double CFG_FAKE_TEMPERATURE;
extern int CFG_ENABLE_EOS;
extern double CFG_EOS_CLOUD_TEMPERATURE;
extern double CFG_RECOMBINE_THRESHOLD;
extern double CFG_BOOST_SOURCE_FACTOR;

extern gsl_rng * RNG;
extern Epoch * EPOCHS;
extern int N_EPOCHS;

void cfg_init(char * filename);
void cfg_dump(char * filename);
void cfg_dump_stream(FILE * file);

double config_setting_parse_units(config_setting_t * e);
int config_setting_parse_units_elem(config_setting_t * e, int elem, double * rt);
int config_setting_parse_units_member(config_setting_t * e, char * member, double * rt);
config_setting_t * config_setting_ensure_member(config_setting_t * e, char * member, int type);
config_setting_t * config_ensure(config_t * config, char * path, int type);

double units_parse(char * units);
double units_format(double value, char * units);

extern double U_CM;
extern double U_GRAM;
extern double U_SEC;
extern double U_M;
extern double U_KM;
extern double U_KPC;
extern double U_YR;
extern double U_MYR;
extern double U_KG;
extern double U_MSUN;
extern double U_MPROTON;

extern double U_KELVIN;
extern double U_ERG;
extern double U_J;
extern double U_EV;
extern double U_RY_ENG;

extern double C_1_CMH;
extern double C_1_GRAMH;
extern double C_1_SECH;
extern double C_HMF;
extern double C_HEMF;
extern double C_OMEGA_L;
extern double C_OMEGA_M;
extern double C_OMEGA_B;
extern double C_H;
extern double C_BOLTZMANN;
extern double C_PLANCK;
extern double C_SPEED_LIGHT;
extern double U_MPROTON_OVER_C_BOLTZMANN;
extern double C_BOLTZMANN_OVER_U_MPROTON;
extern double C_H_PER_MASS;
extern double C_HE_PER_MASS;
extern double C_HI_ENERGY;
extern double C_HEI_ENERGY;
extern double C_HEII_ENERGY;
extern double C_SOLAR_LUM;

double z2t(double z);
double t2z(double z);
static inline double ieye2T(const double ie, const double ye) {
	return U_MPROTON_OVER_C_BOLTZMANN * ie / ( C_HEMF * 0.25 + 
			C_HMF + C_HMF * ye) * 2.0 / 3.0 ;

}
static inline double Tye2ie(const double T, const double ye) {
	return T * C_BOLTZMANN_OVER_U_MPROTON * ( C_HEMF * 0.25 + 
			C_HMF + C_HMF * ye) / 2.0 * 3.0 ;
}

extern int AR_LOG_T;
extern int AR_HI_CI;
extern int AR_HI_CEC;
extern int AR_HI_CIC;
extern int AR_HII_RC_A;
extern int AR_HII_RCC_A;
extern int AR_HII_RC_B;
extern int AR_HII_RCC_B;
extern int AR_HEI_CI;
extern int AR_HEII_CI;
extern int AR_HEII_RC_A;
extern int AR_HEII_RC_B;
extern int AR_HEIII_RC_A;
extern int AR_HEIII_RC_B;
extern int AR_E_BREMC;
extern int AR_E_COMPC;
extern int XS_FREQ;
extern int XS_HI;
extern int XS_HEI;
extern int XS_HEII;

const double ar_get(const int id, const double logT);
const double xs_get(const int id, const double freq);

const int spec_get(const char * name);
double spec_gen_freq(const int id);
double spec_N_from_lum(const int id, double lum);
double spec_lum_from_N(const int id, double lum);

extern int LTE_FREQ_HI;
extern int LTE_FREQ_HEI;
extern int LTE_FREQ_HEII;
double lte_gen_freq(const int id, const double logT);

extern int SECION_PHI_HI;
extern int SECION_PHI_HEI;
extern int SECION_EH;

void secion_init();

double secion_get(int id, double E, double log10x);
double eos_get_cloud_fraction(const double dens_phys);
double eos_get_egyhot(const double dens_phys);
