#include "DigitalTwinEngine.h"
#include "FileManager.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

namespace {

constexpr const char* kStudentsFile = "students.txt";
constexpr const char* kReportFile = "report.txt";

void printBanner() {
    Utils::printLine('=', 80);
    cout << "                    STUDENT DIGITAL TWIN SYSTEM\n";
    cout << "             Smart academic analytics and risk dashboard\n";
    Utils::printLine('=', 80);
}

void printMenu() {
    cout << "\n[1] Add Student\n"
              << "[2] View All Students\n"
              << "[3] Search Student by ID\n"
              << "[4] Delete Student\n"
              << "[5] Update Student\n"
              << "[6] Sort Students by Performance\n"
              << "[7] Top Performers List\n"
              << "[8] Generate Report Card File\n"
              << "[9] System Analytics Dashboard\n"
              << "[10] Risk Prediction System\n"
              << "[0] Exit\n";
}

std::shared_ptr<Student> createStudentFromMenu() {
    cout << "\nSelect student type:\n";
    cout << "1. UG Student\n";
    cout << "2. PG Student\n";
    cout << "3. Scholarship Student\n";

    const int typeChoice = Utils::readInt("Enter choice: ", 1, 3);
    const std::string id = Utils::readText("Student ID: ");
    const std::string name = Utils::readText("Full Name: ");
    const std::string department = Utils::readText("Department: ");
    const int maxSemester = (typeChoice == 2) ? 4 : 8;
    const std::string semesterPrompt = (typeChoice == 2) ? "Semester (1-4): " : "Semester (1-8): ";
    const int year = Utils::readInt(semesterPrompt, 1, maxSemester);
    const double gpa = Utils::readDouble("GPA (0.0 - 4.0): ", 0.0, 4.0);
    const double attendance = Utils::readDouble("Attendance % (0 - 100): ", 0.0, 100.0);
    const double assignments = Utils::readDouble("Assignment Score % (0 - 100): ", 0.0, 100.0);
    const double projects = Utils::readDouble("Semester Project Score % (0 - 100): ", 0.0, 100.0);
    double research = 0.0;
    double scholarship = 0.0;

    if (typeChoice == 2) {
        cout << "Select PG program:\n";
        cout << "1. Masters\n";
        cout << "2. PhD\n";
        const int pgProgram = Utils::readInt("Enter choice: ", 1, 2);
        research = Utils::readDouble("Research Score % (0 - 100): ", 0.0, 100.0);
        scholarship = (pgProgram == 2) ? 100.0 : 0.0;
    } else if (typeChoice == 3) {
        scholarship = Utils::readDouble("Scholarship % (0 - 100): ", 0.0, 100.0);
    }

    const double previousPerformance = 0.0;

    if (typeChoice == 1) {
        return std::make_shared<UGStudent>(id, name, department, year, gpa, attendance,
                                           assignments, projects, research, scholarship,
                                           previousPerformance);
    }
    if (typeChoice == 2) {
        return std::make_shared<PGStudent>(id, name, department, year, gpa, attendance,
                                           assignments, projects, research, scholarship,
                                           previousPerformance);
    }
    return std::make_shared<ScholarshipStudent>(id, name, department, year, gpa, attendance,
                                                assignments, projects, research, scholarship,
                                                previousPerformance);
}

void printStudentTableHeader() {
    Utils::printLine('-', 80);
    cout << std::left << std::setw(12) << "ID"
              << std::setw(20) << "Name"
              << std::setw(14) << "Type"
              << std::setw(15) << "Department"
              << std::setw(10) << "Semester"
              << std::setw(10) << "Score"
              << std::setw(12) << "Trend"
              << std::setw(10) << "Risk"
              << '\n';
    Utils::printLine('-', 80);
}

void showStudents(const DigitalTwinEngine& engine) {
    const auto& students = engine.getStudents();
    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }

    Utils::printHeader("ALL STUDENTS");
    printStudentTableHeader();
    for (const auto& student : students) {
        if (student) {
            student->printSummary(cout);
        }
    }
}

void showSortedStudents(const DigitalTwinEngine& engine) {
    const auto students = engine.getSortedByPerformance(true);
    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }

    Utils::printHeader("STUDENTS SORTED BY PERFORMANCE");
    printStudentTableHeader();
    for (const auto& student : students) {
        if (student) {
            student->printSummary(cout);
        }
    }
}

void showTopPerformers(const DigitalTwinEngine& engine) {
    const int count = Utils::readInt("How many top performers to display? ", 1, 20);
    const auto students = engine.getTopPerformers(static_cast<std::size_t>(count));
    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }

    Utils::printHeader("TOP PERFORMERS");
    printStudentTableHeader();
    for (const auto& student : students) {
        if (student) {
            student->printSummary(cout);
        }
    }
}

void showReportCard(const Student& student) {
    Utils::printHeader("STUDENT REPORT CARD");
    cout << student.reportCard();
    Utils::printLine('=', 80);
}

bool promptAndAssignString(const std::string& label, std::string& currentValue) {
    const std::string input = Utils::readText(label + " [Enter to keep current]: ", true);
    if (input.empty()) {
        return false;
    }
    currentValue = input;
    return true;
}

bool promptAndAssignInt(const std::string& label, int& currentValue, int minimum, int maximum) {
    while (true) {
        const std::string input = Utils::readText(label + " [Enter to keep current]: ", true);
        if (input.empty()) {
            return false;
        }
        int parsed = 0;
        if (Utils::tryParseInt(input, parsed) && parsed >= minimum && parsed <= maximum) {
            currentValue = parsed;
            return true;
        }
        cout << "Enter a valid integer between " << minimum << " and " << maximum << ".\n";
    }
}

bool promptAndAssignDouble(const std::string& label, double& currentValue, double minimum, double maximum) {
    while (true) {
        const std::string input = Utils::readText(label + " [Enter to keep current]: ", true);
        if (input.empty()) {
            return false;
        }
        double parsed = 0.0;
        if (Utils::tryParseDouble(input, parsed) && parsed >= minimum && parsed <= maximum) {
            currentValue = parsed;
            return true;
        }
        cout << "Enter a valid number between " << minimum << " and " << maximum << ".\n";
    }
}

void updateStudentInteractive(DigitalTwinEngine& engine) {
    const std::string id = Utils::readText("Enter student ID to update: ");
    Student* student = engine.findStudentById(id);
    if (!student) {
        cout << "Student not found.\n";
        return;
    }

    Utils::printHeader("CURRENT RECORD");
    cout << student->reportCard();
    Utils::printLine('=', 80);

    std::string name = student->getName();
    std::string department = student->getDepartment();
    int year = student->getYear();
    const int maxSemester = (Utils::toUpper(student->getType()) == "PG") ? 4 : 8;
    double gpa = student->getGpa();
    double attendance = student->getAttendanceRate();
    double assignments = student->getAssignmentScore();
    double projects = student->getProjectScore();
    double research = student->getResearchScore();
    double scholarship = student->getScholarshipPercent();

    promptAndAssignString("Full Name", name);
    promptAndAssignString("Department", department);
    promptAndAssignInt("Semester", year, 1, maxSemester);
    promptAndAssignDouble("GPA", gpa, 0.0, 4.0);
    promptAndAssignDouble("Attendance %", attendance, 0.0, 100.0);
    promptAndAssignDouble("Assignment Score %", assignments, 0.0, 100.0);
    promptAndAssignDouble("Semester Project Score %", projects, 0.0, 100.0);
    if (Utils::toUpper(student->getType()) == "PG") {
        promptAndAssignDouble("Research Score %", research, 0.0, 100.0);
    }
    if (Utils::toUpper(student->getType()) == "SCHOLARSHIP") {
        promptAndAssignDouble("Scholarship %", scholarship, 0.0, 100.0);
    }

    const double previousScore = student->calculatePerformanceScore();
    student->setName(name);
    student->setDepartment(department);
    student->setYear(year);
    student->setGpa(gpa);
    student->setAttendanceRate(attendance);
    student->setAssignmentScore(assignments);
    student->setProjectScore(projects);
    student->setResearchScore(research);
    student->setScholarshipPercent(scholarship);
    student->setPreviousPerformanceScore(previousScore);

    if (engine.saveData()) {
        cout << "Student updated successfully.\n";
    } else {
        cout << "Student updated, but saving failed.\n";
    }
}

void saveStudentData(DigitalTwinEngine& engine) {
    if (!engine.saveData()) {
        cout << "Warning: could not save student data.\n";
    }
}

} // namespace

int main() {
    DigitalTwinEngine engine;

    printBanner();
    cout << "Loaded records: " << engine.getStudents().size() << '\n';

    while (true) {
        printMenu();
        const int choice = Utils::readInt("Select an option: ", 0, 10);

        switch (choice) {
        case 1: {
            auto student = createStudentFromMenu();
            if (engine.addStudent(student)) {
                cout << "Student added successfully.\n";
            } else {
                cout << "A student with that ID already exists.\n";
            }
            break;
        }
        case 2:
            showStudents(engine);
            break;
        case 3: {
            const std::string id = Utils::readText("Enter student ID to search: ");
            Student* student = engine.findStudentById(id);
            if (student) {
                showReportCard(*student);
            } else {
                cout << "Student not found.\n";
            }
            break;
        }
        case 4: {
            const std::string id = Utils::readText("Enter student ID to delete: ");
            if (engine.deleteStudent(id)) {
                cout << "Student deleted successfully.\n";
            } else {
                cout << "Student not found.\n";
            }
            break;
        }
        case 5:
            updateStudentInteractive(engine);
            break;
        case 6:
            showSortedStudents(engine);
            break;
        case 7:
            showTopPerformers(engine);
            break;
        case 8: {
            const std::string reportContent = engine.generateReportContent();
            if (FileManager::saveReport(kReportFile, reportContent)) {
                cout << "Report generated successfully in report.txt\n";
            } else {
                cout << "Failed to generate report.\n";
            }
            break;
        }
        case 9:
            engine.displayAnalyticsDashboard();
            break;
        case 10:
            engine.displayRiskPredictionSystem();
            break;
        case 0:
            saveStudentData(engine);
            cout << "Exiting system. Data saved successfully.\n";
            return 0;
        default:
            cout << "Invalid option. Try again.\n";
            break;
        }
    }
}
