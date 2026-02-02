/******************************************************************************
 * student-management-application-c
 *
 * This project implements a console-based student management system in C.
 * It allows users to perform CRUD operations on student records through
 * a menu-driven interface. The program uses dynamic memory allocation for
 * student data and a linked list for storage.
 *
 * Main functionalities:
 * - Capture new student details with validation
 * - Search for students by ID
 * - Delete student records with confirmation
 * - Print report of all students
 * - Exit the application cleanly
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ID_LENGTH 20
#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 100
#define MAX_COURSE_LENGTH 50
#define MIN_AGE 16
#define MAX_AGE 120

/******************************************************************************
 * Structure: Student
 *
 * Represents a student record with the following fields:
 * - studentId: Unique identifier for the student
 * - name: Student's full name
 * - age: Student's age (must be >= MIN_AGE)
 * - email: Student's email address
 * - course: Course the student is enrolled in
 * - next: Pointer to the next student in the linked list
 ******************************************************************************/
typedef struct Student {
    char studentId[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    int age;
    char email[MAX_EMAIL_LENGTH];
    char course[MAX_COURSE_LENGTH];
    struct Student* next;
} Student;

/******************************************************************************
 * Global Variables
 *
 * - head: Pointer to the first student in the linked list
 * - studentCount: Number of students currently in the system
 ******************************************************************************/
Student* head = NULL;
int studentCount = 0;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
void showMenu(void);
void captureStudent(void);
int validateAgeInput(const char* input);
void searchStudent(void);
void deleteStudent(void);
void printStudentReport(void);
void exitApplication(void);
void saveStudent(Student* student);
Student* searchStudentById(const char* studentId);
int deleteStudentById(const char* studentId);
void freeAllStudents(void);
void trimNewline(char* str);
int getYesNoInput(const char* prompt);

/******************************************************************************
 * Function: main
 *
 * Entry point of the application. Displays initial greeting and launches
 * the main menu loop. Continuously processes user input until exit is chosen.
 *
 * Returns: 0 on successful execution
 ******************************************************************************/
int main(void)
{
    char initialChoice[10];

    /* Display application header */
    printf("STUDENT MANAGEMENT APPLICATION\n");
    printf("*************************************************\n");
    printf("Enter (1) to launch menu or any other key to exit\n");
    printf("    \n");

    /* Get initial user choice */
    if (fgets(initialChoice, sizeof(initialChoice), stdin) == NULL)
    {
        printf("Error reading input. Exiting application.\n");
        return 1;
    }

    trimNewline(initialChoice);

    /* Check if user wants to launch the application */
    if (strcmp(initialChoice, "1") != 0)
    {
        printf("Exiting the application. Goodbye!\n");
        return 0;
    }

    /* Main application loop */
    while (1)
    {
        showMenu();

        char choice[10];
        if (fgets(choice, sizeof(choice), stdin) == NULL)
        {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        trimNewline(choice);

        /* Process user menu choice */
        if (strcmp(choice, "1") == 0)
        {
            captureStudent();
        }
        else if (strcmp(choice, "2") == 0)
        {
            searchStudent();
        }
        else if (strcmp(choice, "3") == 0)
        {
            deleteStudent();
        }
        else if (strcmp(choice, "4") == 0)
        {
            printStudentReport();
        }
        else if (strcmp(choice, "5") == 0)
        {
            exitApplication();
            break; /* Exit the loop after cleanup */
        }
        else
        {
            printf("Invalid choice! Please enter a valid menu option.\n");
        }
    }

    return 0;
}

/******************************************************************************
 * Function: showMenu
 *
 * Displays the main menu with all available options to the user.
 * Each option corresponds to a specific operation in the application.
 ******************************************************************************/
void showMenu(void)
{
    printf("    \n");
    printf("Please select one of the following menu items:\n");
    printf("(1) Capture a new student.\n");
    printf("(2) Search for a student.\n");
    printf("(3) Delete a student.\n");
    printf("(4) Print student report.\n");
    printf("(5) Exit Application.\n");
    printf("    \n");
}

/******************************************************************************
 * Function: captureStudent
 *
 * Guides the user through entering details for a new student.
 * Validates input and saves the student record to the system.
 * Uses dynamic memory allocation for the new student structure.
 ******************************************************************************/
void captureStudent(void)
{
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent == NULL)
    {
        printf("Memory allocation failed. Cannot add new student.\n");
        return;
    }

    printf("CAPTURE A NEW STUDENT\n");
    printf("*****************************\n");

    /* Get student ID */
    printf("Enter the student id: ");
    if (fgets(newStudent->studentId, sizeof(newStudent->studentId), stdin) == NULL)
    {
        printf("Error reading input.\n");
        free(newStudent);
        return;
    }
    trimNewline(newStudent->studentId);

    /* Get student name */
    printf("Enter the student name: ");
    if (fgets(newStudent->name, sizeof(newStudent->name), stdin) == NULL)
    {
        printf("Error reading input.\n");
        free(newStudent);
        return;
    }
    trimNewline(newStudent->name);

    /* Get and validate student age */
    char ageInput[10];
    int validAge = 0;
    while (!validAge)
    {
        printf("Enter the student age: ");
        if (fgets(ageInput, sizeof(ageInput), stdin) == NULL)
        {
            printf("Error reading input.\n");
            free(newStudent);
            return;
        }
        trimNewline(ageInput);

        newStudent->age = validateAgeInput(ageInput);
        if (newStudent->age >= MIN_AGE)
        {
            validAge = 1;
        }
        else
        {
            printf("You have entered an incorrect student age!!!\n");
            printf("Please re-enter the student age >>\n");
        }
    }

    /* Get student email */
    printf("Enter the student email: ");
    if (fgets(newStudent->email, sizeof(newStudent->email), stdin) == NULL)
    {
        printf("Error reading input.\n");
        free(newStudent);
        return;
    }
    trimNewline(newStudent->email);

    /* Get student course */
    printf("Enter the student course: ");
    if (fgets(newStudent->course, sizeof(newStudent->course), stdin) == NULL)
    {
        printf("Error reading input.\n");
        free(newStudent);
        return;
    }
    trimNewline(newStudent->course);

    /* Initialize the next pointer and save the student */
    newStudent->next = NULL;
    saveStudent(newStudent);

    printf("Student details have been successfully saved.\n");
}

/******************************************************************************
 * Function: validateAgeInput
 *
 * Validates that the age input is a positive integer within the valid range.
 *
 * Parameters:
 *   input - String containing the user's age input
 *
 * Returns:
 *   Validated age as integer, or -1 if input is invalid
 ******************************************************************************/
int validateAgeInput(const char* input)
{
    /* Check if input is empty */
    if (input == NULL || strlen(input) == 0)
    {
        return -1;
    }

    /* Check if all characters are digits */
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (!isdigit(input[i]))
        {
            return -1;
        }
    }

    /* Convert to integer and validate range */
    int age = atoi(input);
    if (age < MIN_AGE || age > MAX_AGE)
    {
        return -1;
    }

    return age;
}

/******************************************************************************
 * Function: searchStudent
 *
 * Prompts the user for a student ID and searches for the student in the system.
 * Displays the student's details if found, or an appropriate message if not.
 ******************************************************************************/
void searchStudent(void)
{
    char studentId[MAX_ID_LENGTH];

    printf("Enter the student id to search: ");
    if (fgets(studentId, sizeof(studentId), stdin) == NULL)
    {
        printf("Error reading input.\n");
        return;
    }
    trimNewline(studentId);

    Student* student = searchStudentById(studentId);

    printf("---------------------------------------------\n");
    if (student != NULL)
    {
        printf("STUDENT ID: %s\n", student->studentId);
        printf("STUDENT NAME: %s\n", student->name);
        printf("STUDENT AGE: %d\n", student->age);
        printf("STUDENT EMAIL: %s\n", student->email);
        printf("STUDENT COURSE: %s\n", student->course);
    }
    else
    {
        printf("Student with Student Id: %s was not found!\n", studentId);
    }
    printf("---------------------------------------------\n");
}

/******************************************************************************
 * Function: deleteStudent
 *
 * Prompts the user for a student ID and asks for confirmation before deleting.
 * Removes the student from the system if confirmed and found.
 * Provides feedback on the operation's success or failure.
 ******************************************************************************/
void deleteStudent(void)
{
    char studentId[MAX_ID_LENGTH];

    printf("Enter the student id to delete: ");
    if (fgets(studentId, sizeof(studentId), stdin) == NULL)
    {
        printf("Error reading input.\n");
        return;
    }
    trimNewline(studentId);

    /* Ask for confirmation */
    if (getYesNoInput("Are you sure you want to delete this student? (y/n): "))
    {
        int success = deleteStudentById(studentId);

        printf("---------------------------------------------\n");
        if (success)
        {
            printf("Student with Student Id: %s WAS deleted!\n", studentId);
        }
        else
        {
            printf("Student with Student Id: %s was not found!\n", studentId);
        }
        printf("---------------------------------------------\n");
    }
    else
    {
        printf("Deletion cancelled.\n");
    }
}

/******************************************************************************
 * Function: printStudentReport
 *
 * Generates and displays a report of all students in the system.
 * Lists each student's details in a formatted output.
 * Shows a message if no students are found.
 ******************************************************************************/
void printStudentReport(void)
{
    if (studentCount == 0)
    {
        printf("No students found in the system.\n");
        return;
    }

    Student* current = head;
    int count = 1;

    while (current != NULL)
    {
        printf("STUDENT %d\n", count);
        printf("------------------------------\n");
        printf("STUDENT ID: %s\n", current->studentId);
        printf("STUDENT NAME: %s\n", current->name);
        printf("STUDENT AGE: %d\n", current->age);
        printf("STUDENT EMAIL: %s\n", current->email);
        printf("STUDENT COURSE: %s\n", current->course);
        printf("------------------------------\n");

        current = current->next;
        count++;
    }
}

/******************************************************************************
 * Function: exitApplication
 *
 * Cleans up all allocated memory and exits the application.
 * Displays a farewell message to the user.
 ******************************************************************************/
void exitApplication(void)
{
    freeAllStudents();
    printf("Exiting the application. Goodbye!\n");
}

/******************************************************************************
 * Function: saveStudent
 *
 * Adds a new student to the linked list at the beginning.
 * Updates the student count.
 *
 * Parameters:
 *   student - Pointer to the Student structure to be added
 ******************************************************************************/
void saveStudent(Student* student)
{
    if (head == NULL)
    {
        head = student;
    }
    else
    {
        student->next = head;
        head = student;
    }
    studentCount++;
}

/******************************************************************************
 * Function: searchStudentById
 *
 * Searches for a student by ID in the linked list.
 * Uses linear search through the list.
 *
 * Parameters:
 *   studentId - ID of the student to search for
 *
 * Returns:
 *   Pointer to the found student, or NULL if not found
 ******************************************************************************/
Student* searchStudentById(const char* studentId)
{
    Student* current = head;

    while (current != NULL)
    {
        if (strcmp(current->studentId, studentId) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/******************************************************************************
 * Function: deleteStudentById
 *
 * Deletes a student from the linked list by ID.
 * Handles three cases: deleting the head, middle, or tail node.
 *
 * Parameters:
 *   studentId - ID of the student to delete
 *
 * Returns:
 *   1 if student was successfully deleted, 0 if not found
 ******************************************************************************/
int deleteStudentById(const char* studentId)
{
    Student* current = head;
    Student* previous = NULL;

    /* Search for the student */
    while (current != NULL && strcmp(current->studentId, studentId) != 0)
    {
        previous = current;
        current = current->next;
    }

    /* Student not found */
    if (current == NULL)
    {
        return 0;
    }

    /* Student found - delete from list */
    if (previous == NULL)
    {
        /* Deleting the head */
        head = current->next;
    }
    else
    {
        /* Deleting middle or tail */
        previous->next = current->next;
    }

    /* Free memory and update count */
    free(current);
    studentCount--;

    return 1;
}

/******************************************************************************
 * Function: freeAllStudents
 *
 * Frees all dynamically allocated memory for student records.
 * Traverses the linked list and frees each node.
 ******************************************************************************/
void freeAllStudents(void)
{
    Student* current = head;
    Student* next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    head = NULL;
    studentCount = 0;
}

/******************************************************************************
 * Function: trimNewline
 *
 * Removes trailing newline character from a string.
 * Commonly used after fgets() to clean up input.
 *
 * Parameters:
 *   str - String to be modified
 ******************************************************************************/
void trimNewline(char* str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

/******************************************************************************
 * Function: getYesNoInput
 *
 * Prompts the user for yes/no confirmation and validates the input.
 *
 * Parameters:
 *   prompt - The prompt message to display
 *
 * Returns:
 *   1 for yes (y/Y), 0 for no (n/N)
 ******************************************************************************/
int getYesNoInput(const char* prompt)
{
    char input[10];

    while (1)
    {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Error reading input.\n");
            continue;
        }

        trimNewline(input);

        if (strlen(input) == 1)
        {
            char ch = tolower(input[0]);
            if (ch == 'y')
            {
                return 1;
            }
            else if (ch == 'n')
            {
                return 0;
            }
        }

        printf("Please enter 'y' for yes or 'n' for no.\n");
    }
}