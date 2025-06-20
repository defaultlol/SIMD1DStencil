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
    int min = 3;
    for (int i = 0; min < (n - 3); i++)
    {
        y[i] = x[min - 3] + x[min - 2] + x[min - 1] + x[min] + x[min + 1] + x[min + 2] + x[min + 3];
        min++;
    } 
}
void errorCheck(int64_t* y, int n) 
{
    int errorCount = 0;
    int k = 0;
    for (k = 0; k < n; k++)
    {   
        if (y[k] != (int64_t)(28 + (k * 7)))
        {
            errorCount++;
        }
    }
    printf("Error Count:%d\n", errorCount);
}
void printResults(int64_t* y, int n) {
    printf("First 10:");
    for (int i = 0; i < 5; i++)
        printf("\t%d",y[i]);
    printf("\n\t");
    for (int i = 5; i < 10; i++)
        printf("\t%d", y[i]);
    printf("\nLast 10:");
    for (int i = 10; i > 5; i--)
        printf("\t%d", y[n-i]);
    printf("\n\t");
    for (int i = 5; i>0; i--)
        printf("\t%d", y[n-i]);
    printf("\n");

}
int main(int argc, char** argv) {
    const int ARRAY_SIZE = VectorSize;
    int64_t* valuesX;
    int64_t* valuesY;
    double time_spent;
    LARGE_INTEGER start, end, freq;
    valuesX = (int64_t*)malloc(ARRAY_SIZE * sizeof(int64_t));
    valuesY = (int64_t*)malloc((ARRAY_SIZE - 6) * sizeof(int64_t));

    for (int i = 0; i < ARRAY_SIZE; i++)
        valuesX[i] = (int64_t)(i + 1);
    for (int i = 0; i < (ARRAY_SIZE-6); i++)
        valuesY[i] = (int64_t)(0);
    QueryPerformanceFrequency(&freq);
    printf("*********************C Kernel*********************\n");
    printf("Vector Size:%d\n", ARRAY_SIZE);
    QueryPerformanceCounter(&start);
    CVersionStencil(valuesX, valuesY, ARRAY_SIZE);
    QueryPerformanceCounter(&end);
    time_spent=(double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart)*1000;
    errorCheck(valuesY,ARRAY_SIZE-6);
    printf("Elapsed time:%lf ms\n", time_spent);
    printResults(valuesY, ARRAY_SIZE - 6);


    for (int i = 0; i < (ARRAY_SIZE - 6); i++)
        valuesY[i] = (int64_t)(0);
    printf("*************x86-64 assembly language*************\n");
    printf("Vector Size:%d\n", ARRAY_SIZE);
    QueryPerformanceCounter(&start);
    ASMVersionStencil(valuesX, valuesY, ARRAY_SIZE);
    QueryPerformanceCounter(&end);
    time_spent = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart) * 1000;
    errorCheck(valuesY,  ARRAY_SIZE - 6);
    printf("Elapsed time:%lf ms\n", time_spent);
    printResults(valuesY, ARRAY_SIZE - 6);




    for (int i = 0; i < (ARRAY_SIZE - 6); i++) 
        valuesY[i] = (int64_t)(0);
    printf("*************SIMD using XMM register**************\n");
    printf("Vector Size:%d\n", ARRAY_SIZE);
    QueryPerformanceCounter(&start);
    SIMDXVersionStencil(valuesX, valuesY, ARRAY_SIZE);
    QueryPerformanceCounter(&end);
    time_spent = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart) * 1000;
    errorCheck(valuesY,  ARRAY_SIZE - 6);
    printf("Elapsed time:%lf ms\n", time_spent);
    printResults(valuesY, ARRAY_SIZE - 6);

    for (int i = 0; i < (ARRAY_SIZE - 6); i++)
        valuesY[i] = (int64_t)(0);
    printf("*************SIMD using YMM register**************\n");
    printf("Vector Size:%d\n", ARRAY_SIZE);
    QueryPerformanceCounter(&start);
    SIMDYVersionStencil(valuesX, valuesY, ARRAY_SIZE);
    QueryPerformanceCounter(&end);
    time_spent = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart) * 1000;
    errorCheck(valuesY,  ARRAY_SIZE - 6);
    printf("Elapsed time:%lf ms\n", time_spent);
    printResults(valuesY, ARRAY_SIZE - 6);



    return;
}