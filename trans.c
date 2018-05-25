/*
 * Name: Chenyao Lou
 * ID: 2016011343
 */

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int t0, t1, t2, t3, t4, t5, t6, t7;
    int block_i, block_j, i, j;

    if (N == 32) {
        for (block_i = 0; block_i < N; block_i += 8) {
            for (block_j = 0; block_j < M; block_j += 8) {
                for (i = block_i; i < block_i + 8 && i < N; i++) {
                    t0=A[i][block_j+0]; t1=A[i][block_j+1]; t2=A[i][block_j+2]; t3=A[i][block_j+3];
                    t4=A[i][block_j+4]; t5=A[i][block_j+5]; t6=A[i][block_j+6]; t7=A[i][block_j+7];
                    B[block_j+0][i]=t0; B[block_j+1][i]=t1; B[block_j+2][i]=t2; B[block_j+3][i]=t3;
                    B[block_j+4][i]=t4; B[block_j+5][i]=t5; B[block_j+6][i]=t6; B[block_j+7][i]=t7;
                }
            }
        }
    } else if (N == 64) {
        for (block_i = 0; block_i < N; block_i += 8) {
            for (block_j = 0; block_j < M; block_j += 8) {
                // left-up corner
                for (i = block_i; i < block_i + 4; i++) {
                    t0=A[i][block_j+0]; t1=A[i][block_j+1]; t2=A[i][block_j+2]; t3=A[i][block_j+3];
                    B[block_j+0][i]=t0; B[block_j+1][i]=t1; B[block_j+2][i]=t2; B[block_j+3][i]=t3;
                }

                // right-up corner
                for (i = 0; i < 4; i++) {
                    t0=A[block_i+i][block_j+4]; t1=A[block_i+i][block_j+5]; t2=A[block_i+i][block_j+6]; t3=A[block_i+i][block_j+7];
                    B[block_j+0][block_i+4+i]=t0; B[block_j+1][block_i+4+i]=t1; B[block_j+2][block_i+4+i]=t2; B[block_j+3][block_i+4+i]=t3;
                }

                // left-down corner
                for (i = 0; i < 4; i++) {
                    t0=B[block_j+i][block_i+4]; t1=B[block_j+i][block_i+5]; t2=B[block_j+i][block_i+6]; t3=B[block_j+i][block_i+7];
                    t4=A[block_i+4][block_j+i]; t5=A[block_i+5][block_j+i]; t6=A[block_i+6][block_j+i]; t7=A[block_i+7][block_j+i];
                    B[block_j+i][block_i+4]=t4; B[block_j+i][block_i+5]=t5; B[block_j+i][block_i+6]=t6; B[block_j+i][block_i+7]=t7;
                    B[block_j+4+i][block_i+0]=t0; B[block_j+4+i][block_i+1]=t1; B[block_j+4+i][block_i+2]=t2; B[block_j+4+i][block_i+3]=t3;
                }

                // rigth-down corner
                for (i = block_i + 4; i < block_i + 8; i++) {
                    t0=A[i][block_j+4]; t1=A[i][block_j+5]; t2=A[i][block_j+6]; t3=A[i][block_j+7];
                    B[block_j+4][i]=t0; B[block_j+5][i]=t1; B[block_j+6][i]=t2; B[block_j+7][i]=t3;
                }
            }
        }
    } else {
        for (block_i = 0; block_i < N; block_i += 16) {
            for (block_j = 0; block_j < M; block_j += 16) {
                for (i = block_i; i < N && i < block_i + 16; i++) {
                    for (j = block_j; j < M && j < block_j + 16; j++) {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char trans16x16_desc[] = "Transpose by spliting to 16x16 block.";
void trans16x16(int M, int N, int A[N][M], int B[M][N])
{
    int block_i, block_j, i, j;
    for (block_i = 0; block_i < N; block_i += 16) {
        for (block_j = 0; block_j < M; block_j += 16) {
            for (i = block_i; i < N && i < block_i + 16; i++) {
                for (j = block_j; j < M && j < block_j + 16; j++) {
                    B[j][i] = A[i][j];
                }
            }
        }
    }
}

char trans4x4_desc[] = "Transpose by spliting to 4x4 block.";
void trans4x4(int M, int N, int A[N][M], int B[M][N])
{
    int block_i, block_j, i, j;
    for (block_i = 0; block_i < N; block_i += 4) {
        for (block_j = 0; block_j < M; block_j += 4) {
            for (i = block_i; i < N && i < block_i + 4; i++) {
                for (j = block_j; j < M && j < block_j + 4; j++) {
                    B[j][i] = A[i][j];
                }
            }
        }
    }
}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    registerTransFunction(trans16x16, trans16x16_desc); 
    registerTransFunction(trans4x4, trans4x4_desc); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

