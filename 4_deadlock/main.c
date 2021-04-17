#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

/* the maximum time (in seconds) to eat and play */
#define MAX_EAT_TIME 3
#define MAX_PLAY_TIME 2
/* number of boys, girls */
#define NUM_OF_BOYS 3
#define NUM_OF_GIRLS 4
/* # of games each boy or girl must play before terminate */
#define NUM_OF_GAMES 2
/* playroomcap */
#define MAX_ROOM_CAP 2

/* semaphores and mutex lock */
pthread_mutex_t boys_mutex, girls_mutex;
/* the number of waiting boys, girls */
int boys, girls;
/* counting semaphore -boys, girls waiting to enter playroom*/
sem_t boys_q, girls_q;
/* binarysemaphore -playroom*/
sem_t room_mutex;


void* t_function(void*);
void* t_function2(void*);

typedef struct 
{
    /* data */
    int thread_number;
    int boy_number;
    int num_played;
} parameter;

int main(){
    int NUM_KIDS = NUM_OF_BOYS + NUM_OF_GIRLS;
    pthread_t threads[NUM_KIDS];

    parameter *data = (parameter *)malloc(sizeof(parameter));
    pthread_create(&threads[thread_count++], NULL, t_function, (void *)data);

    return 0;
}