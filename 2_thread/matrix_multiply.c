/* 
 * matrix_multiply.c
 * 
 * Created on: Mar 1, 2021
 * Author: Jooyul Yoon
 */

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INPM1_ROW   5
#define INPM1_COL   6
#define INPM2_ROW   INPM1_COL
#define INPM2_COL   3
#define OUTM_ROW    INPM1_ROW
#define OUTM_COL    INPM2_COL

#define THREADCNT_ONEPERCELL 0

int inpm1[INPM1_ROW][INPM1_COL] =  {{1,2,3,4,5,6}, 
                                    {7,8,9,10,11,12},
                                    {13,14,15,16,17,18}, 
                                    {19,20,21,22,23,24}, 
                                    {25,26,27,28,29,30}};
int inpm2[INPM2_ROW][INPM2_COL] = { {6,5,4}, 
                                    {3,2,1}, 
                                    {5,3,1}, 
                                    {2,4,6}, 
                                    {4,2,6},
                                    {3,1,5}};
int outm[OUTM_ROW][OUTM_COL];

typedef struct 
{
    /* data */
    int param1;
    int param2;
} parameter;

void* t_function(void*);
void printResult();
void* t_function2(void*);

int main(int argc, char *argv[])
{ 
    // get argument from user input
    int MAX_THREAD = atoi(argv[1]);

    printf("CS149 Spring 2021 parallel matrix multiplication from Jooyul Yoon\n\n");

    // N == 0
    if (MAX_THREAD == THREADCNT_ONEPERCELL)
    {
        // reset the size and declare thread array of the size
        MAX_THREAD = OUTM_ROW * OUTM_COL;
        pthread_t threads[MAX_THREAD];

        int thread_count = 0;

        for (int i = 0; i < OUTM_ROW; i++){
            for(int j = 0; j < OUTM_COL; j++){
                // store target row and target column information into data block
                parameter *data = (parameter *)malloc(sizeof(parameter));

                data->param1 = i; // target row #
                data->param2 = j; // target col #

                // create a thread
                printf("thread[%d]: row= %d, col= %d\n", thread_count, i, j);
                pthread_create(&threads[thread_count++], NULL, t_function, (void *)data);

                // deallocate each data block
                free(data);
            }
        }

        // wait for all thread done
        for (int i = 0; i < MAX_THREAD; i++) {
            pthread_join(threads[i], NULL);
        }

        // Print Resultant Matrix
        printResult();

        return 0;
    }

    // 0 < N <= m
    if (MAX_THREAD <= INPM1_ROW)
    {
        // declare array of threads
        pthread_t threads[MAX_THREAD];

        // initialize variables
        int first_row = 0;
        int divisor = OUTM_ROW / MAX_THREAD;
        int remainder = OUTM_ROW % MAX_THREAD;
        int row_per_thread;

        // every thread
        for(int thread_count = 0; thread_count < MAX_THREAD; thread_count++)
        {
            /*  set rows per thread                                  *
             *  e.g. 29 / 5 = 5 ... 4 - divisor = 5, remainder = 4   *
             *  The first four thread compute 5 + 1 = 6 rows         *
             *  The last thread       compute         5 rows         */
            if(remainder > 0)
            {
                row_per_thread = divisor + 1;
                remainder--;
            }
            else 
            {
                row_per_thread = divisor;
            }

            // store information into data block
            parameter *data = (parameter *)malloc(sizeof(parameter));
            data->param1 = first_row;      // first row of each thread
            data->param2 = row_per_thread; // # of rows of each thread

            // create thread
            printf("thread[%d] - first row= %d, number of rows = %d\n", thread_count, data->param1, data->param2);
            pthread_create(&threads[thread_count], NULL, t_function2, (void *)data);
            
            // deallocate each data block
            free(data);
            
            // update the first row for next thread
            first_row += row_per_thread;
        }
        
        // wait for all thread done
        for (int i = 1; i <= MAX_THREAD; i++) {
            pthread_join(threads[i], NULL);
        }

        // Print Resultant Matrix
        printResult();
    }

    // N > m
    if (MAX_THREAD > INPM1_ROW)
    {
        // print error message
        fprintf(stderr, "[ERROR] enter a value less than %d\n", OUTM_ROW);
    }

    return 0;   
}


// Each thread computes single element in the resultant matrix
// param1 : target row #, param2 : target column #
void* t_function(void* arg) 
{ 
    // declare variables
    parameter *data = (parameter *)arg;
    int row = data->param1;
    int col = data->param2;
    int sum = 0;
    
    // compute multiplication
    for(int i = 0; i < INPM1_COL; i++){
        sum += inpm1[row][i] * inpm2[i][col];
    }

    // store the sum value in the resultant matrix
    outm[row][col] = sum;

    // thread exit
    pthread_exit(NULL);
}

// Each thread computes resultant matrix in certain row
// param1 : first row of each thread, param2 : # of rows in thread
void* t_function2(void *arg) {
    // declare variables
    parameter *data = (parameter *)arg;
    int first_row = data->param1;
    int row_per_thread = data->param2;
    
    for(int row = first_row; row < first_row + row_per_thread; row++){ // row range
        for(int col = 0; col < INPM1_COL; col++){
            
            // compute multiplication of each element
            int sum = 0;
            for(int i = 0; i < INPM1_COL; i++){
                sum += inpm1[row][i] * inpm2[i][col];
            }
            outm[row][col] = sum;

        }
    }
    
    // thread exit
    pthread_exit(NULL);
}

// print Resultant Matrix
void printResult() 
{
        printf("\n --- Resultant Matrix --- \n\n");
        for(int i = 0; i < OUTM_ROW; i++){
            for(int j = 0; j < OUTM_COL; j++){
                printf("%d\t", outm[i][j]);
            }
            printf("\n");
        }
}