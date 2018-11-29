#pragma once

#include "common.h"

typedef int HTBKeyType;
typedef int HTBValueType;

typedef struct HashNode
{
	struct HashNode* _next;
	HTBKeyType _key;
	HTBValueType _value;
}HashNode;


typedef  struct HashBucket
{
	HashNode**  _tables;
	size_t _size;
	size_t len;
}HashBucket;


void HBInit(HashBucket* hb, size_t len);
int HBInsert(HashBucket* hb,HTBKeyType key,HTBValueType val);
int HBRemove(HashBucket* hb, HTBKeyType key);
HashNode* HBFind(HashBucket* hb, HTBKeyType key);
int HBSize(HashBucket* hb);
int HBEmpty(HashBucket* hb);
void HBDestroy(HashBucket*hb);

void PrintHashBucket(HashBucket* hb);