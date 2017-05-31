#include <stdio.h>
#include <math.h>
#include "vector.c"
const int N = 100;
typedef enum _kInd /* перечисление значений*/
{
	END = -1, // ячейка конец матрицы
} kInd;

void multiMatrix1(Vector *v1, Vector *v2, Vector *v3, int m, int q, int n);
void printSourceMatrix(Vector *v, const int m, const int n);
void printInnerMatrix(const Vector *v);
void InputMatrix(Vector *v, FILE *in, int *m, int *n);

int main(void)
{

	FILE *in = fopen("test", "r");
	if (in == NULL)
	{
		printf("Неудается открыть файл");
		return 1;
	}
	Vector v1, v2;
	int n1, m1, n2, m2;
	InputMatrix(&v1, in, &m1, &n1);
	InputMatrix(&v2, in, &m2, &n2);
	/* Задание */
	printf("Обычное представление:\n");
	printSourceMatrix(&v1, m1, n1);
	printf("Внутреннее представление\n");
	printInnerMatrix(&v1);
	printf("Обычное представление:\n");
	printSourceMatrix(&v2, m2, n2);
	printf("Внутреннее представление\n");
	printInnerMatrix(&v2);
	Vector v3;
	if (n1 == m2)
		multiMatrix1(&v1, &v2, &v3, m1, n2, n1);
	else
		printf("ERROR\n");
	printf("Внутреннее представление\n");
	printInnerMatrix(&v3);
	printf("Обычное представление:\n");
	printSourceMatrix(&v3, m1, n2);
	return 0;
}
     /* Ввод матрицы */
void InputMatrix(Vector *v, FILE *in, int *m, int *n)
{
	Item tmpItem;
	fscanf(in, "%d", m);
	fscanf(in, "%d", n);
	if (*m < 1 || *m > N || *n < 1 || *n > N)
	{
		printf("Количество строк должно быть в диапозоне от 1 до %d\n", N);
		exit(EXIT_FAILURE);
	}
	vectorCreate(v, 1);
	tmpItem.index = END; // пустой изначально
	double tmp_template;
	int ind = 0;
	for (int i = 1; i <= *m; i++)
	{
		for (int j = 1; j <= *n; j++)
		{
			fscanf(in, "%lf", &tmp_template);
			if (tmp_template == 0.0)
				continue;
			tmpItem.index = (i-1)*(*n) + j - 1;
			tmpItem.value = tmp_template;
			vectorPushBack(v, tmpItem); 
			ind++;
		}
	}
	// добавляем признак конца матрицы
	tmpItem.index = END;
	vectorPushBack(v, tmpItem);
	return;
}

void printSourceMatrix(Vector *v, const int m, const int n)
{
	int i, j;
	for (i = 1; i <= m; i++)
	{
		for (j = 1; j <= n; j++)
		{
			int ind = (i-1)*n+j-1;
			Item tmp = vectorLoad(v, ind);
			if (tmp.index == -1)
				printf("%.2lf ", 0.0);
			else
				printf("%.2lf ", tmp.value);
		}
		printf("\n");
	}
}
void multiMatrix1(Vector *v1, Vector *v2, Vector *v3, int m, int q, int n)
{
	vectorCreate(v3, 1);
	Item tmp;
	for(int i = 1; i <= m; i++)
    for(int j = 1; j <= q; j++)
    {
		int ind_i_j = (i-1)*m+j-1;
		tmp.index = ind_i_j;
		tmp.value = 0;
        for(int k = 1; k <= n; k++)
            {
				int ind_i_k = (i-1)*m+k-1;
				int ind_k_j = (k-1)*q+j-1;
				Item A = vectorLoad(v1, ind_i_k);
				Item B = vectorLoad(v2, ind_k_j);
				if (A.index == END || B.index == END)
				continue;
				tmp.value += A.value*B.value;
			}
		if (tmp.value != 0.0)
		vectorPushBack(v3, tmp);
    }
	tmp.index = END;
	vectorPushBack(v3, tmp);
	return ;
}
void printInnerMatrix(const Vector *v)
{
	int i;
	for (i = 0; i < vectorSize(v); i++)
	{
		printf("%d ", v->data[i].index);
	}
	printf("\n");
	for (i = 0; i < vectorSize(v)-1; i++)
	{
		printf("%.2lf ", v->data[i].value);
	}
	printf("\n");
	return;
}
