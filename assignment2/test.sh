#!/bin/bash

for i in {1..100}
do
    ./withLocks $i ~comp3430/shared/a2_files/*.txt
    ./withFIFOs $i ~comp3430/shared/a2_files/*.txt
    rm -rf 
done

# for i in {1..100}
# do
#     ./serial ~comp3430/shared/a2_files/*.txt
#     ./withFIFOs $i ~comp3430/shared/a2_files/*.txt
#     ./withFIFOs $i ~comp3430/shared/a2_files/*.txt
# done