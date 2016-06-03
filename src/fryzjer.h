#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void logger();
void prepareResClients();
void addResignedClient(int);
void mutex_style();
void *barber();
void *customer(void *);

volatile int debug = 0;
volatile int condv = 0;

volatile int resigned = 0;
int *resClients;
int resClientsSize = 10;

volatile int currentlyInWRoom = 0;
volatile int numOfChairs;

volatile int lastCustNr = 0;
volatile int custInChair = 0;
volatile int served = 1;

sem_t customers;
sem_t barbers;
sem_t mutex;
sem_t chair;

#include "mutex_style.c"
#include "logger.c"
#include "resigned_clients.c"