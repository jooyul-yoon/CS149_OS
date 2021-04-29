#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Argument error handling
    if(argc != 3){
        printf("Please enter two arguments\n");
        exit(1);
    }
    else {
        printf("CS 149 Spring 2021 Logical address translation by Jooyul Yoon 154\n");

        // Get input from commandline and parse them to integer
        int N = atoi(argv[1]);
        int logiAddr = atoi(argv[2]);

        // Compute variables
        int pageSize = 1024 * N;
        int pageNum = logiAddr / pageSize;
        int offset = logiAddr % pageSize;

        // display output
        printf("logical address %d, page size = %d => page number = %d, offset = %d\n", logiAddr, pageSize, pageNum, offset);

    }

    return 0;
}