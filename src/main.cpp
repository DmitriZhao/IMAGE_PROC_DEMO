#include <iostream>
#include <vector>
#include <fstream>
#include "Types.h"
#include "FileReaderInstance.hpp"
#include "BmpReader.hpp"
#include "GreyScaleImage.h"
#include "ImageAnalyzer.h"

using namespace std;

int main(int argc, char** argv)
{
    FileReaderInstance* fileReader = new FileReaderInstance(argv[1]);
    if(!fileReader->available())
    {
        cerr<<"BMP read failed, aborting"<<endl;
        return 1;
    }
    BmpReader* bmpReader = new BmpReader(fileReader);   

    GreyScaleImage* img = new GreyScaleImage(bmpReader);
    //img->show();
    img->evalThreshold(HEIGHT/2);
    //img->show();
    
    ImageAnalyzer* imageAnalyzer = new ImageAnalyzer(img);
    imageAnalyzer->findPath();
    imageAnalyzer->show();

    //cin.get();
    delete imageAnalyzer;
    delete img;
    delete bmpReader;
    delete fileReader;
    return 0;
}