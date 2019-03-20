#!/bin/bash
for file in ./res/new/*.BMP
do
    ./bin/Image_Proc_Demo ${file}
    echo -e "In "${file}"\n"
done