#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define NUM_STUDENTS 5
// Define a struct to hold student information
struct Student {
    char name[50];
    int student_id;
    float grade;
};
// Function to bell curve a grade
void *bellcurve(void *arg) {
    // Cast the argument back to a Student pointer
    struct Student *student = (struct Student *)arg;
    
    // Calculate the bell-curved grade
    float bellcurved_grade = student->grade * 1.50;
    
    // Print the student's information along with the bell-curved grade
    printf("Name: %s, ID: %d, Bell-curved Grade: %.2f\n", 
           student->name, student->student_id, bellcurved_grade);
    
    // Exit the thread
    pthread_exit(NULL);
}
int main() {
    // Declare variables
    struct Student students[NUM_STUDENTS];
    pthread_t threads[NUM_STUDENTS];

    // Prompt the professor to input information for each student
    printf("Professor, please provide information for %d students:\n", NUM_STUDENTS);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        printf("Student %d:\n", i + 1);
        printf("Name: ");
        scanf("%s", students[i].name);
        printf("ID: ");
        scanf("%d", &students[i].student_id);
        printf("Grade: ");
        scanf("%f", &students[i].grade);
    }

    // Create threads and initialize them with student data
    printf("Bell curving grades...\n");
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_create(&threads[i], NULL, bellcurve, (void *)&students[i]);
    }

    // Wait for all threads to finish execution
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
