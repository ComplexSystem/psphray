#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <messages.h>
#include "config.h"
#include "reader.h"
#define IDHASHBITS 24
#define IDHASHMASK ((((size_t)1) << IDHASHBITS) - 1)
const float Hmf = 0.76;

typedef struct {
	intptr_t *head;
	intptr_t *next;
} IDHash;

typedef struct {
	float (*pos)[3];
	float *xHI;
	float *mass;
	float *T;
	unsigned long long * id;
	IDHash idhash;
} PSystem;

PSystem psys = {0};
static float dist(const float p1[3], const float p2[3]);

void idhash_build(IDHash * h, unsigned long long * id, size_t n) {
	h->head = malloc(sizeof(intptr_t) * (((size_t)1) << IDHASHBITS));
	h->next = malloc(sizeof(intptr_t) * n);
	intptr_t i;
	memset(h->head, -1, sizeof(intptr_t) * (((size_t)1) << IDHASHBITS));
	memset(h->next, -1, sizeof(intptr_t) * n);
	for(i = 0; i < n; i++) {
		intptr_t hash = id[i] & IDHASHMASK;
		h->next[i] = h->head[hash];
		h->head[hash] = i;
	}
}

void psystem_switch_epoch(int i) {
	Reader * r0 = reader_new(EPOCHS[i].format);
	char * fname0 = reader_make_filename(EPOCHS[i].snapshot, 0);
	reader_open(r0, fname0);
	free(fname0);
	ReaderConstants * c = reader_constants(r0);

	/* read stuff */

	if( i == 0) {
		size_t ngas = EPOCHS[i].ngas;
		int fid;
		size_t nread = 0;
		psys.pos = calloc(sizeof(float) * 3, ngas);
		psys.mass = calloc(sizeof(float), ngas);
		psys.id = calloc(sizeof(unsigned long long), ngas);
		psys.T = calloc(sizeof(float), ngas);
		psys.xHI = calloc(sizeof(float), ngas);

		for(fid = 0; fid < c->Nfiles; fid ++) {
			Reader * r = reader_new(EPOCHS[i].format);
			char * fname = reader_make_filename(EPOCHS[i].snapshot, fid);
			reader_open(r, fname);
			printf("%s nread = %lu/%lu ptr=%p(%p)\n", fname,  nread, ngas, psys.pos[nread], psys.pos);
			free(fname);
			reader_read(r, "pos", 0, psys.pos[nread]);
			reader_read(r, "mass", 0, &psys.mass[nread]);
			float * ie = reader_alloc(r, "ie", 0);
			float * ye = reader_alloc(r, "ye", 0);
			intptr_t ipar;
			for(ipar = 0; ipar < reader_npar(r, 0); ipar++) {
				psys.T[ipar+nread] = ie[ipar] / ( (1.0 - Hmf) * 0.25 + Hmf + Hmf * ye[ipar]) * 2.0 / 3.0;
			}
			if(reader_itemsize(r, "id") == 4) {
				unsigned int * id = reader_alloc(r, "id", 0);
				reader_read(r, "id", 0, id);
				intptr_t ipar;
				for(ipar = 0; ipar < reader_npar(r, 0); ipar++) {
					psys.id[nread + ipar] = id[ipar];
				}
				free(id);
			} else {
				reader_read(r, "id", 0, &psys.id[nread]);
			}
			nread += reader_npar(r, 0);
			free(ie);
			free(ye);
			reader_destroy(r);
		}
		idhash_build(&psys.idhash, psys.id, nread);
	} else {
		int fid;
		for(fid = 0; fid < c->Nfiles; fid ++) {
			Reader * r = reader_new(EPOCHS[i].format);
			char * fname = reader_make_filename(EPOCHS[i].snapshot, fid);
			reader_open(r, fname);
			double distsum = 0.0;
			size_t lookups = 0;
			size_t lost = 0;
			float (*pos)[3] = reader_alloc(r, "pos", 0);
			float * mass = reader_alloc(r, "mass", 0);
			float * ie = reader_alloc(r, "ie", 0);
			float * ye = reader_alloc(r, "ye", 0);
			reader_read(r, "pos", 0, pos);
			reader_read(r, "mass", 0, mass);
			reader_read(r, "ie", 0, ie);
			reader_read(r, "ye", 0, ye);

			unsigned long long * id = calloc(8, reader_npar(r, 0));
			if(reader_itemsize(r, "id") == 4) {
				unsigned int * sid = reader_alloc(r, "id", 0);
				reader_read(r, "id", 0, id);
				intptr_t ipar;
				for(ipar = 0; ipar < reader_npar(r, 0); ipar++) {
					id[ipar] = sid[ipar];
				}
				free(sid);
			} else {
				reader_read(r, "id", 0, id);
			}

			intptr_t ipar;
			for(ipar = 0; ipar < reader_npar(r, 0); ipar++) {
				intptr_t hash = id[ipar] & IDHASHMASK;
				intptr_t best_jpar = -1;
				float mindist = 0.0;
				intptr_t jpar;
				for(jpar = psys.idhash.head[hash];
					jpar != -1; jpar = psys.idhash.next[jpar]) {
					float dst = dist(pos[ipar], psys.pos[jpar]);
					if(best_jpar == -1 || dst < mindist) {
						mindist = dst;
						best_jpar = jpar;
					}
					lookups++;
				}
				if(best_jpar != -1) {
					memcpy(&psys.pos[best_jpar][0], &pos[ipar][0], sizeof(float) * 3);
					distsum += mindist;
					psys.mass[best_jpar] = mass[ipar];
					psys.T[best_jpar] = ie[ipar] / ( (1.0 - Hmf) * 0.25 + Hmf + Hmf * ye[ipar]) * 2.0 / 3.0;
				} else {
					lost++;
				}
			}
			
			printf("%s meandist = %g meanlookup = %ld, lost = %ld\n", fname, distsum/reader_npar(r, 0),
					lookups / reader_npar(r, 0), lost);
			free(fname);
			free(mass);
			free(pos);
			free(ye);
			free(ie);
			free(id);
			reader_destroy(r);
		}
	}
	reader_destroy(r0);
}
static float dist(const float p1[3], const float p2[3]) {
	int d;
	double result = 0.0;
	for (d = 0; d < 3; d++) {
		float dd = p1[d] - p2[d];
		result += dd *dd;
	}
	return sqrt(result);
}
