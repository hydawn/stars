@echo off

REM Note:
REM change every file nessacery to make a Chinese version or change it back
REM to UTF-8

set list=^
src/boardAnalyse.cpp ^
src/boardInterface.cpp ^
src/boardRecord.cpp ^
src/boardTools.cpp ^
main.cpp

REM check argument
if "%1" equ "" (
    echo Lack of argument
    echo Usage: cmd /c script/changeEncoding [from_encoding] [to_encoding]
    exit 1
)
if "%2" equ "" (
    echo Lack of argument
    echo Usage: cmd /c script/changeEncoding [from_encoding] [to_encoding]
    exit 1
)
if "%1" neq "GBK" (
if "%1" neq "UTF-8" (
    echo Wrong use of argument, only need GBK and UTF-8 - at leat on my computer
    exit 1
))
if "%2" neq "GBK" (
if "%2" neq "UTF-8" (
    echo Wrong use of argument, only need GBK and UTF-8 - at leat on my computer
    exit 1
))

for %%a in (%list%) do (
    REM echo %%a
    ((iconv --from-code=%1 --to-code=%2 %%a) > tempWorking.cpp) || ^
        echo %%a - something went wrong
    move /Y tempWorking.cpp %%a
)
