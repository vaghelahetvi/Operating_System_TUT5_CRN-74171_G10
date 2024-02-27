#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_STUDENTS 5

// Structure to hold student grade data
struct StudentData {
    int student_number;
    float grade;
};

// Function to bell curve a grade
void *bellcurve(void *arg) {
    struct StudentData *data = (struct StudentData *)arg;
    
    // Calculate bell-curved grade
    float bellcurved_grade = data->grade * 1.50;
    
    // Print the original grade and the bell-curved grade
    printf("Student %d: Original Grade: %.2f, Bellcurved Grade: %.2f\n", 
           data->student_number, data->grade, bellcurved_grade);
    
    // Exit the thread
    pthread_exit(NULL);
}

int main() {
    // Declare variables
    float grades[NUM_STUDENTS];
    pthread_t threads[NUM_STUDENTS];
    struct StudentData student_data[NUM_STUDENTS];

    // Prompt the professor to enter grades for each student
    printf("Professor, please enter the grades for %d students:\n", NUM_STUDENTS);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        printf("Enter the grade for Student %d: ", i + 1);
        scanf("%f", &grades[i]);
    }

    // Create threads and initialize them with student grade data
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_data[i].student_number = i + 1;
        student_data[i].grade = grades[i];
        pthread_create(&threads[i], NULL, bellcurve, (void *)&student_data[i]);
    }

    // Wait for all threads to finish execution
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}