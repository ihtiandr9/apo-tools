REM @echo off
set PATH=%~dp0Build\tools\TDM-GCC-32\bin;%PATH%

IF NOT EXIST %~dp0Build\tools\cmake\cmake-3.13.5-win32-x86\bin\cmake.exe (
    IF NOT EXIST %~dp0Build\tools\cmake-3.13.5-win32-x86.zip (
        %~dp0Build\tools\wget.exe --continue http://ihtiandr9.fvds.ru/tools/cmake-3.13.5-win32-x86.zip -O %~dp0Build\tools\cmake-3.13.5-win32-x86.zip
    )

    IF %ERRORLEVEL% == 0 (
        %~dp0Build\tools\unzip\unzip.exe %~dp0Build\tools\cmake-3.13.5-win32-x86.zip cmake-3.13.5-win32-x86\*.* -x cmake-3.13.5-win32-x86\doc\*.*  -d %~dp0Build\tools\cmake
    ) ELSE ( echo "Error extracting cmake" )
)


IF NOT EXIST %~dp0Build\CMake.MinGW32\CMakeCache.txt (
    %~dp0Build\tools\cmake\cmake-3.13.5-win32-x86\bin\cmake.exe Build -DCMAKE_MAKE_PROGRAM=mingw32-make.exe -BBuild/CMake.MinGW -G "MinGW Makefiles"
)

%~dp0Build\tools\cmake\cmake-3.13.5-win32-x86\bin\cmake.exe --build Build/CMake.MinGW -j 6
