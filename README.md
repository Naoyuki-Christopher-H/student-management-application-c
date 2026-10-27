# Student Management Application

Console-based student record management system using a singly linked list.
Implements basic CRUD operations (Create, Read, Delete — Update not yet
implemented) with input validation and duplicate prevention.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Requirements](#requirements)
- [Build and Run](#build-and-run)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Future Improvements](#future-improvements-possible-roadmap)
- [Contributing](#contributing)

## Description

This project is a menu-driven console application written in standard C that
allows users to:

- Add new student records (with validation for ID uniqueness, name length,
  age range, and basic email format)
- Search for a student by ID
- Delete a student record (with confirmation prompt)
- View a formatted report of all registered students
- Exit the program cleanly (with proper memory deallocation)

Data is stored in memory using a singly linked list (insertion at head).
No file persistence is implemented in the current version.

The application demonstrates core C concepts including:

- Dynamic memory allocation (`malloc` / `free`)
- Singly linked list operations (insert, search, delete)
- Input handling with `fgets` and string cleaning
- Basic data validation (age parsing with `strtol`, duplicate ID check,
  minimal email sanity)
- Modular program structure with separate functions

## Features

- Unique student ID enforcement (prevents duplicates)
- Age validation (must be between 16 and 120)
- Name length check (minimum 2 characters)
- Basic email format validation (contains `@` and a `.` after it; shows
  warning but accepts)
- Confirmation before deletion
- Clean memory management on exit (no leaks)
- User-friendly menu interface with clear prompts and separators

**Current limitations:**

- No update/edit functionality
- No save/load to file
- In-memory storage only (data lost on exit)
- No sorting or filtering in reports

## Requirements

- C compiler (GCC, Clang, MSVC, TCC, etc.)
- Standard C library (C99 or later recommended)
- No external dependencies

**Tested on:**

- Linux / macOS (gcc/clang)
- Windows (MinGW / MSVC)

## Build and Run

1. Clone the repository:

   ```bash
   git clone https://github.com/Naoyuki-Christopher-H/student-management-application-c.git
   cd student-management-application-c
   ```

2. Compile the program:

   ```bash
   gcc -o student_manager main.c -Wall -std=c99
   ```

   (Use `-Wall` to enable warnings; `-std=c99` or `-std=c11` for better
   compatibility)

3. Run the application:

   ```bash
   ./student_manager
   ```

   On Windows (with MinGW or similar):

   ```bash
   student_manager.exe
   ```

## Usage

After starting the program:

1. Enter `1` to launch the menu (any other input exits immediately)
2. Choose from the menu options:
   - `1` → Add a new student (guided input with validation)
   - `2` → Search for a student by ID
   - `3` → Delete a student (requires confirmation)
   - `4` → Print report of all students
   - `5` → Exit (frees all memory)

All inputs are line-based (`fgets`). Press Enter after each field.

**Example workflow:**

- Add several students
- Search by ID to verify
- Delete one (confirm with `y`)
- View updated report
- Exit

## Project Structure

```
student-management-application-c/
└── main.c          # Complete source code (single file implementation)
```

(The project is currently implemented in a single source file for simplicity.)

## Future Improvements (Possible Roadmap)

- Add "Update student" functionality (new menu option)
- Implement file I/O (save/load students to `.txt` or `.dat`)
- Add sorting options in report (by name, age, ID)
- Strengthen input validation (stronger email check, course list)
- Add basic statistics (count by course, average age, etc.)
- Split into multiple files (student.h, student.c, utils.c, main.c)

## Contributing

Contributions are welcome.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/add-update`)
3. Commit your changes (`git commit -m "Add student update feature"`)
4. Push to the branch (`git push origin feature/add-update`)
5. Open a pull request

Please ensure code follows the existing style (Allman bracing, detailed
comments, clear variable names).

---

END OF DOCUMENT

---
