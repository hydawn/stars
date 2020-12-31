#!/bin/bash
# build, run, install uninstall
instruction="build"
lang="English"
buildType="Release"
buildDir="build"

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
    echo "  ./build.sh [instruction] [build_type] [language]"
    echo "             ------------- ------------ ----------"
    echo "             build         Release      English    - default"
    echo "             run           Debug        Chinese"
    echo "             install"
    echo "             uninstall"
    echo "             help"
    echo ""
    echo "Every option can be omitted and set to default"
}

# -- main -- #
# if have a CMakeCache that dose not match
if [ -f ${buildDir}/CMakeCache.txt ]; then
    if [ `grep CMAKE_HOME_DIRECTORY ${buildDir}/CMakeCache.txt | cut -d "=" -f2` != `pwd` ]; then
        buildDir="lbuild"
    fi
fi

# set instruction, buildType and language
if [ $# -gt 0 ]; then
    if [ $# -gt 2 ]; then
        echo "./build.sh : too many args"
        displayHelp ./build.sh
        exit 1
    fi
    # set type
    if [ $1 == "Debug" ]; then
        buildType=$1
    elif [ $1 == "Chinese" ]; then
        lang=$1
    elif [ $1 != "English" ] && [ $1 != "Release" ]; then
        instruction=$1
    fi
    # set lang
    if [ $# -eq 2 ]; then
        if [ $2 == "Chinese" ]; then
            lang=$2
        elif [ $2 != "English" ]; then
            echo "./build.sh: no such language option: $2"
            displayHelp ./build.sh
            exit 1
        fi
    fi
fi

echo ${instruction} ${buildType} ${lang} 

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
