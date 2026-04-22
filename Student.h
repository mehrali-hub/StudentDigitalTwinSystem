#ifndef STUDENT_H
#define STUDENT_H

#include <iosfwd>
#include <string>

class Student {
private:
    std::string studentId;
    std::string fullName;
    std::string department;
    int studyYear;
    double gpa;
    double attendanceRate;
    double assignmentScore;
    double projectScore;
    double researchScore;
    double scholarshipPercent;
    double previousPerformanceScore;

protected:
    std::string buildCommonRecord() const;

public:
    Student(const std::string& id = "",
            const std::string& name = "",
            const std::string& dept = "",
            int year = 1,
            double gpaValue = 0.0,
            double attendanceValue = 0.0,
            double assignmentValue = 0.0,
            double projectValue = 0.0,
            double researchValue = 0.0,
            double scholarshipValue = 0.0,
            double previousPerformanceValue = 0.0);
    virtual ~Student() = default;

    virtual std::string getType() const = 0;
    virtual std::string getProfileTag() const = 0;
    virtual double calculatePerformanceScore() const = 0;
    virtual std::string serialize() const = 0;

    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getDepartment() const;
    int getYear() const;
    double getGpa() const;
    double getAttendanceRate() const;
    double getAssignmentScore() const;
    double getProjectScore() const;
    double getResearchScore() const;
    double getScholarshipPercent() const;
    double getPreviousPerformanceScore() const;

    void setId(const std::string& value);
    void setName(const std::string& value);
    void setDepartment(const std::string& value);
    void setYear(int value);
    void setGpa(double value);
    void setAttendanceRate(double value);
    void setAssignmentScore(double value);
    void setProjectScore(double value);
    void setResearchScore(double value);
    void setScholarshipPercent(double value);
    void setPreviousPerformanceScore(double value);

    std::string getTrend() const;
    std::string getRiskLevel() const;
    bool isAtRisk() const;
    std::string getSuggestion() const;
    std::string getPerformanceBand() const;
    std::string reportCard() const;
    void printSummary(std::ostream& output) const;
};

#endif
