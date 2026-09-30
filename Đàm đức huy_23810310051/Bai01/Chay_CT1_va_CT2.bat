@echo off
title Chay CT1 va CT2 dong thoi - Bai 01
cd /d "%~dp0"
echo ========================================================
echo   KHOI DONG DONG THOI CT1 VA CT2 (BAI 01)
echo   CT1 se phat sinh so va ghi vao file dulieu.dat
echo   CT2 se doc so tu file dulieu.dat va hien thi
echo ========================================================
echo.
start "CT1 - Sinh so va ghi dulieu.dat" cmd /c "CT1.exe"
start "CT2 - Doc file dulieu.dat" cmd /c "CT2.exe"
