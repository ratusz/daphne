#!/usr/bin/env bash

#******************************************************************************
# Parse arguments
#******************************************************************************

# Defaults.
target="daphne"
export BUILD_CUDA="-DUSE_CUDA=OFF"
export BUILD_DEBUG="-DCMAKE_BUILD_TYPE=Release"

while [[ $# -gt 0 ]]
do
    key=$1
    shift
    case $key in
        -h|--help)
            printHelp
            exit 0
            ;;
        --clean)
            cleanBuildDirs
            exit 0
            ;;
        --target)
            target=$1
            shift
            ;;
        --cuda)
            echo using CUDA
            export BUILD_CUDA="-DUSE_CUDA=ON"
            ;;
        --debug)
            echo building DEBUG version
            export BUILD_DEBUG="-DCMAKE_BUILD_TYPE=Debug"
            ;;
        *)
            printf "Unknown option: '%s'\n\n" "$key"
            printHelp
            exit 1
            ;;
    esac
done;

installPrefix="$PWD"/thirdparty/installed
#antlrVersion=4.9.2
#antlrJarName=antlr-${antlrVersion}-complete.jar
#-DANTLR4_JAR_LOCATION=$installPrefix/share/antlr4/$antlrJarName

cmake -S . -B build -G Ninja $BUILD_CUDA $BUILD_DEBUG -DCMAKE_PREFIX_PATH="$installPrefix"

cmake --build build --target "$target"
