#!/bin/bash

comp_time() {
    local start=${EPOCHREALTIME/./}
    "$@"
    local exit_code=$?
    local finish=${EPOCHREALTIME/./}
    echo >&2 "Took ~$(( (finish - start) / 1000 )) ms."
    return ${exit_code}
}


DEBUG=false
CLEAN=false
HELP=false
OPTIMIZE=false
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
    -O / --optimize ->  adds optimalization flags such as -O3, -march, ... 
    "
    exit 0
fi

# Set file locations
BIN_FILES_LOCATION="bin"

# Set compiling flags for gcc
FLAGS=(-fopenmp -lm )

# Set the source file and output binary names
SOURCE_FILE="main.c"
OUTPUT_BINARY="apk"

# Check if source file is present
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: Source file '$SOURCE_FILE' not found!"
    exit 1
fi

echo "Source file present"

if $DEBUG; then
    FLAGS+=(-g)
    echo "Debug enabled."
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

comp_time gcc "$SOURCE_FILE" -o "$BIN_FILES_LOCATION/$OUTPUT_BINARY" "${FLAGS[@]}"
    

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful."
else
    echo "Compilation failed."
    exit 1
fi
