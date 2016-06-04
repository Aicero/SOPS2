void createNewCustomer(pthread_t customerThread) {
    int thrErr;
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