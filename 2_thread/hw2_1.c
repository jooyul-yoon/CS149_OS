

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int count;

void *t_function(void * arg)
{
    pid_t *pid = (pid_t *)arg;

    printf("Thread %d\n", count++);
    pthread_exit(NULL);
}



int main()
{
    pthread_t thread;
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        fork();
        printf("haha ");
        pthread_create(&thread, NULL, t_function, &pid);
        fork();
    }
    printf("koko ");
    pthread_create(&thread, NULL, t_function, &pid);
    fork();

    return 0;
}