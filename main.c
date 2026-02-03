/******************************************************************************
 * student-management-application-c
 *
 * Console-based student management system using a singly linked list.
 * Supports CRUD operations with basic input validation and duplicate ID check.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ID_LENGTH       20
#define MAX_NAME_LENGTH     50
#define MAX_EMAIL_LENGTH    100
#define MAX_COURSE_LENGTH   50
#define MIN_AGE             16
#define MAX_AGE             120

/******************************************************************************
 * Structure: Student
 ******************************************************************************/
typedef struct Student
{
    char studentId[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    int  age;
    char email[MAX_EMAIL_LENGTH];
    char course[MAX_COURSE_LENGTH];
    struct Student* next;
} Student;

/******************************************************************************
 * Global Variables
 ******************************************************************************/
Student* head         = NULL;
int      studentCount = 0;

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
void        showMenu(void);
void        captureStudent(void);
int         isValidAgeInput(const char* input, int* outAge);
int         isDuplicateId(const char* id);
int         isBasicEmailFormat(const char* email);
void        searchStudent(void);
void        deleteStudent(void);
void        printStudentReport(void);
void        exitApplication(void);
void        saveStudent(Student* student);
Student*    searchStudentById(const char* studentId);
int         deleteStudentById(const char* studentId);
void        freeAllStudents(void);
void        trimNewline(char* str);
int         getYesNoInput(const char* prompt);

/******************************************************************************
 * Function: main
 ******************************************************************************/
int main(void)
{
    char initialChoice[10];

    printf("STUDENT MANAGEMENT APPLICATION\n");
    printf("=================================================\n");
    printf("Enter 1 to launch menu, any other key to exit\n");
    printf("\n");

    if (fgets(initialChoice, sizeof(initialChoice), stdin) == NULL)
    {
        printf("Error reading input. Exiting.\n");
        return 1;
    }
    trimNewline(initialChoice);

    if (strcmp(initialChoice, "1") != 0)
    {
        printf("Exiting application. Goodbye.\n");
        return 0;
    }

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
            break;
        }
        else
        {
            printf("Invalid option. Please enter 1-5.\n");
        }
    }

    return 0;
}

/******************************************************************************
 * Function: showMenu
 ******************************************************************************/
void showMenu(void)
{
    printf("\n");
    printf("MENU\n");
    printf("-------------------------------------------------\n");
    printf("1) Capture a new student\n");
    printf("2) Search for a student\n");
    printf("3) Delete a student\n");
    printf("4) Print student report\n");
    printf("5) Exit application\n");
    printf("-------------------------------------------------\n");
    printf("Enter your choice (1-5): ");
}

/******************************************************************************
 * Function: captureStudent
 ******************************************************************************/
void captureStudent(void)
{
    Student* newStudent = malloc(sizeof(Student));
    if (newStudent == NULL)
    {
        printf("Memory allocation failed. Cannot add student.\n");
        return;
    }

    printf("\nCAPTURE NEW STUDENT\n");
    printf("-------------------------------------------------\n");

    /* Student ID */
    int valid = 0;
    while (!valid)
    {
        printf("Student ID     : ");
        if (fgets(newStudent->studentId, sizeof(newStudent->studentId), stdin) == NULL)
        {
            free(newStudent);
            return;
        }
        trimNewline(newStudent->studentId);

        size_t len = strlen(newStudent->studentId);
        if (len == 0 || len >= MAX_ID_LENGTH - 1)
        {
            printf("Error: ID must be 1 to %d characters.\n", MAX_ID_LENGTH - 2);
            continue;
        }

        if (isDuplicateId(newStudent->studentId))
        {
            printf("Error: Student ID '%s' already exists.\n", newStudent->studentId);
            continue;
        }

        valid = 1;
    }

    /* Name */
    valid = 0;
    while (!valid)
    {
        printf("Full name      : ");
        if (fgets(newStudent->name, sizeof(newStudent->name), stdin) == NULL)
        {
            free(newStudent);
            return;
        }
        trimNewline(newStudent->name);

        if (strlen(newStudent->name) < 2)
        {
            printf("Error: Name must be at least 2 characters.\n");
            continue;
        }
        valid = 1;
    }

    /* Age */
    char ageInput[16];
    int  age;
    valid = 0;
    while (!valid)
    {
        printf("Age            : ");
        if (fgets(ageInput, sizeof(ageInput), stdin) == NULL)
        {
            free(newStudent);
            return;
        }
        trimNewline(ageInput);

        if (isValidAgeInput(ageInput, &age))
        {
            newStudent->age = age;
            valid = 1;
        }
        else
        {
            printf("Error: Age must be between %d and %d.\n", MIN_AGE, MAX_AGE);
        }
    }

    /* Email */
    valid = 0;
    while (!valid)
    {
        printf("Email          : ");
        if (fgets(newStudent->email, sizeof(newStudent->email), stdin) == NULL)
        {
            free(newStudent);
            return;
        }
        trimNewline(newStudent->email);

        if (strlen(newStudent->email) < 5 || !isBasicEmailFormat(newStudent->email))
        {
            printf("Warning: Please enter a valid-looking email (example@domain.com).\n");
            // Note: still accept — strict validation usually done server-side
        }
        valid = 1;
    }

    /* Course */
    printf("Course         : ");
    if (fgets(newStudent->course, sizeof(newStudent->course), stdin) == NULL)
    {
        free(newStudent);
        return;
    }
    trimNewline(newStudent->course);

    newStudent->next = NULL;
    saveStudent(newStudent);

    printf("\nStudent successfully added.\n");
}

/******************************************************************************
 * Returns 1 if input represents valid age in range, stores result in *outAge
 ******************************************************************************/
int isValidAgeInput(const char* input, int* outAge)
{
    if (input == NULL || *input == '\0') return 0;

    char* end;
    long  val = strtol(input, &end, 10);

    if (end == input || *end != '\0') return 0;           /* not pure number */
    if (val < MIN_AGE || val > MAX_AGE) return 0;

    *outAge = (int)val;
    return 1;
}

/******************************************************************************
 * Very basic email format check (contains @ and . after it)
 ******************************************************************************/
int isBasicEmailFormat(const char* email)
{
    const char* at = strchr(email, '@');
    if (!at || at == email) return 0;

    const char* dot = strrchr(at, '.');
    if (!dot || dot <= at + 1 || dot[1] == '\0') return 0;

    return 1;
}

/******************************************************************************
 * Checks whether the ID already exists in the list
 ******************************************************************************/
int isDuplicateId(const char* id)
{
    Student* current = head;
    while (current != NULL)
    {
        if (strcmp(current->studentId, id) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

/* ────────────────────────────────────────────────────────────────────────── */
/* Remaining functions (only minor formatting & message improvements)        */
/* ────────────────────────────────────────────────────────────────────────── */

void searchStudent(void)
{
    char id[MAX_ID_LENGTH];
    printf("\nSEARCH STUDENT\n");
    printf("Enter student ID: ");
    if (fgets(id, sizeof(id), stdin) == NULL) return;
    trimNewline(id);

    Student* s = searchStudentById(id);
    printf("-------------------------------------------------\n");

    if (s != NULL)
    {
        printf("ID     : %s\n", s->studentId);
        printf("Name   : %s\n", s->name);
        printf("Age    : %d\n",  s->age);
        printf("Email  : %s\n", s->email);
        printf("Course : %s\n", s->course);
    }
    else
    {
        printf("No student found with ID: %s\n", id);
    }
    printf("-------------------------------------------------\n");
}

void deleteStudent(void)
{
    char id[MAX_ID_LENGTH];
    printf("\nDELETE STUDENT\n");
    printf("Enter student ID: ");
    if (fgets(id, sizeof(id), stdin) == NULL) return;
    trimNewline(id);

    if (!getYesNoInput("Confirm deletion? (y/n): "))
    {
        printf("Deletion cancelled.\n");
        return;
    }

    int success = deleteStudentById(id);
    printf("-------------------------------------------------\n");
    if (success)
    {
        printf("Student %s was deleted successfully.\n", id);
    }
    else
    {
        printf("Student %s not found.\n", id);
    }
    printf("-------------------------------------------------\n");
}

void printStudentReport(void)
{
    if (studentCount == 0)
    {
        printf("\nNo students registered yet.\n");
        return;
    }

    printf("\nALL STUDENTS (%d)\n", studentCount);
    printf("=================================================\n");

    Student* cur = head;
    int      idx = 1;

    while (cur != NULL)
    {
        printf("Student #%d\n", idx++);
        printf("ID     : %s\n", cur->studentId);
        printf("Name   : %s\n", cur->name);
        printf("Age    : %d\n",  cur->age);
        printf("Email  : %s\n", cur->email);
        printf("Course : %s\n", cur->course);
        printf("-------------------------------------------------\n");
        cur = cur->next;
    }
}

void exitApplication(void)
{
    freeAllStudents();
    printf("\nAll data cleared. Goodbye.\n");
}

void saveStudent(Student* student)
{
    student->next = head;
    head = student;
    studentCount++;
}

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

int deleteStudentById(const char* studentId)
{
    Student* curr = head;
    Student* prev = NULL;

    while (curr != NULL && strcmp(curr->studentId, studentId) != 0)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) return 0;

    if (prev == NULL)
    {
        head = curr->next;
    }
    else
    {
        prev->next = curr->next;
    }

    free(curr);
    studentCount--;
    return 1;
}

void freeAllStudents(void)
{
    Student* curr = head;
    while (curr != NULL)
    {
        Student* temp = curr;
        curr = curr->next;
        free(temp);
    }
    head = NULL;
    studentCount = 0;
}

void trimNewline(char* str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

int getYesNoInput(const char* prompt)
{
    char buf[10];
    while (1)
    {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
        trimNewline(buf);

        if (strlen(buf) == 1)
        {
            char ch = (char)tolower(buf[0]);
            if (ch == 'y') return 1;
            if (ch == 'n') return 0;
        }
        printf("Please enter y or n.\n");
    }
}