# IMAGE_PROC_DEMO

An image processing demo for NXP smart car competition (under construction)

---

## Build

    # in project root

    mkdir build && cd build

    cmake ..

    make -j16 -l16      # 16 is the number of threads in your CPU

## Run

    # in project root

    ./bin/Image_Proc_Demo "./res/new/01.BMP"       # or Image_Proc_Demo.exe in Windows

## Autorun

    # in project root

    sh ./autorun.sh     # POSIX

    ./autorun.bat       # Windows

### To be continued...