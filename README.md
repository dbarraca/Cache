# Cache

### Cache struct: int hits, int misses, int reads, int writes, LookupTable table

### LookupTable struct: int size, int table[][associativity], int associativity

* Use write-through for writes
* #include <stdint.h> to use int64_t for 64 bit ints
* Store address in upper 32 bits, data in lower 32 bits of each int64_t
