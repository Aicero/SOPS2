void mutex_style() {
	prepareResClients(); // funkcja alokujaca pamiec na tablice rezygnujacych klientow

	pthread_t customerThread;
	int thrErr;

	// inicjalizacja semaforow
	sem_init(&barbers, 0, 0);
	sem_init(&customers, 0, 0);
	sem_init(&chair, 0, 1);
	sem_init(&mutex, 0, 1);

	// utworzenie watku fryzjera
	thrErr = pthread_create(&barberThread, NULL, barber, NULL);
	if (thrErr != 0) {
		fprintf(stderr, "error during creating barber thread!");
		exit(EXIT_FAILURE);
	}

	// nieskonczone tworzenie watkow klientow
	while (1) {
		lastCustNr++;

		int *custNr = malloc(sizeof(*custNr));
		if (custNr == NULL) {
			fprintf(stderr, "error allocating memory for next customer number!");
			exit(EXIT_FAILURE);
		}

		*custNr = lastCustNr;
		thrErr = pthread_create(&customerThread, NULL, customer, custNr);
		if (thrErr != 0) {
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

// funkcja obslugujaca watek fryzjera
void *barber() {
	while (1) {
		// oczekiwanie na pojawienie sie klienta
		sem_wait(&customers);

		sem_wait(&mutex); // wejscie w obszar krytyczny
		if (served == 1) { // czy ostatni klient obsluzony
			sem_post(&barbers); // informowanie klientow o mozliwosci wejscia do gabinetu

			// zmniejszenie licznika osob w WRoom
			currentlyInWRoom--;

			// ustawienie flagi
			served = 0;
		}
		
		sem_post(&mutex); // wyjscie z obszaru krytycznego

		sem_wait(&chair); // oczekiwanie na wejscie klienta do gabinetu

		int rnd = rand() % 3;
		usleep(rnd * 500 * 1000); // strzyzenie

		sem_post(&chair); // wypuszczenie klienta z gabinetu
	}
	pthread_exit(0);
}

// funkcja obslugujaca kazdy pojedynczy watek klienta
void *customer(void *number) {
	int num = *(int *)number;
	sem_wait(&mutex); // wejscie w obszar krytyczny

	// rezygnacja klienta, jesli nie ma miejsc
	if (currentlyInWRoom == numOfChairs) {
		addResignedClient(num); // dodawanie numeru zrezygnowanego klienta do listy
		logger();
		sem_post(&mutex); // wyjscie z obszaru krytycznego
	}
	else { // wejscie do poczekalni
		// zwiekszenie licznika osob w WRoom
		currentlyInWRoom++;
		logger();

		sem_post(&customers); // informowanie fryzjera o kliencie

		sem_post(&mutex); // wyjscie z obszaru krytycznego

		sem_wait(&barbers); // oczekiwanie az fryzjer zaprosi klienta do gabinetu

		sem_wait(&mutex); // wejscie do obszaru krytycznego
		custInChair = num; // klient wchodzi do gabinetu
		logger();
		sem_post(&chair); // informacja dla fryzjera, ze klient wszedl do gabinetu
		sem_post(&mutex); // wyjscie z obszaru krytycznego

		sem_wait(&chair); // oczekiwanie, az fryzjer wypusci klienta z gabinetu
		sem_wait(&mutex); // wejscie do obszaru krytycznego
		served = 1; // ustawienie flagi informujacej o obsluzeniu ostatniego klienta
		custInChair = 0; // usuniecie klienta z fotela
		sem_post(&mutex); // wyjscie z obszaru krytycznego
		sem_post(&chair); // informacja dla fryzjera, ze klient wyszedl z gabinetu
	}

	free(number);
}