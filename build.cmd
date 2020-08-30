@echo off
set lang=English
set buildType=Release
set buildDir=build

if "%1" equ "help" (
    echo Usage:
    echo   build           build
    echo   build run       run build
    echo   build English   build in Endlish (default^)
    echo   build Chinese   build in Chinese
    echo   build help      display this help and exit
    exit 0 > nul
) else ( if "%1" equ "Chinese" (
    set lang=Chinese
    REM wsl ./script/changeEncoding.sh UTF-8 GBK
    cmd /c script\changeEncoding UTF-8 GBK
) else ( if "%1" equ "English" (
    set lang=English
) else ( if "%1" equ "run" (
    .\stars.exe
    exit
) else ( if "%1" neq "" (
    echo build: %1: no such instruction
    exit 1 > nul
)))))

mkdir build
cmake -D STARS_LANGUAGE_SETIING=%lang% -D CMAKE_BUILD_TYPE=Release ^
-D CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=.. -S . -B %buildDir% && ^
cmake --build %buildDir% --config Release --target stars

if "%1" equ "Chinese" (
    REM wsl ./script/changeEncoding.sh GBK UTF-8
    cmd /c script\changeEncoding GBK UTF-8
)
