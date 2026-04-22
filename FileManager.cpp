#include "FileManager.h"

#include "DerivedStudents.h"
#include "Utils.h"

#include <fstream>
#include <memory>

namespace {

double parseDoubleOrZero(const std::string& value) {
    try {
        return std::stod(Utils::trim(value));
    } catch (...) {
        return 0.0;
    }
}

int parseIntOrOne(const std::string& value) {
    try {
        return std::stoi(Utils::trim(value));
    } catch (...) {
        return 1;
    }
}

} // namespace

std::vector<std::shared_ptr<Student>> FileManager::loadStudents(const std::string& fileName) {
    std::vector<std::shared_ptr<Student>> students;
    std::ifstream input(fileName);
    if (!input.is_open()) {
        return students;
    }

    std::string line;
    while (std::getline(input, line)) {
        line = Utils::trim(line);
        if (line.empty()) {
            continue;
        }

        const auto fields = Utils::split(line, '|');
        if (fields.size() < 12) {
            continue;
        }

        const std::string type = Utils::toUpper(Utils::trim(fields[0]));
        const std::string id = Utils::trim(fields[1]);
        const std::string name = Utils::trim(fields[2]);
        const std::string department = Utils::trim(fields[3]);
        const int year = parseIntOrOne(fields[4]);
        const double gpa = parseDoubleOrZero(fields[5]);
        const double attendance = parseDoubleOrZero(fields[6]);
        const double assignments = parseDoubleOrZero(fields[7]);
        const double projects = parseDoubleOrZero(fields[8]);
        const double research = parseDoubleOrZero(fields[9]);
        const double scholarship = parseDoubleOrZero(fields[10]);
        const double previous = parseDoubleOrZero(fields[11]);

        std::shared_ptr<Student> student;
        if (type == "UG") {
            student = std::make_shared<UGStudent>(id, name, department, year, gpa, attendance,
                                                  assignments, projects, research, scholarship, previous);
        } else if (type == "PG") {
            student = std::make_shared<PGStudent>(id, name, department, year, gpa, attendance,
                                                  assignments, projects, research, scholarship, previous);
        } else if (type == "SCHOLARSHIP") {
            student = std::make_shared<ScholarshipStudent>(id, name, department, year, gpa, attendance,
                                                           assignments, projects, research, scholarship, previous);
        }

        if (student) {
            students.push_back(student);
        }
    }

    return students;
}

bool FileManager::saveStudents(const std::string& fileName, const std::vector<std::shared_ptr<Student>>& students) {
    std::ofstream output(fileName, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    for (const auto& student : students) {
        if (student) {
            output << student->serialize() << '\n';
        }
    }

    return true;
}

bool FileManager::saveReport(const std::string& fileName, const std::string& reportContent) {
    std::ofstream output(fileName, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    output << reportContent;
    return true;
}
