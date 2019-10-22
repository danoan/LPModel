#include <boost/filesystem/operations.hpp>
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/writers/GenericWriter.h"

#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/base/Representation.h"

typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DIPaCUS::Representation::Image2D Image2D;

void exportAsImageFile(const DGtal::Z2i::DigitalSet& ds, const std::string& outputFilename)
{
    std::cerr << "Generating " << outputFilename << "\n";
    Image2D dsImg(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(dsImg,ds);
    DGtal::GenericWriter<Image2D>::exportFile( outputFilename, dsImg);
}

int main(int argc, char* argv[])
{

    if(argc<3)
    {
        std::cerr << "Expected output-path grid-step\n";
        exit(1);
    }

    std::string outputPath = argv[1];
    double h = atof(argv[2]);

    boost::filesystem::create_directories(outputPath);

    DigitalSet square = DIPaCUS::Shapes::square(h);
    DigitalSet triangle = DIPaCUS::Shapes::triangle(h);
    DigitalSet flower = DIPaCUS::Shapes::flower(h);


    exportAsImageFile(square,outputPath + "/square.pgm");
    exportAsImageFile(triangle,outputPath + "/triangle.pgm");
    exportAsImageFile(flower,outputPath + "/flower.pgm");


    return 0;
}