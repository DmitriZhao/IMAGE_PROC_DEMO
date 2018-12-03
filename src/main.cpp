#include <iostream>
#include <vector>
#include <fstream>
#include "Types.h"
#include "GreyScaleImage.h"
#include "BmpReader.h"
#include "FileReaderInstance.h"
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

    GreyScaleImage img(bmpReader);
    //img.show();
    img.evalThreshold(HEIGHT/2);
    //img.show();
    
    ImageAnalyzer imageAnalyzer(&img);
    imageAnalyzer.findPath();
    imageAnalyzer.show();

    //cin.get();
    delete bmpReader;
    delete fileReader;
    return 0;
}