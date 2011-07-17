
static inline char * bitmask_alloc(size_t size) {
	char * rt = malloc(((size + 7) >> 3) + sizeof(size_t));
	*(size_t *) rt = size;
	return rt;
}
static inline void bitmask_set_all(char * mask) {
	size_t size = *(size_t *) mask;
	size_t total = (size + 7 ) >> 3;
	char * buf = mask + sizeof(size_t);
	memset(buf, -1, total);
	unsigned int m = ((1 << ((size & 7))) -1);
	buf[total - 1] &= m;
}

static inline void bitmask_clear_all(char * mask) {
	size_t size = *(size_t *) mask;
	size_t total = (size + 7 ) >> 3;
	char * buf = mask + sizeof(size_t);
	memset(buf, 0, total);
}

static inline void bitmask_set(char * mask, intptr_t idx) {
	char * buf = mask + sizeof(size_t);
	int offset = idx & 7;
	int bit = 1 << offset;
	buf[idx >> 3] |= bit;
}

static inline void bitmask_clear(char * mask, intptr_t idx) {
	char * buf = mask + sizeof(size_t);
	int offset = idx & 7;
	int bit = 1 << offset;
	buf[idx >> 3] &= ~bit;
}

static inline size_t bitmask_test(char * mask, intptr_t idx) {
	char * buf = mask + sizeof(size_t);
	int offset = idx & 7;
	int bit = 1 << offset;
	return (buf[idx >> 3] & bit) != 0;
}
static inline size_t bitmask_sum(char * mask) {
	int __builtin_popcount(unsigned int x);
	char * buf = mask + sizeof(size_t);
	size_t size = *(size_t *) mask;
	size_t sum = 0;
	intptr_t i;
	for(i = 0; i < (size + 7)>> 3; i++) {
		sum += __builtin_popcount((unsigned char) buf[i]);
	}
	return sum;
}
