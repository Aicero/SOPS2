#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void mutex_style();
void *barber();
void *customer(void *nr);

volatile int resigned = 0;
volatile int currentlyInWRoom = 0;
volatile int numOfChairs;

volatile int debug = 0;
volatile int condv = 0;
volatile int customerNumber = 0;

//pthread_mutex_t gabinet = PTHREAD_MUTEX_INITIALIZER;

sem_t WRoom;
sem_t InChair;
sem_t BusyAsHell;
sem_t Sleep;

pthread_t brb;

#include "mutex_style.c"