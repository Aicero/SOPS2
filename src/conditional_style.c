pthread_cond_t sleepingBarber_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t workingBarber_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sleepMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t waitMutex = PTHREAD_MUTEX_INITIALIZER;
ticket_lock_t queueMutex = TICKET_LOCK_INITIALIZER;

volatile int sleeping = 0, rndSeed;

int conditional_style()
{
	rndSeed = time(NULL);
	srand(rndSeed);
	pthread_t barberThread, customerThread;
	int thrErr;
	//Tworzenie watku fryzjera
	pthread_create(&barberThread, NULL, sleepingBarber, NULL);
	//Tworzenie watku klientow
	while (1)
	{
		lastCustNr++;

		int *custNr = malloc(sizeof(*custNr));
		if (custNr == NULL)
		{
			fprintf(stderr, "error allocating memory for next customer number!");
			exit(EXIT_FAILURE);
		}

		*custNr = lastCustNr;
		thrErr = pthread_create(&customerThread, NULL, waitingRoom, custNr);
		if (thrErr != 0)
		{
			fprintf(stderr, "error during creating barber thread!");
			exit(EXIT_FAILURE);
		}

		int rnd = rand() % 3;
		usleep(rnd * 500 * 1000); // czas oczekiwania na przyjscie kolejnego klienta
	}

	//zabezpieczenie przed deadlockiem
	//pthread_join(barber,NULL);
	//pthread_join(customerThread,NULL);

	return 0;
}

//Poczekalnia
void *waitingRoom(void *number)
{
	int num = *(int *)number;
	//Lock na mutexie poczekalni (sprawdzac moze tylko jeden klient na raz)
	ticket_lock(&queueMutex);
	//Sprawdzenie poczekalni
	checkWRoom(num);
	//Po zakonczeniu
	free(number);
	//Poczekalnia byla zajeta, klient rezygnuje
	//tutaj dodac do listy klientow ktorzy zrezygnowali
}

//Fryzjer
void *sleepingBarber()
{
	while (1)
	{
		if (currentlyInWRoom == 0)
		{
			//Brak klientow, fryzjer spi
			pthread_mutex_lock(&sleepMutex);
			sleeping = 1;
			//Oczekiwanie na klienta
			pthread_cond_wait(&sleepingBarber_cond, &sleepMutex);
			//Klient przyszedł, wybudzenie fryzjera
			sleeping = 0;
			pthread_mutex_unlock(&sleepMutex);
		}
		else
		{
			//Fryzjer zaczyna strzyzenie
			sleep(rand() % 5);
			//koniec strzyzenia, klient wychodzi
			currentlyInWRoom--;
			//Wyslanie sygnalu o zwolnieniu miejsca u fryzjera
			custInChair = 0;
			pthread_cond_signal(&workingBarber_cond);
		}
	}
	pthread_exit(0);
}

void checkWRoom(int number)
{
	//Klient wszedl do poczekalni "zajmuje miejsce mentalnie"
	currentlyInWRoom++;
	//Klient sprawdza wolne miejsca
	if (currentlyInWRoom < numOfChairs)
	{
		//Byly miejsca w poczekalni, sprawdzanie czy fryzjer spi
		if (sleeping == 1)
		{
			//Fryzjer spal, klient go budzi
			pthread_cond_signal(&sleepingBarber_cond);
		}
		//Klient zwalnia queueMutex - poczekalnie moze sprawdzic kolejny klient
		ticket_unlock(&queueMutex);
		//Klient zajmuje miejsce na samym poczatku kolejki
		pthread_mutex_lock(&waitMutex);
		logger();
		//Klient czeka na zwolnienie miejsca u fryzjera
		pthread_cond_wait(&workingBarber_cond, &waitMutex);
		custInChair = number;
		logger();
		pthread_mutex_unlock(&waitMutex);
		return;
	}
	if (currentlyInWRoom >= numOfChairs)
	{
		//Poczekalnia jest pelna, klient wychodzi
		currentlyInWRoom--;
		addResignedClient(number); // dodawanie numeru zrezygnowanego klienta do listy
		logger();
		//I zwalnia queueMutex - poczekalnie moze sprawdzic kolejny klient
		ticket_unlock(&queueMutex);
		return;
	}
}
