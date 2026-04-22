#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace Utils {

inline std::string trim(const std::string& value) {
    const std::string whitespace = " \t\n\r\f\v";
    const std::size_t start = value.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }
    const std::size_t end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

inline std::string toUpper(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });
    return value;
}

inline std::vector<std::string> split(const std::string& value, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream stream(value);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        parts.push_back(token);
    }
    return parts;
}

inline std::string formatDouble(double value, int precision = 2) {
    std::ostringstream output;
    output << std::fixed << std::setprecision(precision) << value;
    return output.str();
}

inline double clampValue(double value, double minimum, double maximum) {
    return std::max(minimum, std::min(value, maximum));
}

inline void printLine(char character = '-', int count = 80) {
    std::cout << std::string(count, character) << '\n';
}

inline void printHeader(const std::string& title) {
    printLine('=', 80);
    std::cout << title << '\n';
    printLine('=', 80);
}

inline std::string readText(const std::string& prompt, bool allowEmpty = false) {
    while (true) {
        std::cout << prompt;
        std::string value;
        std::getline(std::cin, value);
        value = trim(value);
        if (allowEmpty || !value.empty()) {
            return value;
        }
        std::cout << "Input cannot be empty. Try again.\n";
    }
}

inline int readInt(const std::string& prompt, int minimum, int maximum) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        std::stringstream stream(input);
        int value = 0;
        char extra = '\0';
        if (stream >> value && !(stream >> extra) && value >= minimum && value <= maximum) {
            return value;
        }
        std::cout << "Enter a valid integer between " << minimum << " and " << maximum << ".\n";
    }
}

inline double readDouble(const std::string& prompt, double minimum, double maximum) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        std::stringstream stream(input);
        double value = 0.0;
        char extra = '\0';
        if (stream >> value && !(stream >> extra) && value >= minimum && value <= maximum) {
            return value;
        }
        std::cout << "Enter a valid number between " << minimum << " and " << maximum << ".\n";
    }
}

inline bool tryParseInt(const std::string& text, int& value) {
    std::stringstream stream(trim(text));
    char extra = '\0';
    return (stream >> value) && !(stream >> extra);
}

inline bool tryParseDouble(const std::string& text, double& value) {
    std::stringstream stream(trim(text));
    char extra = '\0';
    return (stream >> value) && !(stream >> extra);
}

} // namespace Utils

#endif
