#include "DerivedStudents.h"

#include "Utils.h"

#include <algorithm>
#include <memory>

namespace {

double gpaToScore(double gpa) {
    return Utils::clampValue((gpa / 4.0) * 100.0, 0.0, 100.0);
}

} // namespace

UGStudent::UGStudent(const std::string& id,
                     const std::string& name,
                     const std::string& dept,
                     int year,
                     double gpaValue,
                     double attendanceValue,
                     double assignmentValue,
                     double projectValue,
                     double researchValue,
                     double scholarshipValue,
                     double previousPerformanceValue)
    : Student(id, name, dept, year, gpaValue, attendanceValue, assignmentValue, projectValue,
              researchValue, scholarshipValue, previousPerformanceValue) {}

std::string UGStudent::getType() const {
    return "UG";
}

std::string UGStudent::getProfileTag() const {
    return "Undergraduate academic growth";
}

double UGStudent::calculatePerformanceScore() const {
    const double score = 0.45 * gpaToScore(getGpa()) +
                         0.20 * getAttendanceRate() +
                         0.20 * getAssignmentScore() +
                         0.15 * getProjectScore();
    return Utils::clampValue(score, 0.0, 100.0);
}

std::string UGStudent::serialize() const {
    return getType() + "|" + buildCommonRecord();
}

PGStudent::PGStudent(const std::string& id,
                     const std::string& name,
                     const std::string& dept,
                     int year,
                     double gpaValue,
                     double attendanceValue,
                     double assignmentValue,
                     double projectValue,
                     double researchValue,
                     double scholarshipValue,
                     double previousPerformanceValue)
    : Student(id, name, dept, year, gpaValue, attendanceValue, assignmentValue, projectValue,
              researchValue, scholarshipValue, previousPerformanceValue) {}

std::string PGStudent::getType() const {
    return "PG";
}

std::string PGStudent::getProfileTag() const {
    if (getScholarshipPercent() >= 50.0) {
        return "PhD track (Semester 1-4)";
    }
    return "Masters track (Semester 1-4)";
}

double PGStudent::calculatePerformanceScore() const {
    double score = 0.0;
    if (getScholarshipPercent() >= 50.0) {
        score = 0.30 * gpaToScore(getGpa()) +
                0.15 * getAttendanceRate() +
                0.40 * getResearchScore() +
                0.15 * getProjectScore();
    } else {
        score = 0.35 * gpaToScore(getGpa()) +
                0.20 * getAttendanceRate() +
                0.25 * getResearchScore() +
                0.20 * getProjectScore();
    }
    return Utils::clampValue(score, 0.0, 100.0);
}

std::string PGStudent::serialize() const {
    return getType() + "|" + buildCommonRecord();
}

ScholarshipStudent::ScholarshipStudent(const std::string& id,
                                       const std::string& name,
                                       const std::string& dept,
                                       int year,
                                       double gpaValue,
                                       double attendanceValue,
                                       double assignmentValue,
                                       double projectValue,
                                       double researchValue,
                                       double scholarshipValue,
                                       double previousPerformanceValue)
    : UGStudent(id, name, dept, year, gpaValue, attendanceValue, assignmentValue, projectValue,
                researchValue, scholarshipValue, previousPerformanceValue) {}

std::string ScholarshipStudent::getType() const {
    return "SCHOLARSHIP";
}

std::string ScholarshipStudent::getProfileTag() const {
    return "Scholarship student with excellence bonus";
}

double ScholarshipStudent::calculatePerformanceScore() const {
    const double baseScore = UGStudent::calculatePerformanceScore();
    const double scholarshipBonus = std::min(10.0, getScholarshipPercent() * 0.12);
    return Utils::clampValue(baseScore + scholarshipBonus, 0.0, 100.0);
}

std::string ScholarshipStudent::serialize() const {
    return getType() + "|" + buildCommonRecord();
}

std::shared_ptr<Student> createStudentFromRecord(const std::vector<std::string>& fields) {
    if (fields.size() < 12) {
        return nullptr;
    }

    const std::string type = Utils::toUpper(Utils::trim(fields[0]));
    const std::string id = Utils::trim(fields[1]);
    const std::string name = Utils::trim(fields[2]);
    const std::string department = Utils::trim(fields[3]);
    const int year = std::stoi(Utils::trim(fields[4]));
    const double gpa = std::stod(Utils::trim(fields[5]));
    const double attendance = std::stod(Utils::trim(fields[6]));
    const double assignments = std::stod(Utils::trim(fields[7]));
    const double projects = std::stod(Utils::trim(fields[8]));
    const double research = std::stod(Utils::trim(fields[9]));
    const double scholarship = std::stod(Utils::trim(fields[10]));
    const double previous = std::stod(Utils::trim(fields[11]));

    if (type == "UG") {
        return std::make_shared<UGStudent>(id, name, department, year, gpa, attendance,
                                           assignments, projects, research, scholarship, previous);
    }
    if (type == "PG") {
        return std::make_shared<PGStudent>(id, name, department, year, gpa, attendance,
                                           assignments, projects, research, scholarship, previous);
    }
    if (type == "SCHOLARSHIP") {
        return std::make_shared<ScholarshipStudent>(id, name, department, year, gpa, attendance,
                                                    assignments, projects, research, scholarship,
                                                    previous);
    }
    return nullptr;
}
