#include <iostream>
#include "ImageReceiver.hpp"
#include "ImageReader.hpp"
#include "ImageAnalyzer.h"
#include "LaneAnalyzer.h"

using namespace std;

int main(int argc, char** argv)
{
    ImageReader imgReader;
    imgReader.init(argv[1]);

// TODO: Interface to the Camera
//    ImageReceiver imgReceiver;
//    imgReceiver.init(MT9V032_ImageBuffer);
//    ImageAnalyzer imgAnalyzer = ImageAnalyzer(imgReceiver.getImage());

    ImageAnalyzer imgAnalyzer = ImageAnalyzer(imgReader.getImage());
    imgAnalyzer.evalThreshold(HEIGHT/2);

    LaneAnalyzer laneAnalyzer = LaneAnalyzer(imgReader.getImage());
    laneAnalyzer.findPath();
    laneAnalyzer.show();
    laneAnalyzer.showLaneType();

    cout << "Press any key to continue...";
    cin.get();

    return 0;
}