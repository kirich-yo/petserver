#ifndef _PET_HASHTABLE_H_
#define _PET_HASHTABLE_H_

#include <stddef.h>

typedef struct _PetHashTable {
	void ** values;
	size_t size;
} PetHashTable;

PetHashTable * HashTable_New();

void * HashTable_GetValue(PetHashTable * hashtable, const char * key);

void HashTable_SetValue(PetHashTable * hashtable, const char * key, void * value);

unsigned long _HashTable_HashFunction(PetHashTable * hashtable, const char * key);

void HashTable_Free(PetHashTable * hashtable);

/* TODO: Implement the hash table*/

#endif /* _PET_HASHTABLE_H_ */
