#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_STUDENTS 10

// Global variables
float total_grade = 0; // Total grade variable to store the sum of all grades
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for mutual exclusion

// Function to add grade to total_grade with mutual exclusion
void class_total(float grade) {
    // Lock the mutex to ensure mutual exclusion
    pthread_mutex_lock(&mutex);

    // Add the grade to total_grade
    total_grade += grade;

    // Unlock the mutex after updating total_grade
    pthread_mutex_unlock(&mutex);
}

// Function for each student thread
void *student_thread(void *arg) {
    // Get the grade from the argument
    float grade = *(float *)arg;

    // Add the grade to the total with mutual exclusion
    class_total(grade);

    // Exit the thread
    pthread_exit(NULL);
}

int main() {
    float grades[NUM_STUDENTS]; // Array to store the grades of each student
    pthread_t threads[NUM_STUDENTS]; // Array to store thread IDs

    // Prompt the professor for grades
    printf("Professor, please enter the grades for %d students:\n", NUM_STUDENTS);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        printf("Grade for Student %d: ", i + 1);
        scanf("%f", &grades[i]);
    }

    // Create threads for each student
    for (int i = 0; i < NUM_STUDENTS; i++) {
        // Create thread for each student, passing the grade as argument
        if (pthread_create(&threads[i], NULL, student_thread, (void *)&grades[i]) != 0) {
            fprintf(stderr, "Error creating thread for Student %d\n", i + 1);
            // Handle error if thread creation fails
            // Exiting here might not be appropriate for a real application
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the total grade
    printf("Total Grade: %.2f\n", total_grade);

    return 0;
}
