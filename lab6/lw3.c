#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

constexpr double EPS = 0.000001;

typedef struct
{
    int numRows;
    int numColumns;
    double *items;
} Matrix;

void DestroyMatrix(Matrix* matrix)
{
    matrix->numRows = 0;
    matrix->numColumns = 0;
    if (matrix->items != NULL)
    {
        free(matrix->items);
    }
}

int CreateMatrix(const int numRows, const int numColumns, Matrix* m) //TODO Вот это лучший вариант, возвращать ошибку, и принимать указатель на матрицу
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
        DestroyMatrix(m);
        abort();
    }
    *(m->items + (row * m->numColumns + column)) = value;
}

double GetMatrixItemValue(const Matrix* m, const int row, const int column)
{
    if (row < 0 || column < 0 || row >= m->numRows || column >= m->numColumns)
    {
        DestroyMatrix(m);
        abort();
    }
    return *(m->items + (row * m->numColumns + column));
}

int CreateIdentyMatrix(const int size, Matrix* identyMatrix)
{
    CreateMatrix(size, size, identyMatrix);
    for (int i = 0; i < size; i++)
    {
        SetMatrixItemValue(identyMatrix, i, i, 1);
    }
    return 0;
}

bool isEqualMatrices(const Matrix* matrix1, const Matrix* matrix2)
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

int InputMatrix(Matrix* matrix)
{
    int numRows;
    int numColumns;
    printf("Введите количество столбцов и строчек: ");
    if (scanf("%d %d", &numRows, &numColumns) == -1)
    {
        return EBADMSG;
    }
    if (CreateMatrix(numRows, numColumns, matrix) != 0)
    {
        return EBADMSG;
    }
    printf("введите поочерёдно строчки матрицы: \n");
    for (int i = 0; i < matrix->numRows; i++)
    {
        for (int j = 0; j < matrix->numColumns; j++)
        {
            double value;
            if (scanf("%lf", &value) == -1)
            {
                return EBADMSG;
            }
            SetMatrixItemValue(matrix, i, j, value);
        }
    }
    return 0;
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
    CreateMatrix(matrixCopy->numRows, matrixCopy->numColumns, matrixTo);
    for (int i = 0; i < matrixCopy->numRows; i++)
    {
        for (int j = 0; j < matrixCopy->numColumns; j++)
        {
            SetMatrixItemValue(matrixTo, i, j, GetMatrixItemValue(matrixCopy, i, j));
        }
    }
}

int MatrixMultiplication(const Matrix* matrix1, const Matrix* matrix2, Matrix* matrixRes)
{
    CreateMatrix(matrix1->numRows, matrix2->numColumns, matrixRes);
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
                SetMatrixItemValue(matrixRes, i, j, item);
            }
        }
        return 0;
    }
    return 1;
}

double FindDet(const Matrix* matrix)
{
    Matrix m;
    CopyMatrix(matrix, &m);
    for (int factorRowId = 0; factorRowId < m.numRows - 1; factorRowId++)
    {
        for (int operandRowIndex = factorRowId + 1; operandRowIndex < m.numRows; operandRowIndex++)
        {
            const double operandElement = GetMatrixItemValue(&m, operandRowIndex, factorRowId);
            const double factorElement = GetMatrixItemValue(&m, factorRowId, factorRowId);
            const double modifier = operandElement / factorElement;
            for (int elIndex = 0; elIndex < m.numColumns; elIndex++)
            {
                const double decrElMatrix = GetMatrixItemValue(&m, factorRowId, elIndex);
                const double operandElMatrix = GetMatrixItemValue(&m, operandRowIndex, elIndex);
                SetMatrixItemValue(&m, operandRowIndex, elIndex, operandElMatrix - decrElMatrix * modifier);
            }
        }
    }
    double det = 1;
    for (int i = 0; i < matrix->numRows; i++)
    {
        det *= GetMatrixItemValue(&m, i, i);
    }
    return det;
}

void CountBottomTriangle(const Matrix* workMatrix, const Matrix* matrixRes)
{
    const int numRows = workMatrix->numRows;
    for (int factorRowId = 0; factorRowId < numRows; factorRowId++)
    {
        const double diag = GetMatrixItemValue(workMatrix, factorRowId, factorRowId);
        for (int operandRowIndex = factorRowId + 1; operandRowIndex < numRows; operandRowIndex++)
        {
            const double factor = GetMatrixItemValue(workMatrix, operandRowIndex, factorRowId);
            const double modifier = factor / diag;
            for (int elIndex = 0; elIndex < numRows; elIndex++)
            {
                const double workVal = GetMatrixItemValue(workMatrix, operandRowIndex, elIndex);
                const double workFactor = GetMatrixItemValue(workMatrix, factorRowId, elIndex);
                SetMatrixItemValue(workMatrix, operandRowIndex, elIndex, workVal - workFactor * modifier);

                const double resVal = GetMatrixItemValue(matrixRes, operandRowIndex, elIndex);
                const double resFactor = GetMatrixItemValue(matrixRes, factorRowId, elIndex);
                SetMatrixItemValue(matrixRes, operandRowIndex, elIndex, resVal - resFactor * modifier);
            }
        }
    }
}

void CountTopTriangle(const Matrix* workMatrix, const Matrix* matrixRes)
{
    const int numRows = workMatrix->numRows;
    for (int factorRowId = numRows - 1; factorRowId > 0; factorRowId--)
    {
        const double diag = GetMatrixItemValue(workMatrix, factorRowId, factorRowId);
        for (int operandRowIndex = factorRowId - 1; operandRowIndex >= 0; operandRowIndex--)
        {
            const double factor = GetMatrixItemValue(workMatrix, operandRowIndex, factorRowId);
            const double modifier = factor / diag;
            for (int elIndex = 0; elIndex < numRows; elIndex++)
            {
                const double workVal = GetMatrixItemValue(workMatrix, operandRowIndex, elIndex);
                const double workFactor = GetMatrixItemValue(workMatrix, factorRowId, elIndex);
                SetMatrixItemValue(workMatrix, operandRowIndex, elIndex, workVal - workFactor * modifier);

                const double resVal = GetMatrixItemValue(matrixRes, operandRowIndex, elIndex);
                const double resFactor = GetMatrixItemValue(matrixRes, factorRowId, elIndex);
                SetMatrixItemValue(matrixRes, operandRowIndex, elIndex, resVal - resFactor * modifier);
            }
        }
    }

}

void MatrixNormalization(const Matrix* workMatrix, const Matrix* matrixRes)
{
    const int numRows = workMatrix->numRows;
    for (int row = 0; row < numRows; row++)
    {
        const double diag = GetMatrixItemValue(workMatrix, row, row);
        for (int col = 0; col < numRows; col++)
        {
            const double resVal = GetMatrixItemValue(matrixRes, row, col);
            SetMatrixItemValue(matrixRes, row, col, resVal / diag);
        }
    }
}

int InvMatrix(const Matrix* matrix1, Matrix* matrixRes)
{
    const double det = FindDet(matrix1);
    if (fabs(det) <= EPS)
    {
        printf("%lf", det);
        puts("matrix is singular");
        return 1;
    }
    CreateIdentyMatrix(matrix1->numRows, matrixRes);
    Matrix workMatrix;
    CopyMatrix(matrix1, &workMatrix);

    CountBottomTriangle(&workMatrix, matrixRes);
    CountTopTriangle(&workMatrix, matrixRes);

    MatrixNormalization(&workMatrix, matrixRes);

    DestroyMatrix(&workMatrix);
    return 0;
}

int main(void)
{
    Matrix matrix1;
    if (InputMatrix(&matrix1) == EBADMSG)
    {
        goto inputError;
    }
    Matrix matrixRes;
    InvMatrix(&matrix1, &matrixRes);
    PrintMatrix(&matrixRes);

    DestroyMatrix(&matrixRes);
    DestroyMatrix(&matrix1);
    return 0;
inputError:
    puts("Ошибка ввода");
    DestroyMatrix(&matrixRes);
    DestroyMatrix(&matrix1);
    return EBADMSG;
}