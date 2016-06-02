void mutex_style() {
	tablica = (int *)malloc(aktualnyRozmiar * sizeof(int));
	if (tablica == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	pthread_t brb;
	pthread_t cust;
	int thrErr;

	// inicjalizacja semaforow
	sem_init(&barbers, 0, 0);
	sem_init(&customers, 0, 0);
	sem_init(&chair, 0, 1);
	sem_init(&mutex, 0, 1);

	// utworzenie watku fryzjera
	thrErr = pthread_create(&brb, NULL, barber, NULL);
	if (thrErr != 0) {
		printf("error during creating barber thread!");
		exit(EXIT_FAILURE);
	}

	// ciagle tworzenie watkow klientow
	while (1) {
		lastCustNr++;

		if (lastCustNr == aktualnyRozmiar) {
			aktualnyRozmiar *= 2;
			tablica = realloc(tablica, aktualnyRozmiar*sizeof(int));
			if (tablica == NULL) {
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}

		tablica[lastCustNr] = lastCustNr;
		thrErr = pthread_create(&cust, NULL, customer, (void *)&tablica[lastCustNr]);
		if (thrErr != 0) {
			printf("error during creating barber thread!");
			exit(EXIT_FAILURE);
		}

		int rnd = rand() % 3;
		usleep(rnd * 500 * 1000);
	}
}

// funkcja obslugujaca watek fryzjera
void *barber() {
	while (1) {
		// oczekiwanie na pojawienie sie klienta
		sem_wait(&customers);

		sem_wait(&mutex); // wejscie w obszar krytyczny
		if (served == 1) { // weryfikacja, czy ostatni klient zostal obsluzony
			sem_post(&barbers); // klienci sa informowani o mozliwosci wejscia do gabinetu	

			// zmniejszenie licznika osob w WRoom
			currentlyInWRoom--;

			// ustawienie flagi, ktora zablokuje ponowne wejscie do tego ifa --> zmniejszenie licznika odbedzie sie raz
			served = 0;
		}
		sem_post(&mutex); // wyjscie z obszaru krytycznego

		sem_wait(&chair); // oczekiwanie na wejscie klienta do gabinetu

		int rnd = rand() % 3;
		usleep(rnd * 500 * 1000);

		sem_post(&chair); // wypuszczenie klienta z gabinetu
	}
}

// funkcja obslugujaca kazdy pojedynczy watek klienta
void *customer(void *number) {
	int num = *(int *)number;
	sem_wait(&mutex); // wejscie w obszar krytyczny

	// jesli aktualnie w WRoom jest tyle klientow ile jest krzesel to klient rezygnuje
	if (currentlyInWRoom == numOfChairs) {
		resigned++; // zwiekszenie licznika osob, ktore zrezygnowaly
		logger();
		sem_post(&mutex); // wyjscie z obszaru krytycznego
	}
	// w przeciwnym wypadku klient wchodzi do poczekalni
	else {
		// zwiekszenie licznika osob w WRoom
		currentlyInWRoom++;
		logger();

		sem_post(&customers); // fryzjer jest informowany, ze jest jakis klient

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
		logger();
		sem_post(&mutex); // wyjscie z obszaru krytycznego
		sem_post(&chair); // informacja dla fryzjera, ze klient wyszedl z gabinetu
	}
}