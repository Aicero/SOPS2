void mutex_style() {
	printf("jestesmy w mutexach\n\n");
	
	tablica = (int *) malloc(aktualnyRozmiar * sizeof(int));
			
	pthread_t brb;
	pthread_t cust;
	pthread_t cust2;
	pthread_t cust3;
	pthread_t cust4;
	
	sem_init(&barbers, 0, 0);
	sem_init(&customers, 0, 0);
	sem_init(&mutex, 0, 1);
	
	pthread_create(&brb, NULL, barber, NULL);
	
	lastCustNr++; // to w while, i++ -> jak i wieksze niz aktualnyRozmiar to realloc
	tablica[1] = lastCustNr;
	pthread_create(&cust, NULL, customer, (void *)&tablica[1]);
	pthread_join(cust, NULL);
	
	//usleep(50*1000);
	lastCustNr++;
	tablica[2] = lastCustNr;
	pthread_create(&cust2, NULL, customer, (void *)&tablica[2]);
	pthread_join(cust2, NULL);
	
	//usleep(50*1000);
	lastCustNr++;
	tablica[3] = lastCustNr;
	pthread_create(&cust2, NULL, customer, (void *)&tablica[3]);
	pthread_join(cust3, NULL);
	
	//usleep(50*1000);
	lastCustNr++;
	tablica[4] = lastCustNr;
	pthread_create(&cust3, NULL, customer, (void *)&tablica[4]);
	pthread_join(cust3, NULL);
	
	pthread_join(brb, NULL);
	free(tablica);
}

void *barber() {
	while(1) {
		// fryzjer oczekuje na pojawienie się klienta
		sem_wait(&customers);
		//printf("Klient budzi fryzjera.\n");
		
		// obszar krytyczny: 
		sem_wait(&mutex);
		//logger(-1);
		//printf("zapraszam\n");
		sem_post(&barbers); // klient jest informowany o możliwości wejścia do gabinetu
		currentlyInWRoom--; // zmniejszenie licznika osób w WRoom
		
		sem_post(&mutex); // wyjście z obszaru krytycznego
	}
}

void *customer(void *number) {
    int num = *(int *)number;
	// obszar krytyczny
	sem_wait(&mutex);
	
	if(currentlyInWRoom < numOfChairs) {
		printf("Klient %d wchodzi do poczekalni.\n", num);
		currentlyInWRoom++;
		logger(num);
	
		sem_post(&customers); // fryzjer jest informowany, że jest jakiś klient
		
		sem_post(&mutex); // wyjście z obszaru krytycznego
		
		sem_wait(&barbers); // oczekiwanie aż fryzjer zaprosi klienta do gabinetu
		
		printf("Wlosy klienta %d sa scinane.\n", num);
		logger(num);
		//usleep(5000*1000);
	}
	else {
		sem_post(&mutex);
		resigned++;
		//printf("\tPoczekalnia pelna - klient %d rezygnuje.\n", num);
		logger(num);
	}
}