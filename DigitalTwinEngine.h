#ifndef DIGITALTWINENGINE_H
#define DIGITALTWINENGINE_H

#include "DerivedStudents.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

class DigitalTwinEngine {
private:
    std::vector<std::shared_ptr<Student>> students;
    std::string dataFilePath;
    std::string reportFilePath;

    bool isRiskStudent(const Student& student) const;
    std::vector<std::string> buildSuggestions(const Student& student) const;

public:
    DigitalTwinEngine(const std::string& dataFilePath = "students.txt", const std::string& reportFilePath = "report.txt");

    void loadData();
    bool saveData() const;

    const std::vector<std::shared_ptr<Student>>& getStudents() const;
    bool addStudent(const std::shared_ptr<Student>& student);
    Student* findStudentById(const std::string& id) const;
    bool deleteStudent(const std::string& id);
    bool updateStudent(const std::string& id, const std::function<void(Student&)>& updater);

    std::vector<std::shared_ptr<Student>> getSortedByPerformance(bool descending = true) const;
    std::vector<std::shared_ptr<Student>> getTopPerformers(std::size_t limit) const;
    std::vector<std::shared_ptr<Student>> getRiskStudents() const;

    std::string generateDashboard() const;
    std::string generateReportContent() const;

    void displayAllStudents() const;
    void displaySortedStudents() const;
    void displayTopPerformers(std::size_t limit) const;
    void displayAnalyticsDashboard() const;
    void displayRiskPredictionSystem() const;
};

#endif
