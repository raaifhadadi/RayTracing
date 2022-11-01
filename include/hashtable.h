#ifndef __HASHTABLE_H_
#define __HASHTABLE_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct Hashnode {
  struct Hashnode *next;
  struct Hashnode *last;
  void *data;
  char *key;
} hashnode;

struct Hashtable;

typedef void (*hashAdd)(struct Hashtable *, char *, void *, size_t);

typedef void *(*hashQuery)(struct Hashtable *, char *);

typedef void (*hashFree)(struct Hashtable *);

typedef uint32_t (*hashFunc)(char *);

typedef struct Hashtable {
  hashnode **data;
  float load_factor;
  uint32_t num_buckets;
  uint32_t num_elements;
  hashAdd add;
  hashQuery get;
  hashFree free;
  hashFunc hash;
} hashtable_t;

hashtable_t *createHashtable(float loadFactor, uint32_t initialSize,
                             hashFunc hash);

uint32_t djb2(char *str);

#endif  // __HASHTABLE_H_
