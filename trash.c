#include <stdio.h>
#include <math.h>
#include "vector.c"
const int N = 100;
typedef enum _kInd /* перечисление значений*/
{
	END = -3, // ячейка конец матрицы
	COMP, // значение в ячейке матрицы
	EMPTY // пустое значение
} kInd;

typedef struct _Cell
{
	Vector *v;
	int ind;
	int row;
	int col;
	_template data;
} Cell;

Cell cellFirst(Vector *v);
void cellNext(Cell *cell);
void printSourceMatrix(Vector *v, const int m, const int n);
void printInnerMatrix(const Vector *v);
void InputMatrix(Vector *v, FILE *in, int *m, int *n);
int main(void)
{
	int lastInd, cnt, maxCols[N];
	for (int i = 0; i < N; i++)
		maxCols[i] = 0;
	FILE *in = fopen("test", "r");
	if (in == NULL)
	{
		printf("Неудается открыть файл");
		return 1;
	}
	Vector v;
	int n, m;
	Cell cell;
	InputMatrix(&v, in, &m, &n);
	/* Задание */
	printf("Обычное представление:\n");
	printSourceMatrix(&v, m, n);
	printf("Внутреннее представление\n");
	printInnerMatrix(&v);
	_template tmp_template, max_template;
	cell = cellFirst(&v);
	/* Пока не конец матрицы */
	while (cell.row != END)
	{
		if (abs(cell.data) > max_template)
			max_template = abs(cell.data);
		cellNext(&cell);
	}
	printf("Максимальное вещественное число по модулю: %.2lf\n", max_template);
	if (max_template == 0.0)
	{
		printf("Делить на него нельзя, так как его модуль равен нулю\n");
		return 0;
	}

	lastInd = 0;
	cnt = 0; // кол-во столбцов с максимальным значением
	cell = cellFirst(&v);
	while (cell.row != END)
	{
		if (abs(cell.data) == max_template)
		{
			maxCols[cell.col] = 1; // столбец с максимальным значением 1
			lastInd = cell.col;
			cnt++; // кол-во столбцов с максимальным значением
		}
		cellNext(&cell);
	}
	VECTOR_TYPE tmpItem;
	/* предпослений столбец с максимальным значением */
	if (cnt > 1)
		for (int i = lastInd - 1; i >= 0; i--)
			if (maxCols[i]) 
			{
				lastInd = i;
				break;
			}

	cell = cellFirst(&v);
	while (cell.row != END)
	{
		/* если столбец равен "последнему" */
		if (cell.col == lastInd)
		{
			tmpItem = vectorLoad(&v, cell.ind + 1);
			tmpItem.c = cell.data/max_template;
			vectorSave(&v, cell.ind + 1, tmpItem);
		}
		cellNext(&cell);
	}
	printf("Обычное представление после преобразования:\n");
	printSourceMatrix(&v, m, n);
	printf("Внутреннее представление после преобразования:\n");
	printInnerMatrix(&v);

	vectorDestroy(&v);

	return 0;
}
     /* Ввод матрицы */
void InputMatrix(Vector *v, FILE *in, int *m, int *n)
{
	int isRowBegin;
	VECTOR_TYPE tmpItem;
	fscanf(in, "%d", m);
	fscanf(in, "%d", n);
	if (*m < 1 || *m > N || *n < 1 || *n > N)
	{
		printf("Количество строк должно быть в диапозоне от 1 до %d\n", N);
		exit(EXIT_FAILURE);
	}
	vectorCreate(v, 1);
	tmpItem.ind = EMPTY; // пустой изначально
	_template tmp_template;
	vectorPushBack(v, tmpItem);
	for (int i = 0; i < *m; i++)
	{
		isRowBegin = 0;
		for (int j = 0; j < *n; j++)
		{
			fscanf(in, "%lf", &tmp_template);
			if (tmp_template == 0.0)
				continue;

			if (!isRowBegin) // если это начало строки
			{
				isRowBegin = 1;
				tmpItem.ind = i;
				vectorPushBack(v, tmpItem); // добавляем номер строки в структуру
			}

			tmpItem.ind = j; // добавляем номер столбца в структуру
			vectorPushBack(v, tmpItem);

			tmpItem.c = tmp_template; // добавляем элемент i j 
			tmpItem.ind = COMP;
			vectorPushBack(v, tmpItem);
		}

		if (isRowBegin) // если конец строки
		{
			tmpItem.ind = EMPTY; //добавляем нулевую строку
			vectorPushBack(v, tmpItem);
		}
	}
	// добавляем признак конца матрицы
	tmpItem.ind = END;
	vectorPushBack(v, tmpItem);
	return;
}

Cell cellFirst(Vector *v)
{
	Cell res;
	res.v = v;
	res.ind = 2;
	res.row = END;
	res.col = EMPTY;
	res.data = 0.0;
	/* ЕСЛИ ЭТО НЕ КОНЕЦ СТРУКТУРЫ, ТО ЗАПИСЫВАЕМ КООРДИНАТЫ
	ROW - I COL - J DATA - C потому что начало*/
	if (vectorLoad(v, res.ind - 1).ind != END)
	{
		res.row = vectorLoad(v, res.ind - 1).ind;
		res.col = vectorLoad(v, res.ind).ind;
		res.data = vectorLoad(v, res.ind + 1).c;
	}

	return res;
}

void cellNext(Cell *cell)
{
	int c1, c2;

	if (cell->row == END)
		return;

	cell->ind += 2;
	c1 = vectorLoad(cell->v, cell->ind).ind;
	c2 = vectorLoad(cell->v, cell->ind + 1).ind;

	if (c1 > EMPTY && c2 == COMP)
	{
		cell->col = vectorLoad(cell->v, cell->ind).ind;
		cell->data = vectorLoad(cell->v, cell->ind + 1).c;
	}
	else if (c1 == EMPTY && c2 > EMPTY) // если конец строки, то перепрыгиваем элементы
	{
		cell->row = vectorLoad(cell->v, cell->ind + 1).ind;
		cell->col = vectorLoad(cell->v, cell->ind + 2).ind;
		cell->data = vectorLoad(cell->v, cell->ind + 3).c;
		cell->ind += 2;
	}
	else
	{
		cell->row = END;
		cell->col = EMPTY;
	}
}

void printSourceMatrix(Vector *v, const int m, const int n)
{
	int i, j;
	Cell cell = cellFirst(v);
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (i == cell.row && j == cell.col)
			{
				printf("%.2lf ", cell.data);
				cellNext(&cell);
			}
			else
				printf("%.2lf ", 0.0);
		}

		printf("\n");
	}
}

void printInnerMatrix(const Vector *v)
{
	int i;
	VECTOR_TYPE item;
	for (i = 0; i < vectorSize(v); i++)
	{
		item = vectorLoad(v, i);

		if (item.ind == COMP)
			printf("%.2lf ", item.c);
		else
			printf("%d ", item.ind);
	}
	printf("\n");
}
