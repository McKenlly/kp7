#include "vector.h"

void vectorCreate(Vector *v, const int size)
{
	if (size > 0)
	{
		v->data = (Item *)malloc(sizeof(Item) * (size));
		v->_capacity = size;
	}
	else
	{
		v->data = (Item *)malloc(sizeof(Item));
		v->_capacity = 1;
	}
	v->_size = 0;
}

int vectorSize(const Vector *v)
{
	return v->_size;
}

int vectorCapacity(const Vector *v)
{
	return v->_capacity;
}

Item vectorLoad(const Vector *v, const int t)
{
	int ind = 0;
	while (v->data[ind].index != -1)
	{
		if (v->data[ind].index == t)
		return v->data[ind];
		ind++;
	}
	return v->data[ind];
}

void vectorSave(Vector *v, const int index, const Item value)
{
	v->data[index] = value;
}

int vectorPushBack(Vector *v, const Item value)
{
	Item *ptr = NULL;

	if (v->_size == v->_capacity)
	{
		ptr = (Item *)realloc(v->data, sizeof(Item) * v->_capacity*2);

		if (ptr != NULL)
		{
			v->data = ptr;
			v->_capacity *= 2;
		}
		else
			return 0;
	}

	v->data[v->_size++] = value;

	return 1;
}

void vectorDestroy(Vector *v)
{
	if (v->data != NULL)
	{
		free(v->data);

		v->data = NULL;
	}

	v->_size = 0;
	v->_capacity = 0;
}