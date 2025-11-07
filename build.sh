#!/bin/sh
mkdir build
cd build
cmake322 .. 
if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi
cmake322 --build . 
if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi
