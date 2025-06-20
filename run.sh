#!/bin/bash

set -e  # exit on any unhandled error

# Ensure we restore governors on exit or interruption
trap 'echo "Restoring ondemand governor..."; \
     for cpu in 4 5 6 7; do sudo cpufreq-set -c $cpu -g ondemand; done' EXIT

echo "Compiling project..."
./compile.sh -O -c -i

echo "Setting CPU frequency for cores 4-7 to 1.8GHz..."
for cpu in 4 5 6 7; do
    sudo cpufreq-set -c $cpu -g userspace
    sudo cpufreq-set -c $cpu -f 1.8GHz
done

echo "Launching project on cores 4-7..."
taskset -c 4-7 bin/apk 128 256 512 1024 2048 4096 8192
EXIT_STATUS=$?

echo "Computation completed. Exit code $EXIT_STATUS"

# (Governors will be restored automatically by the trap on script exit)
exit $EXIT_STATUS
