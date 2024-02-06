#!/bin/bash

# Explicitly call parser sciprt because add_custom_command does not play nice with idf framework
python3 components/server/parser.py
if [ $? -eq 0 ]
then
    tmp="tmp"
else 
    exit -1
fi

# Build the project and flash it and monitor the seria output
idf.py build flash monitor