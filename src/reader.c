#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <messages.h>
#include "reader.h"
#include "reader-internal.h"

struct header_t;
struct _Reader {
	char * filename;
/* private */
	struct header_t * header;
	FILE * fp;
	int read_only;
	ReaderFuncs funcs;
};
#define _blockid (reader->funcs.blockid)
#define _blockname (reader->funcs.blockname)
#define _itemsize (reader->funcs.itemsize)
#define _pstart (reader->funcs.pstart)
#define _length (reader->funcs.length)
#define _npar (reader->funcs.npar)
#define _offset (reader->funcs.offset)
#define _read (reader->funcs.read)
#define _write (reader->funcs.write)

void massiveblack_get_reader_funcs(ReaderFuncs * funcs);
Reader * reader_new(char * format) {
	Reader * reader = calloc(sizeof(Reader), 1);
	if(!strcmp(format, "massiveblack")) {
		massiveblack_get_reader_funcs(&reader->funcs);
	} else if(!strcmp(format, "e5")) {
		massiveblack_get_reader_funcs(&reader->funcs);
	} else {
		ERROR("format %s unknown", format);
	}
	return reader;
}

void reader_open(Reader * reader, char * filename) {
	reader->filename = strdup(filename);
	reader->fp = fopen(filename, "r");
	reader->read_only = 1;
	char * error = NULL;
	reader->header = reader_alloc(reader, "header", -1);
	_read(reader->header, "header", NULL, 0, 1, reader->fp, &error);
	if(error) {
		ERROR("%s", error);
		free(error);
	}
}
void reader_create(Reader * reader, char * filename) {
	reader->filename = strdup(filename);
	reader->fp = fopen(filename, "w+");
	reader->header = reader_alloc(reader, "header", -1);
	reader->read_only = 0;
}

void reader_close(Reader * reader) {
	char * error = NULL;
	if(!reader->read_only) {
		_write(reader->header, "header", NULL, 0, 1, reader->fp, &error);
	}
	free(reader->header);
	fclose(reader->fp);
	reader->fp = NULL;
}

size_t reader_length(Reader * reader, char * blk) {
	char * error = NULL;
	size_t l = _length(reader->header, blk, &error);
	if(error) {
		ERROR("%s", error);
	}
	return l;
}

size_t reader_npar(Reader * reader, int ptype) {
	char * error = NULL;
	return _npar(reader->header, ptype, &error);
}

void * reader_alloc(Reader * reader, char * blk, int ptype) {
	char * error = NULL;
	size_t npar = 0;
	if(ptype != -1) {
		npar = reader_npar(reader, ptype);
	} else {
		npar = reader_length(reader, blk);
	}
	size_t b = _itemsize(blk, &error);
	if(error) {
		ERROR("%s", error);
		free(error);
	}
	return calloc(b, npar);
}

void reader_read(Reader * reader, char * blk, int ptype, void *buf) {
	char * error = NULL;
	size_t pstart = 0;
	size_t npar = 0;
	if(ptype != -1) {
		pstart = _pstart(reader->header, blk, ptype, &error);
		if(error) {
			ERROR("%s", error);
			free(error);
		}
		npar = reader_npar(reader, ptype);
	} else {
		npar = reader_length(reader, blk);
	}
	_read(reader->header, blk, buf, pstart, npar, reader->fp, &error);
	if(error) {
		ERROR("%s", error);
		free(error);
	}
}

void reader_write(Reader * reader, char * blk, int ptype, void *buf) {
	char * error = NULL;
	size_t pstart = 0;
	size_t npar = 0;
	if(ptype != -1) {
		pstart = _pstart(reader->header, blk, ptype, &error);
		npar = reader_npar(reader, ptype);
		if(error) {
			ERROR("%s", error);
			free(error);
		}
	} else {
		npar = reader_length(reader, blk);
	}
	_write(reader->header, blk, buf, pstart, npar, reader->fp, &error);
	if(error) {
		ERROR("%s", error);
		free(error);
	}
}

int reader_has(Reader * reader, char * blk) {
	char * error = NULL;
	int id = _blockid(blk, &error);
	if(error) {
		free(error);
		return 0;
	}
	return 1;
}
