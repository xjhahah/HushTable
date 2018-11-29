#include "HashBucket.h"

void HBInit(HashBucket* hb, size_t len)
{
	assert(hb);
	hb->len = GetNextPrime(len);
	hb->_size = 0;
	hb->_tables = (HashNode**)malloc(sizeof(HashNode*)*hb->len);
	memset(hb->_tables, 0, sizeof(HashNode*)*hb->len);

}
//定制哈希函数
size_t HBFunc(HTBKeyType key, size_t len)
{
	return key % len;
	//return BKDRHash(key) % len;
}

HashNode* BuyHashNode(HTBKeyType key,HTBValueType val)
{
	HashNode* pNewNode = malloc(sizeof(HashNode));
	pNewNode->_key = key;
	pNewNode->_value = val;
	pNewNode->_next = NULL;
	return pNewNode;
}

int HBInsert(HashBucket* hb, HTBKeyType key, HTBValueType val)
{
	HashNode* cur,*newNode;
	size_t index;

	assert(hb);

	//1.key是否已经存在
	index = HBFunc(key, hb->len);
	cur = hb->_tables[index];
	while (cur)
	{
		if (cur->_key == key)
		{
			return -1;
		}
		else
		{
			cur = cur->_next;
		}
	}

	//2.插入数据，头插
	newNode = BuyHashNode(key,val);
	newNode->_next = hb->_tables[index];
	hb->_tables[index] = newNode;
	hb->_size++;
	return 0;
}

//int HBRemove(HashBucket* hb, HTBKeyType key)
//{
//
//}
//
//HashNode* HBFind(HashBucket* hb, HTBKeyType key)
//{
//
//}

int HBSize(HashBucket* hb)
{
	assert(hb);
	return hb->_size;
}

int HBEmpty(HashBucket* hb)
{
	assert(hb);
	return 0 == hb->_size;
}

void HBDestroy(HashBucket*hb)
{
	assert(hb);

	//哈希桶每个节点下面挂的都是一个个指针，所以释放时不能直接释放 否则会存在内存泄漏问题
	for (size_t i = 0; i < hb->len; ++i)
	{
		HashNode* pCur = hb->_tables[i];
		while (pCur)
		{
			HashNode* pNext = pCur->_next;
			free(pCur);
			pCur = pNext;
		}
		hb->_tables[i] = NULL;
	}
	free(hb->_tables);
	hb->_tables = NULL;
	hb->_size = hb->len = 0;
}