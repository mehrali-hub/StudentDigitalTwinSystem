#ifndef DERIVEDSTUDENTS_H
#define DERIVEDSTUDENTS_H

#include "Student.h"

#include <memory>
#include <string>
#include <vector>

class UGStudent : public Student {
public:
    UGStudent(const std::string& id = "",
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

    std::string getType() const override;
    std::string getProfileTag() const override;
    double calculatePerformanceScore() const override;
    std::string serialize() const override;
};

class PGStudent : public Student {
public:
    PGStudent(const std::string& id = "",
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

    std::string getType() const override;
    std::string getProfileTag() const override;
    double calculatePerformanceScore() const override;
    std::string serialize() const override;
};

class ScholarshipStudent : public UGStudent {
public:
    ScholarshipStudent(const std::string& id = "",
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

    std::string getType() const override;
    std::string getProfileTag() const override;
    double calculatePerformanceScore() const override;
    std::string serialize() const override;
};

std::shared_ptr<Student> createStudentFromRecord(const std::vector<std::string>& fields);

#endif
