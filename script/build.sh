#!/bin/bash
lang="English"
buildType="Release"
buildDir="build"
ins="not-given"

function displayHelp {
    echo "Usage:"
    echo "  ./build.sh [instruction] [build_type] [language]"
    echo "             ------------- ------------ ----------"
    echo "             build         Release      English    - default"
    echo "                           Debug        Chinese"
    echo "             run"
    echo "             install"
    echo "             uninstall"
    echo "             help"
    echo ""
}

# -- main -- #

# move to workspace folder
cd `dirname "$0"`/..

# if have a CMakeCache that dose not match
if [ -f ${buildDir}/CMakeCache.txt ]; then
    if [ `grep CMAKE_HOME_DIRECTORY ${buildDir}/CMakeCache.txt | cut -d "=" -f2` != `pwd` ]; then
        buildDir="lbuild"
    fi
fi

if [ $# -gt 0 ]; then
    if [ $# -gt 3 ]; then
        echo "./build.sh : too many args"
        exit 1
    fi

    # other instrutions
    if [ "help" == $1 ]; then
        displayHelp
    elif [ "run" == $1 ]; then
        ./build/stars
    elif [ "install" == $1 ]; then
        cd ${buildDir}
        sudo make install
    elif [ "uninstall" == $1 ]; then
        sudo rm /usr/local/bin/stars
    else
        ins=build
    fi

    # set build type
    for (( i=$1; i <= "$#"; ++i )); do
        if [ ${!i} == "Chinese" ]; then
            lang=Chinese
        elif [ ${!i} == "Debug" ]; then
            buildType=Debug
        fi
    done
else
    ins=build
fi

# if no instruction is given, build
if [ "build" == $ins ]; then
    mkdir -p $buildDir
    cd $buildDir
    echo $ins $buildType $lang
    cmake .. -DSTARS_LANGUAGE_SETIING=${lang} -DCMAKE_BUILD_TYPE=${buildType} \
        && make
fi