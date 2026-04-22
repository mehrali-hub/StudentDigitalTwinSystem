#include "DigitalTwinEngine.h"

#include "FileManager.h"
#include "Utils.h"

#include <algorithm>
#include <numeric>
#include <sstream>

DigitalTwinEngine::DigitalTwinEngine(const std::string& dataFilePath, const std::string& reportFilePath)
    : dataFilePath(dataFilePath), reportFilePath(reportFilePath) {
    loadData();
}

void DigitalTwinEngine::loadData() {
    students = FileManager::loadStudents(dataFilePath);
}

bool DigitalTwinEngine::saveData() const {
    return FileManager::saveStudents(dataFilePath, students);
}

const std::vector<std::shared_ptr<Student>>& DigitalTwinEngine::getStudents() const {
    return students;
}

Student* DigitalTwinEngine::findStudentById(const std::string& id) const {
    for (const auto& student : students) {
        if (student && student->getId() == id) {
            return student.get();
        }
    }
    return nullptr;
}

bool DigitalTwinEngine::addStudent(const std::shared_ptr<Student>& student) {
    if (!student || findStudentById(student->getId()) != nullptr) {
        return false;
    }

    student->setPreviousPerformanceScore(student->calculatePerformanceScore());
    students.push_back(student);
    return saveData();
}

bool DigitalTwinEngine::deleteStudent(const std::string& id) {
    const auto originalSize = students.size();
    students.erase(std::remove_if(students.begin(), students.end(), [&](const std::shared_ptr<Student>& student) {
        return student && student->getId() == id;
    }), students.end());

    if (students.size() == originalSize) {
        return false;
    }

    return saveData();
}

bool DigitalTwinEngine::updateStudent(const std::string& id, const std::function<void(Student&)>& updater) {
    Student* student = findStudentById(id);
    if (!student) {
        return false;
    }

    const double oldScore = student->calculatePerformanceScore();
    updater(*student);
    student->setPreviousPerformanceScore(oldScore);
    return saveData();
}

std::vector<std::shared_ptr<Student>> DigitalTwinEngine::getSortedByPerformance(bool descending) const {
    std::vector<std::shared_ptr<Student>> sortedStudents = students;
    std::sort(sortedStudents.begin(), sortedStudents.end(), [descending](const std::shared_ptr<Student>& left, const std::shared_ptr<Student>& right) {
        if (!left) {
            return false;
        }
        if (!right) {
            return true;
        }

        const double leftScore = left->calculatePerformanceScore();
        const double rightScore = right->calculatePerformanceScore();
        if (leftScore == rightScore) {
            return left->getId() < right->getId();
        }
        return descending ? (leftScore > rightScore) : (leftScore < rightScore);
    });
    return sortedStudents;
}

std::vector<std::shared_ptr<Student>> DigitalTwinEngine::getTopPerformers(std::size_t limit) const {
    auto sortedStudents = getSortedByPerformance(true);
    if (limit < sortedStudents.size()) {
        sortedStudents.resize(limit);
    }
    return sortedStudents;
}

std::vector<std::shared_ptr<Student>> DigitalTwinEngine::getRiskStudents() const {
    std::vector<std::shared_ptr<Student>> riskStudents;
    for (const auto& student : students) {
        if (student && student->isAtRisk()) {
            riskStudents.push_back(student);
        }
    }
    return riskStudents;
}

bool DigitalTwinEngine::isRiskStudent(const Student& student) const {
    return student.isAtRisk();
}

std::vector<std::string> DigitalTwinEngine::buildSuggestions(const Student& student) const {
    std::vector<std::string> suggestions;
    if (student.getAttendanceRate() < 65.0) {
        suggestions.push_back("Improve attendance consistency.");
    }
    if (student.getAssignmentScore() < 60.0) {
        suggestions.push_back("Submit assignments earlier and review teacher feedback.");
    }
    if (student.getGpa() < 2.6) {
        suggestions.push_back("Focus on core concepts and seek academic support.");
    }
    if (student.getTrend() == "Declining") {
        suggestions.push_back("Use weekly progress checks to stop the decline.");
    }
    if (suggestions.empty()) {
        suggestions.push_back("Maintain the current routine and keep improving.");
    }
    return suggestions;
}

std::string DigitalTwinEngine::generateDashboard() const {
    std::ostringstream output;
    output << "DIGITAL TWIN ANALYTICS DASHBOARD\n";
    output << std::string(80, '=') << '\n';

    if (students.empty()) {
        output << "No student data available.\n";
        return output.str();
    }

    const double totalScore = std::accumulate(students.begin(), students.end(), 0.0,
        [](double total, const std::shared_ptr<Student>& student) {
            return total + (student ? student->calculatePerformanceScore() : 0.0);
        });

    const double averageScore = totalScore / static_cast<double>(students.size());
    int ugCount = 0;
    int pgCount = 0;
    int scholarshipCount = 0;
    int improvingCount = 0;
    int decliningCount = 0;
    int stableCount = 0;
    int highRiskCount = 0;
    int mediumRiskCount = 0;
    int lowRiskCount = 0;

    std::shared_ptr<Student> topStudent = students.front();
    for (const auto& student : students) {
        if (!student) {
            continue;
        }

        if (!topStudent || student->calculatePerformanceScore() > topStudent->calculatePerformanceScore()) {
            topStudent = student;
        }

        if (student->getType() == "UG") {
            ++ugCount;
        } else if (student->getType() == "PG") {
            ++pgCount;
        } else if (student->getType() == "SCHOLARSHIP") {
            ++scholarshipCount;
        }

        if (student->getTrend() == "Improving") {
            ++improvingCount;
        } else if (student->getTrend() == "Declining") {
            ++decliningCount;
        } else {
            ++stableCount;
        }

        const std::string riskLevel = student->getRiskLevel();
        if (riskLevel == "High") {
            ++highRiskCount;
        } else if (riskLevel == "Medium") {
            ++mediumRiskCount;
        } else {
            ++lowRiskCount;
        }
    }

    output << "Total Students       : " << students.size() << '\n';
    output << "Average Performance   : " << Utils::formatDouble(averageScore) << '\n';
    if (topStudent) {
        output << "Top Performer        : " << topStudent->getName() << " (" << topStudent->getId() << ") - "
               << Utils::formatDouble(topStudent->calculatePerformanceScore()) << '\n';
    }
    output << "Type Distribution     : UG=" << ugCount << ", PG=" << pgCount << ", Scholarship=" << scholarshipCount << '\n';
    output << "Trend Distribution    : Improving=" << improvingCount << ", Stable=" << stableCount << ", Declining=" << decliningCount << '\n';
    output << "Risk Distribution     : High=" << highRiskCount << ", Medium=" << mediumRiskCount << ", Low=" << lowRiskCount << '\n';
    output << "Risk Students         : " << getRiskStudents().size() << '\n';
    return output.str();
}

std::string DigitalTwinEngine::generateReportContent() const {
    std::ostringstream output;
    output << "STUDENT DIGITAL TWIN SYSTEM REPORT\n";
    output << std::string(80, '=') << '\n';
    output << generateDashboard();
    output << '\n' << "TOP PERFORMERS\n";
    output << std::string(80, '-') << '\n';

    const auto topPerformers = getTopPerformers(5);
    if (topPerformers.empty()) {
        output << "No top performers available.\n";
    } else {
        for (const auto& student : topPerformers) {
            if (student) {
                output << student->getId() << " | " << student->getName() << " | " << student->getType()
                       << " | Score: " << Utils::formatDouble(student->calculatePerformanceScore())
                       << " | Trend: " << student->getTrend() << '\n';
            }
        }
    }

    output << '\n' << "RISK STUDENTS\n";
    output << std::string(80, '-') << '\n';
    const auto riskStudents = getRiskStudents();
    if (riskStudents.empty()) {
        output << "No risk students identified.\n";
    } else {
        for (const auto& student : riskStudents) {
            if (student) {
                output << student->getId() << " | " << student->getName() << " | " << student->getRiskLevel()
                       << " Risk | " << student->getSuggestion() << '\n';
            }
        }
    }

    output << '\n' << "STUDENT REPORT CARDS\n";
    output << std::string(80, '-') << '\n';
    if (students.empty()) {
        output << "No student records available.\n";
    } else {
        for (const auto& student : students) {
            if (student) {
                output << student->reportCard() << '\n';
                output << std::string(80, '-') << '\n';
            }
        }
    }

    return output.str();
}

void DigitalTwinEngine::displayAllStudents() const {
    if (students.empty()) {
        std::cout << "No student records found.\n";
        return;
    }

    Utils::printHeader("ALL STUDENTS");
    Utils::printLine('-', 80);
    std::cout << std::left << std::setw(12) << "ID"
              << std::setw(20) << "Name"
              << std::setw(14) << "Type"
              << std::setw(15) << "Department"
              << std::setw(8) << "Year"
              << std::setw(10) << "Score"
              << std::setw(12) << "Trend"
              << std::setw(10) << "Risk"
              << '\n';
    Utils::printLine('-', 80);
    for (const auto& student : students) {
        if (student) {
            student->printSummary(std::cout);
        }
    }
}

void DigitalTwinEngine::displaySortedStudents() const {
    const auto sortedStudents = getSortedByPerformance(true);
    if (sortedStudents.empty()) {
        std::cout << "No student records found.\n";
        return;
    }

    Utils::printHeader("STUDENTS SORTED BY PERFORMANCE");
    Utils::printLine('-', 80);
    std::cout << std::left << std::setw(12) << "ID"
              << std::setw(20) << "Name"
              << std::setw(14) << "Type"
              << std::setw(15) << "Department"
              << std::setw(8) << "Year"
              << std::setw(10) << "Score"
              << std::setw(12) << "Trend"
              << std::setw(10) << "Risk"
              << '\n';
    Utils::printLine('-', 80);
    for (const auto& student : sortedStudents) {
        if (student) {
            student->printSummary(std::cout);
        }
    }
}

void DigitalTwinEngine::displayTopPerformers(std::size_t limit) const {
    const auto topStudents = getTopPerformers(limit);
    if (topStudents.empty()) {
        std::cout << "No student records found.\n";
        return;
    }

    Utils::printHeader("TOP PERFORMERS");
    Utils::printLine('-', 80);
    std::cout << std::left << std::setw(12) << "ID"
              << std::setw(20) << "Name"
              << std::setw(14) << "Type"
              << std::setw(15) << "Department"
              << std::setw(8) << "Year"
              << std::setw(10) << "Score"
              << std::setw(12) << "Trend"
              << std::setw(10) << "Risk"
              << '\n';
    Utils::printLine('-', 80);
    for (const auto& student : topStudents) {
        if (student) {
            student->printSummary(std::cout);
        }
    }
}

void DigitalTwinEngine::displayAnalyticsDashboard() const {
    std::cout << generateDashboard();
}

void DigitalTwinEngine::displayRiskPredictionSystem() const {
    Utils::printHeader("RISK PREDICTION SYSTEM");
    const auto riskStudents = getRiskStudents();

    if (riskStudents.empty()) {
        std::cout << "No risk students identified.\n";
        return;
    }

    Utils::printLine('-', 80);
    std::cout << std::left << std::setw(12) << "ID"
              << std::setw(20) << "Name"
              << std::setw(14) << "Type"
              << std::setw(15) << "Department"
              << std::setw(8) << "Year"
              << std::setw(10) << "Score"
              << std::setw(12) << "Trend"
              << std::setw(10) << "Risk"
              << '\n';
    Utils::printLine('-', 80);

    for (const auto& student : riskStudents) {
        if (student) {
            student->printSummary(std::cout);
        }
    }

    std::cout << '\n' << "Detailed Suggestions:\n";
    for (const auto& student : riskStudents) {
        if (student) {
            std::cout << student->getId() << " - " << student->getName() << ": " << student->getSuggestion() << '\n';
        }
    }
}
