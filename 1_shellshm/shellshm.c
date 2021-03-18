/* 
 * shellshm.c
 * 
 * Created on: Feb 27, 2021
 * Author: Jooyul Yoon
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <fcntl.h>

#define MAXLINE 80 /* The maximum length of a command line */
#define MAXARGS 40 /* The maximum # of tokens in command line */

// shared memory
#define SHM_NAME = "cs149shell"

/* This method is used to remove the newline from cmdline */
char *choppy( char *s )
{
    char *n = malloc( strlen( s ? s : "\n" ) );
    if( s )
        strcpy( n, s );
    n[strlen(n)-1]='\0';
    return n;
}

int main(void)
{
    char cmdline[MAXLINE];
    char *args[MAXARGS];

    // shared memory
    struct timeval start, end;
    int fd[2];
    pipe(fd);
    

    while(1) 
    {
        /* prompt */
        fprintf(stdout, "CS 149 Spring 2021 Shellshm from Jooyul Yoon\n");
        fflush(stdout);
        printf("Jooyul-154> ");

        /* read user input */
        fgets(cmdline, MAXLINE, stdin); /* input */


        /* parse tokens from cmdline to args[] */        
        char *token;
        char* temp = choppy(cmdline);
        int argCount = 0;

        while((token = strtok_r(temp, " ", &temp)) && argCount < MAXARGS)
        {
            if(token == NULL)
                break;
            args[argCount] = token; /* store tokens in args */
            argCount++;
        }
        args[argCount] = NULL;


        /* 3 - if user enters exit, terminate the shell */
        if(strcmp(args[0], "exit")== 0)
        {
            return -1;
        }
        

        /* fork a child process using fork() */
        pid_t pid = fork();
        
        if (pid < 0) // fork error handling
        {
            fprintf(stderr, "Fork error\n");
            exit(1);
        }
        else if (pid == 0) // fork child case
        {   
            // close(fd[0]);
            // gettimeofday(&start, NULL);
            // char buffer[100];
            // sprintf(buffer, "%ld %d", start.tv_sec, start.tv_usec);
            // // printf("Child: %d\n", start.tv_usec);
            // write(fd[1],buffer,strlen(buffer)+1);

            // /* call execvp */
            execvp(args[0], args);
            fprintf(stderr, "Child process could not do execvp\n");

            // close(fd[1]);

        }
        else  // fork parent case
        {
            // Initialize variables for timer
            double timetaken;
            long int startsec, startusec;

            // bring start time from child fork
            close(fd[1]);
            char buffer[100];
            dup2(fd[0], 0);
            read(fd[0], buffer, 100);
            scanf("%ld", &startsec);
            scanf("%ld",&startusec);
            
            // set end time and calculate the time taken
            gettimeofday(&end,NULL);
            timetaken = (end.tv_sec - startusec) * 1e-6;
            timetaken = (timetaken + (end.tv_usec - startusec)) * 1e-6;
            // printf("Parent: %d\n", end.tv_usec);
            printf("\nelapsed time: %0.6f sec\n", timetaken); 
            close(fd[0]);

            // if no & at the end of cmdline, call wait() to wait 
            // for the termination of the child, else do nothing
            if(strcmp(args[argCount - 1], "&") != 0)
            {
                wait(NULL);
            }
        }

        printf("\n");
    }
    return 0;
}