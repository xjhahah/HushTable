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
void HBCheckCapacity(HashBucket* hb)
{
	assert(hb);
	HashBucket newHT;
	if (hb->_size == hb->len)
	{
		newHT.len = GetNextPrime(hb->len);
		HBInit(&newHT, newHT.len);
	}
	for (size_t i = 0; i < hb->len; ++i)
	{
		HashNode* pCur = hb->_tables[i];
		while(pCur)
		{
			HashNode* next = pCur->_next;
			size_t index = HBFunc(pCur->_key, newHT.len);
			pCur->_next = newHT._tables[index];
			newHT._tables[index] = pCur;
			pCur = next;
		}
		hb->_tables[i] = NULL;
	}
	HBDestroy(hb);
	hb->_tables = newHT._tables;
	hb->_size = newHT._size;
	hb->len = newHT.len;
}
int HBInsert(HashBucket* hb, HTBKeyType key, HTBValueType val)
{
	HashNode* cur,*newNode;
	size_t index;

	HBCheckCapacity(hb);
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

int HBRemove(HashBucket* hb, HTBKeyType key)
{
	size_t index;
	HashNode* cur=NULL;
	HashNode* prev=NULL;
	assert(hb);
	index = HBFunc(key, hb->len);

	cur = hb->_tables[index];
	while (cur)
	{
		if (cur->_key == key)
		{
			if (prev == NULL)
			{
				hb->_tables[index] = cur->_next;
			}
			else
			{
				prev->_next = cur->_next;
			}
			free(cur);
			--hb->_size;
			return 0;
		}
		else
		{
			prev = cur;
			cur = cur->_next;
		}
	}
	return -1;
}

HashNode* HBFind(HashBucket* hb, HTBKeyType key)
{
	HashNode* cur;
	assert(hb);
	//先算位置
	size_t index = HBFunc(key, hb->len);
	cur = hb->_tables[index];
	while (cur)
	{
		if (cur->_key == key)
		{
			printf("找到了！！！\n");
			return cur;
		}
		else
		{
			cur = cur->_next;
		}
	}
	return NULL;
}

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

void PrintHashBucket(HashBucket* hb)
{
	assert(hb);
	for (size_t i = 0; i < hb->len; ++i)
	{
		HashNode* cur = hb->_tables[i];
		printf("table[%d]-->", i);

		while (cur)
		{
			printf("[%d:%d]-->", cur->_key, cur->_value);
			cur = cur->_next;
		}
		printf("NULL\n");
	}
	printf("\n");
}