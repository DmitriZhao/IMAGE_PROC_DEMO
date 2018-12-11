#include <iostream>
#include "ImageReader.hpp"
#include "ImageAnalyzer.h"
#include "LaneAnalyzer.h"

using namespace std;

int main(int argc, char** argv)
{
    ImageReader imgReader; 
    imgReader.init(argv[1]);
    
    ImageAnalyzer imgAnalyzer = ImageAnalyzer(imgReader.getImage());
    imgAnalyzer.evalThreshold(HEIGHT/2);

    LaneAnalyzer laneAnalyzer = LaneAnalyzer(imgReader.getImage());
    laneAnalyzer.findPath();
    laneAnalyzer.show();
    laneAnalyzer.showLaneType();

    //cout << "Press any key to exit...";
    //cin.get();

    return 0;
}