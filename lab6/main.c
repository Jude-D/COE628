/**
 *
 * COE 628 - Operating Systems
 * Lab 6
 *
 **/

// if not already compiled run: (wont build in netbeans)
// gcc -pthread -o main main.c
// else if the main executable is already in folder you can just run it
#include <stdlib.h>     /* NULL */
#include  <stdio.h>	/* printf */
#include  <sys/types.h>	/* pid_t */
#include <unistd.h>	/* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h> 	/* wait */
#include <pthread.h>

#define PRODUCER_NO 5	//Number of producers, can change here
#define NUM_PRODUCED 20 //2000 //Number of items to be produced, can change here

void *generator_function(void*);
void *print_function(void*);
long sum; /* Sum of generated values*/
long finished_producers; /* number of the producer that finished producing */

//C: Mutex declaration and initialization
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

//F: Condition variable declaration and initialization
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

int main(void) {
    int i;
    /* initialize random seed: */
    srand(time(NULL));
    sum = 0;
    finished_producers=0;
    
    //A: Creates five generator thread
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";
    char *message3 = "Thread 3";
    char *message4 = "Thread 4";
    char *message5 = "Thread 5";
    char *message6 = "Thread Print";
    int iret1, iret2, iret3, iret4, iret5, iret6;
    
    iret1 = pthread_create( &thread1, NULL, generator_function, (void*) message1);
    iret2 = pthread_create( &thread2, NULL, generator_function, (void*) message2);
    iret3 = pthread_create( &thread3, NULL, generator_function, (void*) message3);
    iret4 = pthread_create( &thread4, NULL, generator_function, (void*) message4);
    iret5 = pthread_create( &thread5, NULL, generator_function, (void*) message5);
    
    //D: Creates print thread
    iret6 = pthread_create(&thread6, NULL, print_function, (void*) message6);
    
    //B: Makes sure that all generator threads has finished before proceeding
    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
    pthread_join( thread3, NULL);
    pthread_join( thread4, NULL);
    pthread_join( thread5, NULL);
    
    //E: Makes sure that print thread has finished before proceeding
   
    pthread_join( thread6, NULL);
       
    return (0);

}

void * generator_function(void* junk) {
    pthread_mutex_lock(&mutex1); // lock
    
    long counter = 0;
    long sum_this_generator = 0;
   
    while (counter < NUM_PRODUCED) {

      long tmpNumber = sum;
      long rnd_number = 1;//rand() % 10; 
        // can make = 1, output should be 100 (20 loops*5 threads)
      printf("current sum of the generated number up to now is %ld going to add %ld to it.\n", tmpNumber, rnd_number);
        sum = tmpNumber + rnd_number;
        counter++;
        sum_this_generator += rnd_number;
        usleep(1000);
    }
    
    
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    printf("The sum of produced items for this number generator at the end is: %ld \n", sum_this_generator);
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    finished_producers++;
    
    
    //H: If all generator has finished fire signal for condition variable
    if (finished_producers == PRODUCER_NO){
        pthread_cond_signal( &cond1 );
    }
    pthread_mutex_unlock(&mutex1); // unlock
    return (0);
}

void *print_function(void* junk) {
    //G: Wait until all generator has finished
    pthread_mutex_lock( &mutex1 );
      while(finished_producers != PRODUCER_NO )
      {
         pthread_cond_wait( &cond1, &mutex1 );
      }
    
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("The value of counter at the end is: %ld \n", sum);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    pthread_mutex_unlock( &mutex1 );
}