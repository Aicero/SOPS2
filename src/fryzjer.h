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

int *resClients; // tablica przechowujaca liste klientow, ktorzy zrezygnowali z wizyty
int resClientsSize = 10; // zmienna przechowujaca aktualny rozmiar listy Res
volatile int resigned = 0; // liczba klientow ktorzy zrezygnowali z wizyty

volatile int numOfChairs; // liczba krzesel w poczekalni
volatile int currentlyInWRoom = 0; // aktualna liczba klientow w poczekalni

volatile int lastCustNr = 0; // flaga przechowujaca numer ostatnio stworzonego klienta
volatile int custInChair = 0; // flaga przechowujaca numer klienta w gabinecie
volatile int served = 1; // flaga, 'czy ostatni klient obsluzony'

sem_t customers;
sem_t barbers;
sem_t mutex;
sem_t chair;

#include "mutex_style.c"
#include "logger.c"
#include "resigned_clients.c"