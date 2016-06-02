void mutex_style() {
	printf("jestesmy w mutexach\n\n");
	
	tablica = (int *) malloc(aktualnyRozmiar * sizeof(int));
	if(tablica == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
			
	pthread_t brb;
	pthread_t cust;
	
	sem_init(&barbers, 0, 0);
	sem_init(&customers, 0, 0);
	sem_init(&chair, 0, 1);
	sem_init(&mutex, 0, 1);
	
	pthread_create(&brb, NULL, barber, NULL);
	
	while (1) {
		usleep(5000*100);
		lastCustNr++;
		
		if (lastCustNr == aktualnyRozmiar) {
			printf("\taktualny rozmiar: %d", aktualnyRozmiar);
			printf("\ttworzenie klienta %d -> realloc\n", lastCustNr);
			aktualnyRozmiar *= 2;
			tablica = realloc(tablica, aktualnyRozmiar*sizeof(int));
			if (tablica == NULL) {
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
		tablica[lastCustNr] = lastCustNr;
		pthread_create(&cust, NULL, customer, (void *)&tablica[lastCustNr]);
	}
		
	pthread_join(brb, NULL);
	free(tablica);
}

void *barber() {
	while(1) {
		// fryzjer oczekuje na pojawienie się klienta
		sem_wait(&customers);
		
		// obszar krytyczny: 
		sem_wait(&mutex);
		
		if(served == 1) {
			sem_post(&barbers); // klient jest informowany o możliwości wejścia do gabinetu	
			printf("\tprzed: %d\n", currentlyInWRoom);
			currentlyInWRoom--; // zmniejszenie licznika osób w WRoom
			served = 0;
			printf("\t\tpo: %d\n", currentlyInWRoom);
			logger();
		}
		
		sem_post(&mutex); // wyjście z obszaru krytycznego
	}
}

void *customer(void *number) {
    int num = *(int *)number;
	sem_wait(&mutex); // obszar krytyczny
	
	if(currentlyInWRoom < numOfChairs) {
		printf("Klient %d wchodzi do poczekalni.\n", num);
		currentlyInWRoom++;
		logger();
	
		sem_post(&customers); // fryzjer jest informowany, że jest jakiś klient
		
		sem_post(&mutex); // wyjście z obszaru krytycznego
		
		sem_wait(&barbers); // oczekiwanie aż fryzjer zaprosi klienta do gabinetu
		
		sem_wait(&chair);
		custInChair = num;
		logger();
		printf("\tKlient %d wchodzi do gabinetu.\n", num);
		usleep(5000*200);
		sem_wait(&mutex);
		served = 1;
		custInChair = 0;
		logger();
		printf("\nhalo lapki w gore\n");
		
		sem_post(&mutex);
		sem_post(&chair);
	}
	else {
		resigned++;
		sem_post(&mutex);
		printf("\tPoczekalnia pelna - klient %d rezygnuje.\n", num);
		logger();
	}
}