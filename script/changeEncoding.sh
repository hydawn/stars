#!/bin/bash

# Note:
# change every file nessacery to make a Chinese version or change it back to
# UTF-8

from_encoding=$1
to_encoding=$2
declare -a filesToConvert=(
    "src/boardAnalyse.cpp"
    "src/boardInterface.cpp"
    "src/boardRecord.cpp"
    "src/boardTools.cpp"
    "main.cpp"
)

function checkArgs() {
    if [ $# -ne 2 ]; then
        echo Lack of arguments
        echo Usage:
        echo   ./changeEncoding.sh [from_encoding] [to-encoding]
        exit 1
    fi
    if [ "$1" != "GBK" ] && [ "$1" != "UTF-8" ]; then
        echo Such encoding is not supported
        echo Usage:
        echo   ./changeEncoding.sh [from_encoding] [to-encoding]
        exit 1
    fi
    if [ "$2" != "GBK" ] && [ "$2" != "UTF-8" ]; then
        echo Such encoding is not supported
        echo Usage:
        echo   ./changeEncoding.sh [from_encoding] [to-encoding]
        exit 1
    fi
}

function wentWrong() {
    echo "In file $1, something went wrong"
}

# -- main -- #
checkArgs $1 $2
echo "To encoding: ${to_encoding}"
for fileName in "${filesToConvert[@]}"
do
    if [ "${from_encoding}" != "${to_encoding}" ]; then
        iconv ${fileName} --from-code=${from_encoding} \
        --to-code=${to_encoding} --output=tempWorking.cpp || \
        wentWrong ${fileName}
        mv tempWorking.cpp ${fileName}
    fi
done
