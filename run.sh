#!/bin/bash

echo "Compiling project..."
./compile.sh -O -c
if [ $? -ne 0 ]; then
    echo "Compilation failed, exiting."
    exit 1
fi

# Set CPU frequency for cores 4-7 to 2.2GHz by forcing userspace governor
echo "Setting CPU frequency for cores 4-7 to 2GHz..."
for cpu in 4 5 6 7; do
    sudo cpufreq-set -c $cpu -g userspace
    sudo cpufreq-set -c $cpu -f 2GHz
    if [ $? -ne 0 ]; then
         echo "Failed to set CPU frequency for core $cpu"
         exit 1
    fi
done

# Launch the project using taskset on cores 4-7
echo "Launching project on cores 4-7..."
taskset -c 4-7 bin/apk 
EXIT_STATUS=$?

echo "Computation completed. Exit code $EXIT_STATUS"

# Restore the ondemand governor for cores 4-7
echo "Restoring original CPU governors..."
for cpu in 4 5 6 7; do
    sudo cpufreq-set -c $cpu -g ondemand
    if [ $? -ne 0 ]; then
         echo "Failed to restore governor for core $cpu"
         exit 1
    fi
done

exit $EXIT_STATUS
 
