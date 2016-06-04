void prepareResClients() {
	resClients = (int *)malloc(resClientsSize * sizeof(int));
	if (resClients == NULL) {
		fprintf(stderr, "error allocating memory for resigned clients list");
		exit(EXIT_FAILURE);
	}
}

void addResignedClient(int num) {
	if (resigned == resClientsSize) {
		resClientsSize += 10;
		resClients = realloc(resClients, resClientsSize * sizeof(int));
		if (resClients == NULL) {
			fprintf(stderr, "error reallocating memory for resigned clients list");
			exit(EXIT_FAILURE);
		}
	}

	resClients[resigned] = num;
	resigned++; // zwiekszenie licznika osob, ktore zrezygnowaly
}