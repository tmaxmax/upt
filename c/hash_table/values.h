#ifndef HASH_TABLE_VALUES_H
#define HASH_TABLE_VALUES_H

// Interprets the void* returned by any of the hash table's methods
// as a float. NULL values are interpreted as 0.0.
float ht_v_float(void *from_ht);

// Bitwise cast from the given float to a void*, which should
// be used as a hash table value (for insertion etc.).
void *ht_v_from_float(float f);

#endif
