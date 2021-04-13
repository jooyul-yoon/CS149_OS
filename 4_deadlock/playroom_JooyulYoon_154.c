#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

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
/* binary semaphore -playroom*/
sem_t room_mutex;

typedef struct {
    /* data */
    int no;
    int gender;
} parameter;

void* t_function(void*);

int main(){
    printf("CS149 Spring 2021 PlayRoom from Jooyul Yoon\n"); fflush(NULL);

    pthread_t threads[NUM_OF_BOYS + NUM_OF_GIRLS];
    int error;

    // Initialize semaphores and pthread mutex
    if(sem_init(&room_mutex, 0, 1) == -1){
        perror("sem_init"); exit(1);
    }

    if(sem_init(&boys_q, 0, NUM_OF_GAMES) == -1){
        perror("sem_init"); exit(1);
    }

    if(sem_init(&girls_q, 0, NUM_OF_GAMES) == -1){
        perror("sem_init"); exit(1);
    }
    
    if(pthread_mutex_init(&boys_mutex, NULL) != 0 || pthread_mutex_init(&girls_mutex, NULL) != 0){
        perror("mutex init has failed"); exit(1);
    }

    // Create boys threads
    int boy_no[NUM_OF_BOYS];
    for(int i = 0; i < NUM_OF_BOYS; i++) {
        parameter *data = (parameter *)malloc(sizeof(parameter));
        data->no = i;
        data->gender = 0;
        if((error = pthread_create(&threads[i], NULL, t_function, (void *)data)) == -1){
            fprintf(stderr, "Error = %d (%s)\n", error, strerror(error)); exit(1);
        }
    }

    // Create girls threads
    int girl_no[NUM_OF_GIRLS];
    for(int i = 0; i < NUM_OF_GIRLS; i++) {
        parameter *data = (parameter *)malloc(sizeof(parameter));
        data->no = i;
        data->gender = 1;
        if((error = pthread_create(&threads[NUM_OF_BOYS + i], NULL, t_function, (void *)data)) == -1){
            fprintf(stderr, "Error = %d (%s)\n", error, strerror(error)); exit(1);
        }
    }

    // wait for boys threads to terminate
    for (int i = 0; i < NUM_OF_BOYS; i++){
        if((error = pthread_join(threads[i], NULL)) == -1){
            fprintf(stderr, "Error = %d (%s)\n", error, strerror(error)); exit(1);
        }
    }
        
    // wait for boys threads to terminate
    for (int i = 0; i < NUM_OF_GIRLS; i++){
        if((error = pthread_join(threads[NUM_OF_BOYS + i], NULL)) == -1){
            fprintf(stderr, "Error = %d (%s)\n", error, strerror(error)); exit(1);
        }
    }
        
    // Destroy semaphores
    if(sem_destroy(&room_mutex) == -1){
        perror("sem_destroy"); exit(1);
    }
    if(sem_destroy(&boys_q) == -1){
        perror("sem_destroy"); exit(1);
    }
    if(sem_destroy(&girls_q) == -1){
        perror("sem_destroy"); exit(1);
    }

    return 0;
}

void* t_function(void* arg)
{
    parameter *data = (parameter *)arg;
    int no = data->no;
    int gender = data->gender;
    unsigned int num_play = 0;

    if(gender == 0)
    {
        /* every boy thread ends when its number of game played reaches MAX_NUM_GAME */
        while(num_play < NUM_OF_GAMES) {

            /* eating */
            struct timeval cur_time;
            gettimeofday(&cur_time, NULL);
            u_int32_t seed = cur_time.tv_usec;
            int eat_time = (rand_r(&seed) % MAX_EAT_TIME) + 1;
            printf("boy[%d, %d]: eat for %d seconds\n", no, num_play, eat_time); fflush(NULL);
            sleep(eat_time);

            /* arrive at the door */
            if(pthread_mutex_lock(&boys_mutex) == -1){
                perror("pthread_mutex_lock: boys_mutex");
                exit(1);
            }
                // Critical Section
                ++boys;
                printf("boy[%d, %d]: arrive, boys(including me) = %d\n", no, num_play, boys); fflush(NULL);
            
            if(pthread_mutex_unlock(&boys_mutex) == -1){
                perror("pthread_mutex_unlock: boys_mutex");
                exit(1);
            }

            /* first boy */
            if(girls == 0 && boys == 1){
                // printf("first boy\n");
                if(sem_trywait(&room_mutex) == -1){
                    perror("sem_wait: girls_q"); exit(1);
                }
            }

                /* keep waiting when room is full */
                if(sem_wait(&boys_q) == -1){
                    perror("sem_wait: girls_q"); exit(1);
                }

                    /* playing */
                    int play_time = (rand_r(&seed) % MAX_PLAY_TIME) + 1;
                    num_play++;
                    printf("boy[%d, %d]: play for %d seconds\n", no, num_play, play_time); fflush(NULL);
                    sleep(play_time);

                    /* depart from game room */
                    if(pthread_mutex_lock(&boys_mutex) == -1){
                        perror("pthread_mutex_lock: boys_mutex");
                        exit(1);
                    }
                        // Critical Section
                        --boys;
                        printf("boy[%d, %d]: depart, boys(excluding me) = %d\n", no, num_play, boys); fflush(NULL);

                    if(pthread_mutex_unlock(&boys_mutex) == -1){
                        perror("pthread_mutex_unlock: boys_mutex");
                        exit(1);
                    }
            
                /* release when room is vacant */
                if(sem_post(&boys_q) == -1){
                    perror("sem_wait: girls_q"); exit(1);
                }

            /* last boy */
            if(boys == 0){
                // printf("last boy\n");
                if(sem_post(&room_mutex) == -1){
                    perror("sem_wait: girls_q"); exit(1);
                }
            }
        }
    }


    else if(gender == 1)
    {
        /* every girl thread ends when its number of game played reaches MAX_NUM_GAME */
        while(num_play < NUM_OF_GAMES) {

            /* eating */
            struct timeval cur_time;
            gettimeofday(&cur_time, NULL);
            u_int32_t seed = cur_time.tv_usec;
            int eat_time = (rand_r(&seed) % MAX_EAT_TIME) + 1;
            printf("girl[%d, %d]: eat for %d seconds\n", no, num_play, eat_time); fflush(NULL);
            sleep(eat_time);
            
            /* arrive at the door */
            if(pthread_mutex_lock(&girls_mutex) == -1){
                perror("pthread_mutex_lock: boys_mutex");
                exit(1);
            }
                // Critical Section
                ++girls;
                printf("girl[%d, %d]: arrive, girls(including me) = %d\n", no, num_play, girls); fflush(NULL);

            if(pthread_mutex_unlock(&girls_mutex) == -1){
                perror("pthread_mutex_unlock: boys_mutex");
                exit(1);
            }

            /* first girl */
            if(boys == 0 && girls == 1){
                // printf("first girl\n");
                if(sem_trywait(&room_mutex) == -1){
                    perror("sem_wait: girls_q"); exit(1);
                }
            }

                /* keep waiting when room is full */
                if(sem_wait(&girls_q) == -1){
                    perror("sem_wait: girls_q"); exit(1);
                }

                    /* playing */
                    int play_time = (rand_r(&seed) % MAX_PLAY_TIME) + 1;
                    num_play++;
                    printf("girl[%d, %d]: play for %d seconds\n", no, num_play, play_time); fflush(NULL);
                    sleep(play_time);

                    /* depart from game room  */
                    if(pthread_mutex_lock(&girls_mutex) == -1){
                        perror("pthread_mutex_lock: boys_mutex");
                        exit(1);
                    }
                        // Critical Section
                        --girls;
                        printf("girl[%d, %d]: depart, girls(excluding me) = %d\n", no, num_play, girls); fflush(NULL);
                
                    if(pthread_mutex_unlock(&girls_mutex) == -1){
                        perror("pthread_mutex_unlock: boys_mutex");
                        exit(1);
                    }      
            
            /* release when room is vacant */
            if(sem_post(&girls_q) == -1){
                perror("sem_wait: girls_q"); exit(1);
            }

            /* last girl */
            if(girls == 0){
                // printf("last girl\n");
                if(sem_post(&room_mutex) == -1){
                    perror("sem_wait: girls_q"); exit(1);
                }
            }
        }
    }
    else
    {
        printf("Error: gender not exists\n"); exit(1);
    }
    

    // deallocate each data block
    free(data);
    
    return NULL;
}