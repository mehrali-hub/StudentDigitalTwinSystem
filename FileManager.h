#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Student.h"

#include <memory>
#include <string>
#include <vector>

class FileManager {
public:
    static std::vector<std::shared_ptr<Student>> loadStudents(const std::string& fileName);
    static bool saveStudents(const std::string& fileName, const std::vector<std::shared_ptr<Student>>& students);
    static bool saveReport(const std::string& fileName, const std::string& reportContent);
};

#endif
