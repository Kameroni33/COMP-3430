#!/bin/bash

# clean up any old tests...
rm -f /tmp/ronaldk1/output/* /tmp/ronaldk1/fifo/* /tmp/ronaldk1/log/*

# Test with various number of workers
for i in {1..100}
do
    ./withLocks $i ~comp3430/shared/a2_files/*.txt
    ./withFIFOs $i ~comp3430/shared/a2_files/*.txt
    # clean up output files so we don't run out of space
    rm -f /tmp/ronaldk1/output/*
done

# Test each program 100 times
for i in {1..100}
do
    ./serial ~comp3430/shared/a2_files/*.txt
    ./withLocks 10 ~comp3430/shared/a2_files/*.txt
    ./withFIFOs 10 ~comp3430/shared/a2_files/*.txt
    # clean up output files so we don't run out of space
    rm -f /tmp/ronaldk1/output/*
done
