#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdint.h>
#define VectorSize 1<<20

extern void ASMVersionStencil(int64_t* X, int64_t* Y, int max);
extern void SIMDXVersionStencil(int64_t* X, int64_t* Y, int max);
extern void SIMDYVersionStencil(int64_t* X, int64_t* Y, int max);
void CVersionStencil(int64_t* x, int64_t* y, int n)
{
    int min = 3, check = 0;
    for (int i = 0; min < (n - 3); i++)
    {
        y[i] = x[min - 3] + x[min - 2] + x[min - 1] + x[min] + x[min + 1] + x[min + 2] + x[min + 3];
        min++;
        check = i;
        //printf("%d\n", check);
    }
}
void errorCheck(int64_t* y1, int64_t* y2, int n)
{
    int errorCount = 0;
    for (int i = 0; i < n; i++)
    {
        if (y1[i] != y2[i])
            errorCount++;
    }
    printf("Error Count:%d\n", errorCount);
}
int main(int argc, char** argv) {
    const int ARRAY_SIZE = VectorSize;
    int64_t* valuesX;
    int64_t* valuesY;
    int64_t* correctY;
    double time_spent;
    clock_t start, end;
    valuesX = (int64_t*)malloc(ARRAY_SIZE * sizeof(int64_t));
    valuesY = (int64_t*)malloc((ARRAY_SIZE - 6) * sizeof(int64_t));
    correctY = (int64_t*)malloc((ARRAY_SIZE - 6) * sizeof(int64_t));
    for (int i = 0; i < (ARRAY_SIZE - 6); i++)
        correctY[i] = (int64_t)(28 + (i * 7));
    for (int i = 0; i < ARRAY_SIZE; i++)
        valuesX[i] = (int64_t)(i + 1);
    for (int i = 0; i < (ARRAY_SIZE-6); i++)
        valuesY[i] = (int64_t)(0);

    printf("*********************C Kernel*********************\n");
    printf("Vector Size:%d\n", ARRAY_SIZE);
    start = clock();
    CVersionStencil(valuesX, valuesY, ARRAY_SIZE);
    end = clock();
    time_spent = ((double)(end - start)) * 1e3 / CLOCKS_PER_SEC;
    errorCheck(valuesY, correctY,5);
    printf("Elapsed time:%lf ms\n", time_spent);



    for (int i = 0; i < (ARRAY_SIZE - 6); i++)
        valuesY[i] = (int64_t)(0);
    printf("*************x86-64 assembly language*************\n");
    printf("Vector Size:%d\n", ARRAY_SIZE);
    start = clock();
    ASMVersionStencil(valuesX, valuesY, ARRAY_SIZE);
    end = clock();
    time_spent = ((double)(end - start)) * 1e3 / CLOCKS_PER_SEC;
    errorCheck(valuesY, correctY, 5);
    printf("Elapsed time:%lf ms\n", time_spent);

    for (int i = 0; i < (ARRAY_SIZE - 6); i++)
        valuesY[i] = (int64_t)(0);
    printf("*************SIMD using XMM register**************\n");
    printf("Vector Size:%d\n", ARRAY_SIZE);
    start = clock();
    SIMDXVersionStencil(valuesX, valuesY, ARRAY_SIZE);
    end = clock();
    time_spent = ((double)(end - start)) * 1e3 / CLOCKS_PER_SEC;
    errorCheck(valuesY, correctY, 5);
    printf("Elapsed time:%lf ms\n", time_spent);

    for (int i = 0; i < (ARRAY_SIZE - 6); i++)
        valuesY[i] = (int64_t)(0);
    printf("*************SIMD using YMM register**************\n");
    printf("Vector Size:%d\n", ARRAY_SIZE);
    start = clock();
    SIMDYVersionStencil(valuesX, valuesY, ARRAY_SIZE);
    end = clock();
    time_spent = ((double)(end - start)) * 1e3 / CLOCKS_PER_SEC;
    errorCheck(valuesY, correctY, 5);
    printf("Elapsed time:%lf ms\n", time_spent);




    return;
}