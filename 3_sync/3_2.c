#define MAX_PROCS 1023

int num_of_procs = 0; 

/* the implementation of fork() calls this function */
int allocate_process() {
    int new_pid;
    if (num_of_procs == MAX_PROCS) 
        return -1;
    else    /* allocate process resources and assign the PID to new_pid */
        return new_pid;
}

/* the implementation of exit() calls this function */
void release_process() {
    /* release process resources */
    --num_of_procs;
}