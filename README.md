# Student Digital Twin System

## Project Overview

Student Digital Twin System is a console-based C++ OOP semester project that simulates a smart academic analytics engine. It manages Undergraduate, Postgraduate, and Scholarship student records, stores data persistently in a text file, generates report cards, predicts risk, and provides an analytics dashboard.

The project is designed as a professional multi-file C++ application with clear separation of concerns:
- `Student` handles the core student model
- `UGStudent`, `PGStudent`, and `ScholarshipStudent` provide polymorphic scoring behavior
- `DigitalTwinEngine` manages business logic and analytics
- `FileManager` handles persistent file storage and report generation
- `Utils` centralizes formatting and input helpers

## Features

- Add student records for UG, PG, and Scholarship types
- View all students
- Search student by ID
- Update student data
- Delete student records
- Sort students by performance
- Display top performers
- Generate a full report card file
- Show a system analytics dashboard
- Predict student risk and suggest improvement actions

## Project Structure

- `main.cpp`
- `Student.h`
- `Student.cpp`
- `DerivedStudents.h`
- `DerivedStudents.cpp`
- `DigitalTwinEngine.h`
- `DigitalTwinEngine.cpp`
- `FileManager.h`
- `FileManager.cpp`
- `Utils.h`
- `students.txt`
- `report.txt`

## How the System Works

### 1. Startup
The program loads existing student records from `students.txt`.

### 2. Menu Dashboard
The user interacts with a terminal menu that supports all major operations.

### 3. Add / Update / Delete
Whenever records are changed, the program saves the updated collection back to `students.txt`.

### 4. Performance Engine
Each student type has a different scoring model:
- UG students emphasize GPA, attendance, assignments, and projects
- PG students emphasize GPA, attendance, research, and assignments
- Scholarship students receive an additional scholarship bonus

### 5. Digital Twin Analytics
The engine calculates:
- performance score
- trend status: improving, declining, or stable
- risk level
- study suggestions
- analytics summary

### 6. Reporting
The system writes a detailed report to `report.txt` containing:
- all students
- top performers
- risk students
- report cards
- dashboard statistics

## OOP Concept Mapping

### Encapsulation
Student data is stored as private members inside the `Student` class and accessed through public methods.

### Inheritance
The project uses inheritance to create specialized student types:
- `UGStudent`
- `PGStudent`
- `ScholarshipStudent`

### Polymorphism
The derived classes override virtual functions such as performance calculation and type identification.

### Abstraction
`DigitalTwinEngine` exposes a clean interface while hiding the internal analytics and prediction logic.

## File Handling

### students.txt
Stores all student records permanently using a pipe-separated format.

### report.txt
Generated report file that contains the analytics dashboard and student report cards.

This ensures persistence across program runs.

## Class Design Summary

### Student
Base abstract class with common academic fields and shared behavior.

### UGStudent
Represents undergraduate students and uses UG-specific performance weighting.

### PGStudent
Represents postgraduate students and emphasizes research performance.

### ScholarshipStudent
Represents scholarship students and adds scholarship-based scoring bonus.

### DigitalTwinEngine
Controls add, search, delete, update, sorting, analytics, and risk prediction.

### FileManager
Reads and writes persistent records and report files.

### Utils
Provides reusable helper functions for input, formatting, and string utilities.

## UML / Text-Based Class Diagram

```text
+---------------------------+
|          Student          |
+---------------------------+
| - studentId               |
| - fullName                |
| - department              |
| - studyYear               |
| - gpa                     |
| - attendanceRate          |
| - assignmentScore         |
| - projectScore            |
| - researchScore           |
| - scholarshipPercent      |
| - previousPerformanceScore |
+---------------------------+
| + getType()               |
| + getProfileTag()         |
| + calculatePerformanceScore() |
| + serialize()             |
| + getTrend()              |
| + getRiskLevel()          |
| + getSuggestion()         |
+---------------------------+
            ^
            |
   +--------+---------+
   |        |         |
+--------+ +--------+ +-------------------+
| UGStudent| |PGStudent| | ScholarshipStudent |
+--------+ +--------+ +-------------------+
| UG score | | PG score | | UG bonus score   |
+--------+ +--------+ +-------------------+

+---------------------------+
|     DigitalTwinEngine     |
+---------------------------+
| - students                |
| - dataFilePath            |
| - reportFilePath          |
+---------------------------+
| + addStudent()            |
| + searchStudent()         |
| + deleteStudent()         |
| + updateStudent()         |
| + getSortedByPerformance()|
| + generateDashboard()     |
| + generateReportContent() |
+---------------------------+

+---------------------------+
|        FileManager        |
+---------------------------+
| + loadStudents()          |
| + saveStudents()          |
| + saveReport()            |
+---------------------------+
```

## Compilation and Execution

### Compile using g++
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Student.cpp DerivedStudents.cpp DigitalTwinEngine.cpp FileManager.cpp -o student_digital_twin.exe
```

### Run the program
```bash
./student_digital_twin.exe
```

On Windows PowerShell:
```powershell
.\student_digital_twin.exe
```

## Sample Flow

1. Start the program
2. Add one or more students
3. View or search a student by ID
4. Update weak records
5. Check sorted results and top performers
6. Generate `report.txt`
7. Review the analytics dashboard and risk prediction output

## Viva Questions and Answers

### 1. What is the purpose of the Student Digital Twin System?
It simulates an academic analytics system that monitors student performance and predicts risk.

### 2. Why is `Student` abstract?
Because it defines a common interface and forces derived classes to implement scoring behavior.

### 3. Where is polymorphism used?
In overridden virtual functions such as `calculatePerformanceScore()` and `getType()`.

### 4. How is data stored permanently?
All records are written to and loaded from `students.txt`.

### 5. What is the role of `DigitalTwinEngine`?
It contains the business logic for analytics, sorting, risk prediction, and reporting.

### 6. How does the system identify risk students?
It checks low performance score, low attendance, low GPA, and declining trend.

### 7. Why is file handling important here?
It ensures the project keeps data across multiple program runs.

### 8. What makes `ScholarshipStudent` different from `UGStudent`?
It inherits undergraduate behavior but adds a scholarship bonus to the score.

### 9. What is the use of the report file?
It provides a printable academic summary and analytics snapshot.

### 10. How does the trend prediction work?
The current score is compared with the previous score to decide whether the student is improving, stable, or declining.

## Notes

- The project is console-based and uses simple text files for persistence.
- The code is structured for semester submission, viva explanation, and demonstration.
- The project was verified to compile and run successfully in the workspace.
