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
    FileReaderInstance::Ptr fileReader(new FileReaderInstance(argv[1]));
    if(!fileReader->available())
    {
        cerr<<"BMP read failed, aborting"<<endl;
        return 1;
    }
    BmpReader::Ptr bmpReader(new BmpReader(fileReader));

    GreyScaleImage::Ptr img(new GreyScaleImage(bmpReader));
    //img->show();
    img->evalThreshold(HEIGHT/2);
    //img->show();
    
    ImageAnalyzer::Ptr imageAnalyzer(new ImageAnalyzer(img));
    imageAnalyzer->findPath();
    imageAnalyzer->show();

    //cin.get();

    return 0;
}