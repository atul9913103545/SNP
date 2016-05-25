#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
# include <unistd.h>

// The maximum number of customer threads.

#define MAX_CUSTOMERS 25

volatile int currentCustomer = -1;

// Function prototypes...

void *customer(void *num);

void *tech_assistant(void *);

void randwait(int secs);

// Define the semaphores.

// on_hold Limits the # of customers allowed
// to enter the on hold room at one time.

sem_t on_hold;

// on_call ensures mutually exclusive access to
// the tech_assistant .

sem_t on_call;

// on_break is used to allow the tech_assistant to rest
// until a customer arrives.

sem_t on_break;

// on_service is used to make the customer to wait until
// the tech_assistant is done assisting present customer.

sem_t on_service;

// Flag to stop the tech_assistant thread when all customers
// have been serviced.

int allDone = 0;

int main(int argc, char *argv[]) {

    pthread_t ttid;

    pthread_t tid[MAX_CUSTOMERS];

    int i, numCustomers, numhold;

    int Number[MAX_CUSTOMERS];

    // Check to make sure there are the right number of
    // command line arguments.

    if (argc != 3) {

	printf("Use: Sleeptech_assistant <Num Customers> <Num OnHold>\n");
	exit(-1);
    }

    // Get the command line arguments and convert them
    // into integers.
    numCustomers = atoi(argv[1]);

    numhold = atoi(argv[2]);

    // Make sure the number of threads is less than the number of
    // customers we can support.

    if (numCustomers > MAX_CUSTOMERS) {

	printf("The maximum number of Customers is %d.\n", MAX_CUSTOMERS);
	exit(-1);
    }

    printf("A solution to the Call Center tech_assistant problem using semaphores.\n");

    // Initialize the numbers array.

    for (i=0; i<MAX_CUSTOMERS; i++) {
	Number[i] = i;
    }

    // Initialize the semaphores with initial values...

    sem_init(&on_hold, 0, numhold);

    sem_init(&on_call, 0, 1);

    sem_init(&on_break, 0, 0);

    sem_init(&on_service, 0, 0);

    // Create the tech_assistant.

    pthread_create(&ttid, NULL, tech_assistant, NULL);

    // Create the customers.

    for (i=0; i<numCustomers; i++) {
	pthread_create(&tid[i], NULL, customer, (void *)&Number[i]);
    }

    // Join each of the threads to wait for them to finish.

    for (i=0; i<numCustomers; i++) {
	pthread_join(tid[i],NULL);
    }

    // When all of the customers are finished, kill the
    // tech_assistant thread.

    allDone = 1;

    sem_post(&on_break);  // Wake the tech_assistant so he will exit.

    pthread_join(ttid,NULL);
}

void *customer(void *number) {
    int num = *(int *)number;

    // Dial customer support and take some random amount of
    // time to connect.

    printf("Customer %d dialing for tech_assistant support.\n", num);

    randwait(1);

    printf("Customer %d call arrived at tech_assistant support.\n", num);

    // Wait for space to open up in the on hold room...

   sem_wait(&on_hold);

    printf("Customer %d entering OnHold.\n", num);

    // Wait for the tech_assistant to become free.

    sem_wait(&on_call);

    // On hold room space is free so give up your spot in the
    //  room.

    sem_post(&on_hold);

    // Wake up the tech_assistant...

    printf("Customer %d waking the tech_assistant.\n", num);

    sem_post(&on_break);

    currentCustomer = num;

    // Wait for the tech_assistant to finish assisting current customer.

    sem_wait(&on_service);
    // Customer ends call with assistant.

    sem_post(&on_call);

    printf("Customer %d has ended with tech_assistant support.\n", num);
}

void *tech_assistant(void *junk) {

    // While there are still customers to be serviced...

    while (!allDone) {
	// Sleep until someone arrives and wakes you..

	printf("The tech_assistant is OnBreak\n");

	sem_wait(&on_break);

	if (!allDone) {
	    // Take a 2 sec of time to assist
	    // customer.

	    printf("The tech_assistant is assisting the customer %d\n",currentCustomer);
    randwait(2);

	    printf("The tech_assistant has finished assisting the customer %d.\n",currentCustomer);

	    // Release the customer when done assisting...

	    sem_post(&on_service);
	}

	else {

	    printf("The tech_assistant is going home for the day.\n");

	}

    }

}

void randwait(int secs) {

        sleep(secs);

}
