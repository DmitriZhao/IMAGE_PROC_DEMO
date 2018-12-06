#include <iostream>
#include <vector>
#include <fstream>
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

    for(COORD x = 1; x < imgReader.size().x-1; x++)
        for(COORD y = 1; y < imgReader.size().y-1; y++)
            imgAnalyzer.applyOperator(x, y, ImageAnalyzer::SOBEL);

    imgAnalyzer.showGradMap(byThreshold);

    LaneAnalyzer laneAnalyzer = LaneAnalyzer(imgReader.getImage());
    laneAnalyzer.findPath();
    laneAnalyzer.show();

    cout << "Press any key to exit...";
    cin.get();

    return 0;
}