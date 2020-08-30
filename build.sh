#!/bin/bash
# build, run, install uninstall
instruction="build"
lang="English"
buildType="Release"
buildDir="build"

# function checkRoot() {
#     if [ $EUID -ne 0 ]; then
#         echo "Please run $0 as root"
#         exit 1
#     fi
# }

function build() {
    mkdir -p $1
    cd $1
    cmake .. -DSTARS_LANGUAGE_SETIING=${lang} -DCMAKE_BUILD_TYPE=${buildType} \
    -DEXECUTABLE_OUTPUT_PATH=..
    if [ $? -ne 0 ]; then
        cd ..
        return 1
    fi
    make
}

function displayHelp {
    echo "Usage:"
    echo " ./build.sh"
    echo " ./build.sh Chinese"
    echo " ./build.sh English"
    echo " ./build.sh run"
    echo " ./build.sh install"
    echo " ./build.sh uninstall"
    echo " ./build.sh help"
}

# -- main -- #
# if have a CMakeCache that dose not match
if [ -f ${buildDir}/CMakeCache.txt ]; then
    if [ `grep CMAKE_HOME_DIRECTORY ${buildDir}/CMakeCache.txt | cut -d "=" -f2` != `pwd` ]; then
        buildDir="lbuild"
    fi
fi

if [ $# -gt 0 ]; then
    if [ $# -gt 1 ]; then
        echo "$0 : too many args"
        displayHelp ./build.sh
        exit 1
    fi
    if [ $1 == "Chinese" ]; then
        lang=$1
        elif [ $1 != "English" ]; then
        instruction=$1
    fi
fi

if [ ${instruction} == "help" ];then
    displayHelp ./build.sh
    exit 0
fi

if [ ${instruction} == "build" ]; then
    build ${buildDir}
    elif [ ${instruction} == "run" ]; then
    ./stars
    elif [ ${instruction} == "install" ]; then
    cd ${buildDir}
    sudo make install
    elif [ ${instruction} == "uninstall" ]; then
    sudo rm /usr/local/bin/stars
else
    echo "./build.sh: no such option -- '${instruction}'"
    displayHelp ./build.sh
    exit 1
fi
