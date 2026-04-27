#include "Student.h"

#include "Utils.h"

#include <iomanip>
#include <sstream>

Student::Student(const std::string& id,
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
    : studentId(id),
      fullName(name),
      department(dept),
      studyYear(year),
      gpa(gpaValue),
      attendanceRate(attendanceValue),
      assignmentScore(assignmentValue),
      projectScore(projectValue),
      researchScore(researchValue),
      scholarshipPercent(scholarshipValue),
      previousPerformanceScore(previousPerformanceValue) {}

std::string Student::buildCommonRecord() const {
    std::ostringstream output;
    output << studentId << '|'
           << fullName << '|'
           << department << '|'
           << studyYear << '|'
           << Utils::formatDouble(gpa) << '|'
           << Utils::formatDouble(attendanceRate) << '|'
           << Utils::formatDouble(assignmentScore) << '|'
           << Utils::formatDouble(projectScore) << '|'
           << Utils::formatDouble(researchScore) << '|'
           << Utils::formatDouble(scholarshipPercent) << '|'
           << Utils::formatDouble(previousPerformanceScore);
    return output.str();
}

const std::string& Student::getId() const { return studentId; }
const std::string& Student::getName() const { return fullName; }
const std::string& Student::getDepartment() const { return department; }
int Student::getYear() const { return studyYear; }
double Student::getGpa() const { return gpa; }
double Student::getAttendanceRate() const { return attendanceRate; }
double Student::getAssignmentScore() const { return assignmentScore; }
double Student::getProjectScore() const { return projectScore; }
double Student::getResearchScore() const { return researchScore; }
double Student::getScholarshipPercent() const { return scholarshipPercent; }
double Student::getPreviousPerformanceScore() const { return previousPerformanceScore; }

void Student::setId(const std::string& value) { studentId = value; }
void Student::setName(const std::string& value) { fullName = value; }
void Student::setDepartment(const std::string& value) { department = value; }
void Student::setYear(int value) { studyYear = value; }
void Student::setGpa(double value) { gpa = Utils::clampValue(value, 0.0, 4.0); }
void Student::setAttendanceRate(double value) { attendanceRate = Utils::clampValue(value, 0.0, 100.0); }
void Student::setAssignmentScore(double value) { assignmentScore = Utils::clampValue(value, 0.0, 100.0); }
void Student::setProjectScore(double value) { projectScore = Utils::clampValue(value, 0.0, 100.0); }
void Student::setResearchScore(double value) { researchScore = Utils::clampValue(value, 0.0, 100.0); }
void Student::setScholarshipPercent(double value) { scholarshipPercent = Utils::clampValue(value, 0.0, 100.0); }
void Student::setPreviousPerformanceScore(double value) { previousPerformanceScore = Utils::clampValue(value, 0.0, 100.0); }

std::string Student::getTrend() const {
    const double delta = calculatePerformanceScore() - previousPerformanceScore;
    if (delta > 3.0) {
        return "Improving";
    }
    if (delta < -3.0) {
        return "Declining";
    }
    return "Stable";
}

std::string Student::getRiskLevel() const {
    const double score = calculatePerformanceScore();
    if (attendanceRate < 80.0) {
        return "Debar";
    }
    if (score < 60.0 || gpa < 2.0) {
        return "High";
    }
    if (score < 70.0 || gpa < 2.6) {
        return "Medium";
    }
    return "Low";
}

bool Student::isAtRisk() const {
    return getRiskLevel() != "Low";
}

std::string Student::getSuggestion() const {
    std::ostringstream output;
    const std::string risk = getRiskLevel();
    const std::string trend = getTrend();

    if (risk == "Debar") {
        output << "Attendance is below 80%: you can be debarred in this course.";
    } else if (risk == "High") {
        output << "Urgent intervention: meet an advisor, improve attendance, and review core concepts.";
    } else if (risk == "Medium") {
        output << "Stay consistent: strengthen weekly study habits and track assignment deadlines.";
    } else {
        output << "Maintain the current pace and continue building stronger project outcomes.";
    }

    if (calculatePerformanceScore() < 60.0) {
        output << " Risk is much higher because score is below 60%.";
    }
    if (gpa < 2.0) {
        output << " GPA is on warning (below 2.0).";
    }

    if (trend == "Declining") {
        output << " Performance is declining, so act early to reverse the trend.";
    } else if (trend == "Improving") {
        output << " Momentum is positive, so keep this study pattern active.";
    }

    return output.str();
}

std::string Student::getPerformanceBand() const {
    const double score = calculatePerformanceScore();
    if (score >= 85.0) {
        return "Excellent";
    }
    if (score >= 70.0) {
        return "Good";
    }
    if (score >= 55.0) {
        return "Satisfactory";
    }
    return "Needs Improvement";
}


std::string Student::reportCard() const {
    std::ostringstream output;
    output << "Student ID   : " << studentId << '\n';
    output << "Name         : " << fullName << '\n';
    output << "Type         : " << getType() << '\n';
    output << "Profile Tag  : " << getProfileTag() << '\n';
    output << "Department   : " << department << '\n';
    output << "Semester     : " << studyYear << '\n';
    output << "GPA          : " << Utils::formatDouble(gpa) << '\n';
    output << "Attendance   : " << Utils::formatDouble(attendanceRate) << "%\n";
    output << "Assignments  : " << Utils::formatDouble(assignmentScore) << "%\n";
    output << "Sem Project  : " << Utils::formatDouble(projectScore) << "%\n";
    output << "Research     : " << Utils::formatDouble(researchScore) << "%\n";
    output << "Scholarship  : " << Utils::formatDouble(scholarshipPercent) << "%\n";
    output << "Previous Score: " << Utils::formatDouble(previousPerformanceScore) << '\n';
    output << "Current Score : " << Utils::formatDouble(calculatePerformanceScore()) << '\n';
    output << "Trend        : " << getTrend() << '\n';
    output << "Risk Level   : " << getRiskLevel() << '\n';
    output << "Band         : " << getPerformanceBand() << '\n';
    output << "Suggestion   : " << getSuggestion() << '\n';
    return output.str();
}

void Student::printSummary(std::ostream& output) const {
    output << std::left
           << std::setw(12) << getId()
           << std::setw(20) << getName().substr(0, 18)
           << std::setw(14) << getType()
           << std::setw(15) << getDepartment().substr(0, 13)
           << std::setw(10) << getYear()
           << std::setw(10) << Utils::formatDouble(calculatePerformanceScore())
           << std::setw(12) << getTrend()
           << std::setw(10) << getRiskLevel()
           << '\n';
}
