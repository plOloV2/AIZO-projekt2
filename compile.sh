#!/bin/bash

comp_time() {
    local start=$(date +%s%3N)
    "$@"
    local exit_code=$?
    local finish=$(date +%s%3N)
    echo >&2 "Took ~$(( finish - start )) ms."
    return ${exit_code}
}


DEBUG=false
CLEAN=false
HELP=false
OPTIMIZE=false
TEST=false
while [[ $# -gt 0 ]]; do
    case "$1" in
        -d|--debug)
            DEBUG=true
            shift ;;
        -c|--clean)
            CLEAN=true
            shift ;;
        -O|--optimize)
            OPTIMIZE=true
            shift;;
        -t|--test)
            TEST=true
            shift;;
        -h|--help)
            HELP=true
            shift;;
        *)
            echo "Unknown option: $1"
            exit 1 ;;
    esac
done

if $HELP; then
    echo "Script to compile this project with gcc. All available flags:
    -d / --debug    ->  adds -g to gcc flags
    -c / --clean    ->  cleans /bin directory
    -t / --test     ->  builds test version of project
    -O / --optimize ->  adds optimalization flags such as -O3, -march, ... 
    "
    exit 0
fi

# Set file locations
BIN_FILES_LOCATION="bin"

# Collect all .c files from lib/ and algorithms/
shopt -s nullglob
LIB_FILES=(lib/*.c)
ALGO_FILES=(algorithms/*.c)
shopt -u nullglob

# Set compiling flags for gcc
FLAGS=(-fopenmp -lm)
FLAGS+=(-Wall -Wextra -Werror)

# Set the source file and output binary names
if $TEST; then
    SOURCE_FILE="test.c"
    OUTPUT_BINARY="test_apk"
    FLAGS+=(-DTEST)
    echo "Test enabled."
else
    SOURCE_FILE="main.c"
    OUTPUT_BINARY="apk"
fi

# Check if source file is present
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: Source file '$SOURCE_FILE' not found!"
    exit 1
fi

echo "Source file present"

if $DEBUG; then
    FLAGS+=(-g)
    echo "Debug enabled."
    echo "Included source files from lib/: ${LIB_FILES[*]}"
    echo "Included source files from algorithms/: ${ALGO_FILES[*]}"
fi

mkdir -p "$BIN_FILES_LOCATION"

if $CLEAN; then
    echo "Cleaning build directory"
    rm -rf "$BIN_FILES_LOCATION"/*
fi


if $OPTIMIZE; then
    FLAGS+=(-O3 -march=native -mtune=native -flto -fomit-frame-pointer)
    echo "Added special flags for better optimalization"
fi

echo "Compiling program with flags: ${FLAGS[@]}"

FLAGS+=("${LIB_FILES[@]}" "${ALGO_FILES[@]}")

comp_time gcc "$SOURCE_FILE" -o "$BIN_FILES_LOCATION/$OUTPUT_BINARY" "${FLAGS[@]}"
    

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful."
else
    echo "Compilation failed."
    exit 1
fi
