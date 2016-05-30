#include "fryzjer.h"

int main(int argc, char* argv[]) {
	if(argc == 1) {
		printf("Uzycie: %s [liczba_krzesel]\n", argv[0]);
		printf("\topcjonalne parametry: [-debug] [-zmienne]\n\n");
		exit(EXIT_FAILURE);
	}
	int i = 2;
	
	numOfChairs = atoi(argv[1]);
	if(numOfChairs < 1) {
		printf("Liczba krzesel musi byc wieksza niz 0.\n\n");
		exit(EXIT_FAILURE);
	}

	for(i; i<argc; i++) {
		if(strcmp(argv[i], "-debug") == 0) {
			debug = 1;
		} else if (strcmp(argv[i], "-zmienne") == 0) { 
			condv = 1;
		} else {
			printf("\tBlad: Dostepne parametry: [-debug] [-zmienne]\n");
		}
	}
	
	printf("Liczba krzesel: %d\n", numOfChairs);
	
	if(condv == 0) { // bez wykorzystania zmiennych warunkowych (tylko mutexy/semafory)
		mutex_style();
	} else {
		// tu wywołanie funkcji do zmiennych warunkowych
	}
	
	printf("Podsumowanie: debug %d | zmienne: %d.\n", debug, condv);
	return 0;
}

