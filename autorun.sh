#!/bin/bash
for file in ./res/*.BMP
do
    ./bin/Image_Proc_Demo ${file}
    echo -e "In "${file}"\n"
done