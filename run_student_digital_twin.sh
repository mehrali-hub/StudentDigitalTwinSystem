#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

echo "Compiling Student Digital Twin System..."
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Student.cpp DerivedStudents.cpp DigitalTwinEngine.cpp FileManager.cpp -o student_digital_twin.exe

echo
echo "Build successful. Launching application..."
echo
./student_digital_twin.exe