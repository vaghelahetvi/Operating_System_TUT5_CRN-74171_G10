#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Include the header file for sleep function

// Function to print "Hello, world!"
void *say_hello(void *arg) {
    (void)arg; // Suppress unused parameter warning
    // Generate a random sleep duration between 1 and 3 seconds
    int sleep_duration = rand() % 3 + 1;
    sleep(sleep_duration);
    printf("Hello, world!\n");
    pthread_exit(NULL);
}

// Function to print "Goodbye!"
void *say_goodbye(void *arg) {
    (void)arg; // Suppress unused parameter warning
    // Generate a random sleep duration between 1 and 3 seconds
    int sleep_duration = rand() % 3 + 1;
    sleep(sleep_duration);
    printf("Goodbye!\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2; // Declare two threads
    int ret1, ret2;

    // Seed the random number generator
    srand(time(NULL));

    // Create thread for saying hello
    ret1 = pthread_create(&thread1, NULL, say_hello, NULL);
    if (ret1) {
        fprintf(stderr, "Error - Could not create thread for saying hello. Error code: %d\n", ret1);
        exit(EXIT_FAILURE);
    }

    // Create thread for saying goodbye
    ret2 = pthread_create(&thread2, NULL, say_goodbye, NULL);
    if (ret2) {
        fprintf(stderr, "Error - Could not create thread for saying goodbye. Error code: %d\n", ret2);
        exit(EXIT_FAILURE);
    }

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
