void logger() {
	printf("Res: %d WRoom: %d/%d [in: %d]\n", resigned, currentlyInWRoom, numOfChairs, custInChair);
	if (debug == 1) {
		printf("Res: tu bedzie kolejka\n");
		printf("WRoom: tu bedzie kolejka\n");
	}
	printf("\n");
}