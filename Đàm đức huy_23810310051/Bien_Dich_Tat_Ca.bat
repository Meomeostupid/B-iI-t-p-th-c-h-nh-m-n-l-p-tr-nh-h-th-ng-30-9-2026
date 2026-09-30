@echo off
title Bien dich tat ca cac bai tap C++
cd /d "%~dp0"
echo ========================================================
echo   DANG TIEN HANH BIEN DICH TOAN BO CAC BAI TAP C++
echo ========================================================
echo.

set GPP="C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe"

if not exist %GPP% (
    set GPP=g++
)

echo [1/5] Dang bien dich Bai 01 (CT1.cpp)...
%GPP% -std=c++11 -Wall "Bai01\CT1.cpp" -o "Bai01\CT1.exe"
if %errorlevel% neq 0 ( echo LOI bien dich CT1! & goto end )

echo [2/5] Dang bien dich Bai 01 (CT2.cpp)...
%GPP% -std=c++11 -Wall "Bai01\CT2.cpp" -o "Bai01\CT2.exe"
if %errorlevel% neq 0 ( echo LOI bien dich CT2! & goto end )

echo [3/5] Dang bien dich Bai 02 (Bai02.cpp)...
%GPP% -std=c++11 -Wall "Bai02\Bai02.cpp" -o "Bai02\Bai02.exe"
if %errorlevel% neq 0 ( echo LOI bien dich Bai02! & goto end )

echo [4/5] Dang bien dich Bai 03 (Bai03.cpp)...
%GPP% -std=c++11 -Wall "Bai03\Bai03.cpp" -o "Bai03\Bai03.exe"
if %errorlevel% neq 0 ( echo LOI bien dich Bai03! & goto end )

echo [5/5] Dang bien dich Bai 04 (Bai04.cpp)...
%GPP% -std=c++11 -Wall "Bai04\Bai04.cpp" -o "Bai04\Bai04.exe"
if %errorlevel% neq 0 ( echo LOI bien dich Bai04! & goto end )

echo.
echo ========================================================
echo   CHUC MUNG: TAT CA CAC FILE DA DUOC BIEN DICH THANH CONG!
echo ========================================================
:end
pause
