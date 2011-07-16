typedef struct _Reader Reader;
Reader * reader_new(char * format);
void * reader_alloc(Reader * reader, char * blk, int ptype);
void reader_read(Reader * reader, char * blk, int ptype, void * buf);
int reader_has(Reader * reader, char * blk);
void reader_write(Reader * reader, char * blk, int ptype, void *buf);
size_t reader_length(Reader * reader, char * blk);
void reader_open(Reader * reader, char * filename);
size_t reader_npar(Reader * reader, int ptype);
void reader_close(Reader * reader);
