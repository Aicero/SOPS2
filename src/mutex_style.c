void mutex_style() {
	printf("jestesmy w mutexach\n\n");
	
	tablica = (int *) malloc(aktualnyRozmiar * sizeof(int));
	tablica[1] = 1;
	tablica[2] = 2;
			
	pthread_t brb;
	pthread_t cust;
	pthread_t cust2;
	
	sem_init(&barbers, 0, 0);
	sem_init(&customers, 0, 0);
	sem_init(&mutex, 0, 1);
	
	pthread_create(&brb, NULL, barber, NULL);
	
	lastCustNr++;// to w while, i++ -> jak i wieksze niz aktualnyRozmiar to realloc
	pthread_create(&cust, NULL, customer, (void *)&tablica[1]);
	lastCustNr++;
	pthread_create(&cust2, NULL, customer, (void *)&tablica[2]);
	
	pthread_join(brb, NULL);
	pthread_join(cust, NULL);
	pthread_join(cust2, NULL);
	free(tablica);
}

void *barber() {
	while(1) {
		printf("Fryzjer SPI.\n");
		sem_wait(&customers);
		printf("jest jakis klient\n");
		sem_wait(&mutex);
		printf("Fryzjer zaprasza klienta na fotel.\n");
		currentlyInWRoom--;
		
		sem_post(&barbers);
		
		sem_post(&mutex);
		
		printf("Fryzjer scina wlosy klienta.\n");
	}
}

void *customer(void *number) {
    int num = *(int *)number;
	sem_wait(&mutex);
	
	if(currentlyInWRoom < numOfChairs) {
		printf("Klient %d wchodzi do poczekalni.\n", num);
		currentlyInWRoom++;
	
		sem_post(&customers);
		
		sem_post(&mutex);
		
		sem_wait(&barbers);
		
		printf("Wlosy klienta %d sa scinane.\n", num);		
	}
	else {
		sem_post(&mutex);
		resigned++;
		printf("\tPoczekalnia pelna - klient %d rezygnuje.\n", num);
	}
}