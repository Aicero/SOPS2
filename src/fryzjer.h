#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void logger();
void mutex_style();
void *barber();
void *customer(void *);

volatile int resigned = 0;
volatile int currentlyInWRoom = 0;
volatile int numOfChairs;

volatile int debug = 0;
volatile int condv = 0;
volatile int lastCustNr = 0;

volatile int custInChair = 0;

volatile int served = 1;

int *tablica;
int aktualnyRozmiar = 10;

sem_t customers;
sem_t barbers;
sem_t mutex;
//pthread_mutex_t mutex;
sem_t chair;

#include "mutex_style.c"
#include "logger.c"