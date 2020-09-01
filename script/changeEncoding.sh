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
    if [ "$1" != "gbk" ] && [ "$1" != "utf-8" ]; then
        echo Encoding $1 is not supported
        echo Usage:
        echo   ./changeEncoding.sh [from_encoding] [to-encoding]
        exit 1
    fi
    if [ "$2" != "gbk" ] && [ "$2" != "utf-8" ]; then
        echo Encoding $2 is not supported
        echo Usage:
        echo   ./changeEncoding.sh [from_encoding] [to-encoding]
        exit 1
    fi
}

# check if files is all utf-8 and won't need to convert
function getFileEncoding() {
    file -i $1 | cut -d "=" -f2
}

function wentWrong() {
    echo "In file $1, something went wrong"
}

# -- main -- #
checkArgs $1 $2
if [ "${from_encoding}" == "${to_encoding}" ]; then
    exit 0
fi
echo "To encoding: ${to_encoding}"
for fileName in "${filesToConvert[@]}"
do
    if [ `getFileEncoding ${fileName}` != ${to_encoding} ]; then
        echo Current `getFileEncoding ${fileName}` doesnt match with ${to_encoding} as expected
        iconv ${fileName} --from-code=${from_encoding} \
        --to-code=${to_encoding} --output=tempWorking.cpp || \
        wentWrong ${fileName}
        mv tempWorking.cpp ${fileName}
        else
        echo "they match, do nothing"
    fi
done
