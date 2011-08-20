#include <messages.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "config.h"
#include <array.h>

#include <gsl/gsl_randist.h>

typedef struct {
	char * name;
	char * filename;
	int type;  /* 0 for multi, 1 for mono */
	double mono_freq;
	ARRAY_DEFINE_S(freq, double)
	ARRAY_DEFINE_S(weight, double)
	gsl_ran_discrete_t * randist;
} Spec;

Spec * specs = NULL;

int N_SPECS = 0;

const int spec_get(const char * name) {
	int i;
	for(i = 0; i < N_SPECS; i++) {
		if(!strcmp(specs[i].name, name)) return i;
	}
	ERROR("spec %s unknown", name);
}

const char * spec_name(const int id) {
	return specs[id].name;
}

double spec_gen_freq(const int id) {
	switch(specs[id].type) {
	case 0:
		return specs[id].freq[gsl_ran_discrete(RNG, specs[id].randist)];
	case 1:
		return specs[id].mono_freq;
	}
}

static void spec_from_file(Spec * newitem, const char * filename);
void spec_init() {
	config_setting_t * list = config_lookup(CFG, "specs");
	if(list == NULL) {
		return;
	}
	N_SPECS = config_setting_length(list);
	specs = calloc(N_SPECS, sizeof(Spec));
	int i;
	for(i = 0; i < N_SPECS; i++) {
		config_setting_t * ele = config_setting_get_elem(list, i);
		specs[i].name = config_setting_name(ele);
		if(config_setting_type(ele) == CONFIG_TYPE_STRING) {
			specs[i].type = 0;
			spec_from_file(&specs[i], config_setting_get_string(ele));
		} else {
			specs[i].type = 1;
			specs[i].mono_freq = config_setting_get_float(ele);
		}
	}
}
static void spec_from_file(Spec * newitem, const char * filename) {
	FILE * fp = fopen(filename, "r");
	if(fp == NULL) {
		ERROR("failed to open %s", filename);
	}
	
	int NR = 0;
	char * line = NULL;
	size_t len = 0;
	int stage = 0;

	double freq, weight;
	while(0 <= getline(&line, &len, fp)) {
		if(line[0] == '#') {
			NR++;
			continue;
		}
		switch(stage) {
		case 0:
			if(2 != sscanf(line, "%le %le", &freq, &weight)) {
				ERROR("%s format error at %d", filename, NR);
			} else {
				* ARRAY_APPEND(newitem->freq, double) = freq;
				* ARRAY_APPEND(newitem->weight, double) = weight;
			}
			break;
		case 1:
			break;
		}
		NR++;
		if(stage == 1) break;
	}
	free(line);
	fclose(fp);
	
	newitem->randist = gsl_ran_discrete_preproc(newitem->weight_length, newitem->weight);
}