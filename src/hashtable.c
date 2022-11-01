#include "../include/hashtable.h"

#include <string.h>

hashnode *createHashnode(char *key, void *data, size_t size) {
  hashnode *node = malloc(sizeof(hashnode));
  node->key = malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(node->key, key);
  node->data = malloc(size);
  memcpy(node->data, data, size);
  node->next = NULL;
  return node;
}

void freeHashnode(hashnode *node) {
  if (!node) return;
  hashnode *next = node->next;
  free(node->key);
  free(node->data);
  free(node);
  freeHashnode(next);
}

void *getHashnode(hashnode *node, char *key) {
  if (!node) return NULL;
  if (!strcmp(key, node->key)) {
    return node->data;
  }
  return getHashnode(node->next, key);
}

uint32_t djb2(char *str) {
  uint32_t hash = 5381;
  for (int i = 0; i < strlen(str); ++i) {
    hash = hash * 33 ^ str[i];
  }
  return hash;
}

void insertNode(hashtable_t *map, hashnode *node) {
  uint32_t hashValue = map->hash(node->key) % map->num_buckets;
  if (map->data[hashValue]) {
    map->data[hashValue]->last->next = node;
  } else {
    map->data[hashValue] = node;
  }
  map->data[hashValue]->last = node;
}

void rehash(hashtable_t *map) {
  uint32_t oldBuckets = map->num_buckets;
  hashnode **oldData = map->data;
  map->num_buckets *= 2;
  map->data = calloc(map->num_buckets, sizeof(hashnode *));
  for (int i = 0; i < oldBuckets; ++i) {
    hashnode *node = oldData[i];
    while (node) {
      hashnode *next = node->next;
      node->next = NULL;
      insertNode(map, node);
      node = next;
    }
  }
  free(oldData);
}

void insert(hashtable_t *map, char *key, void *data, size_t size) {
  hashnode *node = createHashnode(key, data, size);
  insertNode(map, node);
  ++map->num_elements;
  if ((float)map->num_elements / map->num_buckets > map->load_factor) {
    rehash(map);
  }
}

void *get(hashtable_t *map, char *key) {
  uint32_t hashValue = map->hash(key) % map->num_buckets;
  return getHashnode(map->data[hashValue], key);
}

void freeHashtable(hashtable_t *map) {
  if (!map) return;
  for (int i = 0; i < map->num_buckets; ++i) {
    freeHashnode(map->data[i]);
  }
  free(map->data);
  free(map);
}

hashtable_t *createHashtable(float loadFactor, uint32_t initialSize,
                             hashFunc hash) {
  hashtable_t *map = malloc(sizeof(hashtable_t));
  map->data = calloc(initialSize, sizeof(hashnode *));
  map->load_factor = loadFactor;
  map->num_buckets = initialSize;
  map->num_elements = 0;
  map->add = &insert;
  map->get = &get;
  map->free = &freeHashtable;
  map->hash = hash;
  return map;
}
