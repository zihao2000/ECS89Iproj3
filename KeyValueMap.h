#ifndef KEYVALUEMAP_H 	 	    		
#define KEYVALUEMAP_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct STRUCT_KEY_VALUE_MAP_TAG SKeyValueMap;
typedef SKeyValueMap * SKeyValueMapRef;
typedef const SKeyValueMap * SConstKeyValueMapRef;

typedef struct STRUCT_KEY_VALUE_MAP_ITERATOR_TAG SKeyValueMapIterator;
typedef SKeyValueMapIterator * SKeyValueMapIteratorRef;

SKeyValueMapRef KeyValueMapCreate();

bool KeyValueMapDestroy(SKeyValueMapRef kvmap);

size_t KeyValueMapSize(SConstKeyValueMapRef kvmap);

const char *KeyValueMapGet(SConstKeyValueMapRef kvmap, const char *key);

bool KeyValueMapSet(SKeyValueMapRef kvmap, const char *key, const char *value);

bool KeyValueMapDelete(SKeyValueMapRef kvmap, const char *key);

SKeyValueMapIteratorRef KeyValueMapBegin(SConstKeyValueMapRef kvmap);

SKeyValueMapIteratorRef KeyValueMapEnd(SConstKeyValueMapRef kvmap);

SKeyValueMapIteratorRef KeyValueMapFind(SConstKeyValueMapRef kvmap, const char *key);

SKeyValueMapIteratorRef KeyValueMapIteratorNext(SKeyValueMapIteratorRef iterref);

SKeyValueMapIteratorRef KeyValueMapIteratorPrevious(SKeyValueMapIteratorRef iterref);

const char *KeyValueMapIteratorKey(SKeyValueMapIteratorRef iterref);

const char *KeyValueMapIteratorValue(SKeyValueMapIteratorRef iterref);

bool KeyValueMapIteratorSet(SKeyValueMapIteratorRef iterref, const char *value);

SKeyValueMapIteratorRef KeyValueMapIteratorDelete(SKeyValueMapIteratorRef iterref);

#ifdef __cplusplus
}
#endif

#endif
