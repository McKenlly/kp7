#include "vector.h"

void vectorCreate(Vector *v, const int size)
{
	if (size > 0)
	{
		v->_data = (VECTOR_TYPE *)malloc(sizeof(VECTOR_TYPE) * size);
		v->_capacity = size;
	}
	else
	{
		v->_data = (VECTOR_TYPE *)malloc(sizeof(VECTOR_TYPE));
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

VECTOR_TYPE vectorLoad(const Vector *v, const int index)
{
	return v->_data[index];
}

void vectorSave(Vector *v, const int index, const VECTOR_TYPE value)
{
	v->_data[index] = value;
}

int vectorPushBack(Vector *v, const VECTOR_TYPE value)
{
	VECTOR_TYPE *ptr = NULL;

	if (v->_size == v->_capacity)
	{
		ptr = (VECTOR_TYPE *)realloc(v->_data, sizeof(VECTOR_TYPE) * v->_capacity*2);

		if (ptr != NULL)
		{
			v->_data = ptr;
			v->_capacity *= 2;
		}
		else
			return 0;
	}

	v->_data[v->_size++] = value;

	return 1;
}

void vectorDestroy(Vector *v)
{
	if (v->_data != NULL)
	{
		free(v->_data);

		v->_data = NULL;
	}

	v->_size = 0;
	v->_capacity = 0;
}