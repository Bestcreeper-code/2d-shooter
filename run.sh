#!/bin/bash

make -j4
if [ $? -ne 0 ]; then
    printf "\e[31mBuild failed\e[0m\n"
    exit 1
fi
./Game