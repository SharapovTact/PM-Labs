#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

constexpr double EPS = 0.000001;

typedef struct
{
    int numRows;
    int numColumns;
    double *items;
} Matrix;

int CreateMatrix(const int numRows, const int numColumns, Matrix* m)
{
    if (numRows <= 0 || numColumns <= 0)
    {
        return EBADMSG;
    }
    m->numRows = numRows;
    m->numColumns = numColumns;
    m->items = malloc(numRows * numColumns * sizeof(double));
    if (m->items == NULL)
    {
        return ENOMEM;
    }
    for (int i = 0; i < m->numRows; i++)
    {
        for (int j = 0; j < m->numColumns; j++)
        {
            *(m->items + (i * m->numColumns + j)) = 0;
        }
    }
    return 0;
}

void SetMatrixItemValue(const Matrix* m, const int row, const int column, const double value)
{
    if (row < 0 || column < 0 || row >= m->numRows || column >= m->numColumns)
    {
        abort();
    }
    *(m->items + (row * m->numColumns + column)) = value;
}

double GetMatrixItemValue(const Matrix* m, const int row, const int column)
{
    if (row < 0 || column < 0 || row >= m->numRows || column >= m->numColumns)
    {
        abort();
    }
    double item = *(m->items + (row * m->numColumns + column));
    return item;
}

Matrix CreateIdentyMatrix(const int size)
{
    Matrix identyMatrix;
    CreateMatrix(size, size, &identyMatrix);
    for (int i = 0; i < size; i++)
    {
        SetMatrixItemValue(&identyMatrix, i, i, 1);
    }
}

bool isEqualMetrices(Matrix* matrix1, Matrix* matrix2)
{
    if (matrix1->numRows == matrix2->numRows && matrix1->numColumns == matrix2->numColumns)
    {
        for (int i = 0; i < matrix1->numRows; i++)
        {
            for (int j = 0; j < matrix2->numColumns; j++)
            {
                if (fabs(GetMatrixItemValue(matrix1, i, j) - GetMatrixItemValue(matrix2, i, j)) > EPS)
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

Matrix InputMatrix(int *status)
{
    Matrix matrix;
    int numRows;
    int numColumns;
    printf("Введите количество столбцов и строчек: ");
    if (scanf("%d %d", &numRows, &numColumns) == -1)
    {
        goto inputError;
    }
    if (CreateMatrix(numRows, numColumns, &matrix) != 0)
    {
        goto inputError;
    }
    printf("введите поочерёдно строчки матрицы: \n");
    for (int i = 0; i < matrix.numRows; i++)
    {
        for (int j = 0; j < matrix.numColumns; j++)
        {
            double value;
            if (scanf("%lf", &value) == -1)
            {
                goto inputError;
            }
            SetMatrixItemValue(&matrix, i, j, value);
        }
    }
    return matrix;
inputError:
    *status = EBADMSG;
    return matrix;
}

void DestroyMatrix(Matrix* matrix)
{
    matrix->numRows = 0;
    matrix->numColumns = 0;
    if (matrix->items != NULL)
    {
        free(matrix->items);
    }
}

void PrintMatrix(const Matrix* matrix)
{
    for (int i = 0; i < matrix->numRows; i++)
    {
        for (int j = 0; j < matrix->numColumns; j++)
        {
            printf("%lf ", *(matrix->items + (i * matrix->numColumns + j)));
        }
        puts("");
    }
}

void CopyMatrix(const Matrix* matrixCopy, const Matrix* matrixTo)
{
    DestroyMatrix(matrixTo);
    CreateMatrix(matrixCopy->numRows, matrixCopy->numColumns, matrixTo);
    for (int i = 0; i < matrixCopy->numRows; i++)
    {
        for (int j = 0; j < matrixCopy->numColumns; j++)
        {
            SetMatrixItemValue(matrixTo, i, j, GetMatrixItemValue(matrixCopy, i, j));
        }
    }
}

Matrix MatrixMultiplication(const Matrix* matrix1, const Matrix* matrix2, int* status)
{
    *status = 0;
    Matrix matrixRes;
    CreateMatrix(matrix1->numRows, matrix2->numColumns, &matrixRes);
    if (matrix1->numColumns == matrix2->numRows)
    {
        for (int i = 0; i < matrix1->numRows; i++)
        {
            for (int j = 0; j < matrix2->numColumns; j++)
            {
                double item = 0;
                for (int k = 0; k < matrix1->numColumns; k++)
                {
                    item += GetMatrixItemValue(matrix1, i, k) * GetMatrixItemValue(matrix2, k,  j);
                }
                SetMatrixItemValue(&matrixRes, i, j, item);
            }
        }
    }
    *status = 1;
    return matrixRes;
}



int main(void)
{
    int status = 0;
    Matrix matrix1 = InputMatrix(&status);
    Matrix matrix2 = InputMatrix(&status);
    if (status == EBADMSG)
    {
        goto inputError;
    }

    Matrix matrixRes = MatrixMultiplication(&matrix1, &matrix2, &status);
    PrintMatrix(&matrixRes);

    DestroyMatrix(&matrixRes);
    DestroyMatrix(&matrix1);
    DestroyMatrix(&matrix2);
    return 0;
inputError:
    puts("Ошибка ввода");
    DestroyMatrix(&matrix1);
    DestroyMatrix(&matrix2);
    return EBADMSG;
}