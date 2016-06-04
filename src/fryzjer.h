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
int conditional_style();
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

//FIFO http://stackoverflow.com/a/5386266/1558280
typedef struct ticket_lock_t {
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	unsigned long queue_head, queue_tail;
} ticket_lock_t;

#define TICKET_LOCK_INITIALIZER { PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER }

void ticket_lock(ticket_lock_t *ticket)
{
	unsigned long queue_me;

	pthread_mutex_lock(&ticket->mutex);
	queue_me = ticket->queue_tail++;
	while (queue_me != ticket->queue_head)
	{
		pthread_cond_wait(&ticket->cond, &ticket->mutex);
	}
	pthread_mutex_unlock(&ticket->mutex);
}

void ticket_unlock(ticket_lock_t *ticket)
{
	pthread_mutex_lock(&ticket->mutex);
	ticket->queue_head++;
	pthread_cond_broadcast(&ticket->cond);
	pthread_mutex_unlock(&ticket->mutex);
}

void ticket_lock(ticket_lock_t *ticket);
void ticket_unlock(ticket_lock_t *ticket);
void *sleepingBarber();
void *waitingRoom(void *);
void checkWRoom(int);

sem_t customers;
sem_t barbers;
sem_t mutex;
sem_t chair;

#include "mutex_style.c"
#include "logger.c"
#include "resigned_clients.c"
#include "conditional_style.c"


