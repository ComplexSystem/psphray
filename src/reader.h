typedef struct _Reader Reader;
typedef struct _ReaderConstants {
	size_t N[6];
	size_t Ntot[6];
	size_t Nfiles;
	double mass[6];
	double boxsize;
	double redshift;
	double time; /* 1 / redshift, not the code time !!*/
	double OmegaL;
	double OmegaM;
	double OmegaB;
	double h;
	double PhysDensThresh;
} ReaderConstants;
Reader * reader_new(const char * format);
void reader_destroy(Reader * reader);
char * reader_make_filename(const char * snapshot, int id);
void * reader_alloc(Reader * reader, const char * blk, int ptype);
size_t reader_itemsize(Reader * reader, const char * blk);
void reader_read(Reader * reader, const char * blk, int ptype, void * buf);
int reader_has(Reader * reader, const char * blk);
void reader_update_header(Reader * reader);
void reader_update_constants(Reader * reader);
void reader_write(Reader * reader, const char * blk, int ptype, void *buf);
size_t reader_length(Reader * reader, const char * blk);
void reader_open(Reader * reader, const char * filename);
size_t reader_npar(Reader * reader, int ptype);
ReaderConstants * reader_constants(Reader * reader);
void reader_close(Reader * reader);
