#include "Hush.h"

void HTInit(HashTable* ht, size_t len)
{
	assert(len > 0);
	assert(ht);

	len = GetNextPrime(len);
	ht->_tables = (HashData*)malloc(sizeof(HashData)*len);
	memset(ht->_tables, 0, sizeof(HashData)*len);
	ht->_size = 0;
	ht->_len = len;

	for (size_t i = 0; i < len; ++i)
	{
		ht->_tables[i]._state = EMPTY;
	}
}

void HTDestroy(HashTable* ht)
{
	assert(ht);
	free(ht->_tables);
	ht->_tables = NULL;
	ht->_size = ht->_len = 0;
}

//定制哈希函数
size_t HTHashFunc(HTKeyType key, size_t len)
{
	//return key % len;
	return BKDRHash(key) % len;
}

void CheckCapacity(HashTable* ht)
{
	if (ht->_size * 10 / ht->_len > 7)
	{
		HashTable newht;
		size_t i;
		HTInit(&newht, GetNextPrime(ht->_len));
		for (i = 0; i < ht->_len; ++i)
		{
			if (ht->_tables[i]._state == EXIST)
			{
				HTInsert(&newht, ht->_tables[i]._key, ht->_tables[i]._value);
			}
		}

		HTDestroy(ht);
		ht->_tables = newht._tables;
		ht->_size = newht._size;
		ht->_len = newht._len;
	}
}


int HTInsert(HashTable* ht, HTKeyType key, HTValueType value)
{
	// 增容
	int i = 0;
	size_t index;
	size_t start;
	CheckCapacity(ht);

	start = HTHashFunc(key, ht->_len);
	index = start;
	while (ht->_tables[index]._state == EXIST)
	{
		if (ht->_tables[index]._key == key)
		{
			return 0;
		}
		else
		{
			//线性探测
			/*	
			++index;
			if (index == ht->_len)
			{
				index = 0;
			}
			*/
			//发生哈希冲突，采用二次探测，如果数据很多的话用 多阶哈希
			++i;
			index = start + i * i;
			index %= ht->_len;

		}
	}

	ht->_tables[index]._state = EXIST;
	ht->_tables[index]._key = key;
	ht->_tables[index]._value = value;
	ht->_size++;

	return 1;
}

int HTRemove(HashTable* ht, HTKeyType key)
{
	HashData* data;
	assert(ht);
	data = HTFind(ht, key);
	if (data)
	{
		data->_state = DELETE;
		ht->_size--;
		return 1;
	}
	else
	{
		return 0;
	}
}

HashData* HTFind(HashTable* ht, HTKeyType key)
{
	size_t index;
	assert(ht);
	index = HTHashFunc(key, ht->_len);
	while (ht->_tables[index]._state != EMPTY)
	{
		if (ht->_tables[index]._key == key)
		{
			if (ht->_tables[index]._state == EXIST)
			{
				return &(ht->_tables[index]);
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			++index;
			if (index == ht->_len)
			{
				index = 0;
			}
		}
	}

	return NULL;
}

int HTSize(HashTable* ht)
{
	assert(ht);
	return ht->_size;
}

int HTEmpty(HashTable* ht)
{
	assert(ht);
	return ht->_size == 0;
}

void HTPrint(HashTable* ht)
{
	char* strState[3] = { "EMPTY", "EXIST", "DELETE" };
	assert(ht);
	for (size_t i = 0; i < ht->_len; ++i)
	{
		printf("%s->table[%d]%s:%d\n", strState[ht->_tables[i]._state], i,
			ht->_tables[i]._key, ht->_tables[i]._value);
	}
	printf("\n");
}

