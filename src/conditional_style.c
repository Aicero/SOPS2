void conditional_style() {
	prepareResClients(); // funkcja alokujaca pamiec na tablice rezygnujacych klientow

	pthread_t customerThread;
	int thrErr;

	// tworzenie watku fryzjera
	thrErr = pthread_create(&barberThread, NULL, sleepingBarber, NULL);
	if (thrErr != 0) {
		fprintf(stderr, "error during creating barber thread!");
		exit(EXIT_FAILURE);
	}

	// nieskonczone tworzenie watkow klientow
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

		int rnd = rand() % 4;
		usleep(rnd * 500 * 1000); // czas oczekiwania na przyjscie kolejnego klienta
	}

	// zabezpieczenie przed deadlockiem
	pthread_join(barberThread, NULL);
	pthread_join(customerThread, NULL);
}

// poczekalnia
void *waitingRoom(void *number) {
	int num = *(int *)number;

	// lock na mutexie poczekalni (sprawdzac moze tylko jeden klient na raz)
	ticket_lock(&queueMutex);

	// klient sprawdza wolne miejsca, jesli nie ma -> rezygnuje
	if (currentlyInWRoom == numOfChairs) {
		addResignedClient(num); // dodawanie numeru zrezygnowanego klienta do listy
		logger();
		ticket_unlock(&queueMutex); // wyjscie z obszaru krytycznego
	}
	else { // byly miejsca w poczekalni
		currentlyInWRoom++;
		pushToWRoomList(num);
		logger();

		if (sleeping == 1) { // sprawdzanie czy fryzjer spi
			pthread_cond_signal(&sleepingBarber_cond); // klient go budzi
		}

		// klient zwalnia queueMutex - poczekalnie moze sprawdzic kolejny klient
		ticket_unlock(&queueMutex);

		// klient zajmuje miejsce na końcu kolejki
		ticket_lock(&queueFIFOMutex);
		pthread_mutex_lock(&waitMutex);
		ticket_unlock(&queueFIFOMutex);

		// klient czeka na zwolnienie miejsca u fryzjera
		pthread_cond_wait(&workingBarber_cond, &waitMutex);
		custInChair = num;
		removeFromWRoomList(num); // usuniecie osoby z WRoom list
		currentlyInWRoom--;
		logger();
		pthread_cond_wait(&workingBarber_cond, &waitMutex);
		custInChair = 0;
		logger();
		pthread_mutex_unlock(&waitMutex);
	}
	free(number);
}

// fryzjer
void *sleepingBarber() {
	while (1) {
		pthread_mutex_lock(&waitMutex);
		if (currentlyInWRoom == 0) {
			pthread_mutex_unlock(&waitMutex);
			// brak klientow, fryzjer spi
			pthread_mutex_lock(&sleepMutex);
			sleeping = 1;

			// oczekiwanie na klienta
			pthread_cond_wait(&sleepingBarber_cond, &sleepMutex);

			// klient przyszedł, wybudzenie fryzjera
			sleeping = 0;
			pthread_mutex_unlock(&sleepMutex);
		}
		else {
			// fryzjer zaczyna strzyzenie
			pthread_mutex_unlock(&waitMutex);

			int rnd = rand() % 3;
			usleep(rnd * 500 * 1000); // strzyzenie

			// koniec strzyzenia, wyslanie sygnalu o zwolnieniu miejsca u fryzjera
			pthread_cond_signal(&workingBarber_cond);
		}
	}
	pthread_exit(0);
}
