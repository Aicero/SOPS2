#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int mutex_style();

volatile int numOfChairs;
volatile int debug = 0;
volatile int condv = 0;

pthread_mutex_t gabinet = PTHREAD_MUTEX_INITIALIZER;

#include "mutex_style.c"