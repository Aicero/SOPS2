void mutex_style() {
	printf("jestesmy w mutexach\n\n");
	
    pthread_t tid[numOfChairs+20];
	//pthread_t threads[numOfChairs+20];
	//int tret[numOfChairs+20];
	
	sem_init(&WRoom, 0, numOfChairs);
	sem_init(&InChair, 0, 1);
	sem_init(&Sleep, 0, 0);
	
	pthread_create(&brb, NULL, barber, NULL);
	
	int i;
	for(i=1; i<=numOfChairs+20; i++) {
		customerNumber = customerNumber + 1;
		pthread_create(&tid[i-1], NULL, customer, (void *)&customerNumber);
		//tret[i] = pthread_create(&threads[i], NULL, test, NULL);
		//printf("no");
	}
	//printf("\n");
	for(i=0; i<numOfChairs+20; i++) {
		pthread_join(tid[i], NULL);
		//tret[i] = pthread_create(&threads[i], NULL, test, NULL);
		//printf("no");
	}
	pthread_join(brb, NULL);
}

void *barber() {
	while(1) {
		sem_wait(&Sleep);
		printf("scinamy wlosy\n");
		usleep(5000*1000);
		printf("sciete\n");
		sem_post(&InChair);
	}
}

void *customer(void *nr) {
	int customerNr = *(int *)nr;
	
	printf("klient %d chce sie ostrzyc\n", customerNr);
	
	if(currentlyInWRoom < numOfChairs) {
		currentlyInWRoom = currentlyInWRoom + 1;
		sem_wait(&WRoom);
		printf("\tklient %d wchodzi do WRoomu\n", customerNr);
		
		sem_wait(&InChair);
		printf("\t\tklient %d czeka az bedzie wolne krzeslo.\n", customerNr);
		
		sem_post(&WRoom);
		
		sem_post(&Sleep);
		printf("\t\t\tklient %d budzi fryzjera\n", customerNr);
		
		sem_wait(&BusyAsHell);
		
		sem_post(&InChair);
		printf("\t\t\t\tKlient %d obsluzony!!!\n\n", customerNr);
		return 0;
		
	} else {
		resigned = resigned + 1;
		printf("Klient %d rezygnuje.", customerNr);
		pthread_exit(NULL);
	}
}