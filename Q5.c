#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_STUDENTS 10

// Global variables
float total_grade = 0;
float total_bellcurve = 0;
int students_ready = 0; // Counter to track the number of students ready
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for mutual exclusion
pthread_cond_t cond_ready = PTHREAD_COND_INITIALIZER; // Condition variable for synchronization

// Function to read grades from file
void *read_grades(void *arg) {
    (void)arg; // Suppress unused parameter warning
    
    FILE *file = fopen("grades.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file 'grades.txt'\n");
        exit(EXIT_FAILURE);
    }

    // Read grades from file and calculate total grade
    float grade;
    for (int i = 0; i < NUM_STUDENTS; i++) {
        fscanf(file, "%f", &grade);
        
        // Add the grade to total grade with mutual exclusion
        pthread_mutex_lock(&mutex);
        total_grade += grade;
        pthread_mutex_unlock(&mutex);

        // Signal that a student is ready
        pthread_mutex_lock(&mutex);
        students_ready++;
        pthread_cond_signal(&cond_ready);
        pthread_mutex_unlock(&mutex);
    }

    fclose(file);
    pthread_exit(NULL);
}

// Function to bell curve a grade and save it to file
void *save_bellcurve(void *arg) {
    float grade = *(float *)arg;
    float bellcurved_grade = grade * 1.50;
    
    // Add the bellcurved grade to total bellcurve with mutual exclusion
    pthread_mutex_lock(&mutex);
    total_bellcurve += bellcurved_grade;
    pthread_mutex_unlock(&mutex);

    // Save the bellcurved grade to file
    FILE *file = fopen("bellcurve.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file 'bellcurve.txt'\n");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%.2f\n", bellcurved_grade);
    fclose(file);

    pthread_exit(NULL);
}

int main() {
    pthread_t read_thread;
    pthread_t save_threads[NUM_STUDENTS];
    float grades[NUM_STUDENTS];

    // Create thread to read grades from file
    if (pthread_create(&read_thread, NULL, read_grades, NULL) != 0) {
        fprintf(stderr, "Error creating read thread\n");
        exit(EXIT_FAILURE);
    }

    // Wait for all grades to be read before continuing
    pthread_mutex_lock(&mutex);
    while (students_ready < NUM_STUDENTS) {
        pthread_cond_wait(&cond_ready, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    // Read grades from file
    FILE *file = fopen("grades.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file 'grades.txt'\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NUM_STUDENTS; i++) {
        fscanf(file, "%f", &grades[i]);
    }
    fclose(file);

    // Create threads to bell curve and save grades
    for (int i = 0; i < NUM_STUDENTS; i++) {
        if (pthread_create(&save_threads[i], NULL, save_bellcurve, (void *)&grades[i]) != 0) {
            fprintf(stderr, "Error creating save thread for student %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(save_threads[i], NULL);
    }

    // Calculate class average before bell curve
    float class_average_before = total_grade / NUM_STUDENTS;

    // Calculate class average after bell curve
    float class_average_after = total_bellcurve / NUM_STUDENTS;

    // Print results
    printf("Total Grade: %.2f\n", total_grade);
    printf("Class Average before bell curve: %.2f\n", class_average_before);
    printf("Total Bell-curved Grade: %.2f\n", total_bellcurve);
    printf("Class Average after bell curve: %.2f\n", class_average_after);

    return 0;
}
