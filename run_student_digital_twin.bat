@echo off
setlocal

cd /d "%~dp0"

echo Compiling Student Digital Twin System...
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Student.cpp DerivedStudents.cpp DigitalTwinEngine.cpp FileManager.cpp -o student_digital_twin.exe
if errorlevel 1 (
    echo.
    echo Build failed. Please check the compiler output above.
    pause
    exit /b 1
)

echo.
echo Build successful. Launching application...
echo.
student_digital_twin.exe

echo.
echo Application closed.
pause