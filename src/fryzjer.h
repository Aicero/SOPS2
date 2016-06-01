#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void mutex_style();
void *barber();
void *customer();

volatile int resigned = 0;
volatile int currentlyInWRoom = 0;
volatile int numOfChairs;

volatile int debug = 0;
volatile int condv = 0;
volatile int lastCustNr = 0;

int *tablica;
int aktualnyRozmiar = 10;

sem_t customers;
sem_t barbers;
sem_t mutex;

#include "mutex_style.c"