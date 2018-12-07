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
    // imgReader.getImage()->show(byThreshold);
    //imgAnalyzer.getGradMap(ImageAnalyzer::RUA)->show(byHex);
    // GreyScaleImage::Ptr gradImg_sobel  = imgAnalyzer.getGradMap(ImageAnalyzer::SOBEL);
    // GreyScaleImage::Ptr gradImg_scharr = imgAnalyzer.getGradMap(ImageAnalyzer::SCHARR);

    // ImageAnalyzer gradAnalyzer_sobel(gradImg_sobel);
    // ImageAnalyzer gradAnalyzer_scharr(gradImg_scharr);
    
    // gradAnalyzer_sobel.evalThreshold(HEIGHT/2);
    // gradAnalyzer_scharr.evalThreshold(HEIGHT/2);

    ///gradImg_sobel->show(byHex);
    // gradImg_scharr->show(byHex);

    LaneAnalyzer laneAnalyzer = LaneAnalyzer(imgReader.getImage());
    laneAnalyzer.findPath();
    laneAnalyzer.show();

    // cout << "Press any key to exit...";
    // cin.get();

    return 0;
}