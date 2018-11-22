#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/writers/GenericWriter.h"

#include "LPModel/initialization/Shapes.h"

using namespace LPModel;

int main(int argc, char* argv[])
{
    typedef DGtal::Z2i::DigitalSet DigitalSet;
    typedef DIPaCUS::Representation::Image2D Image2D;

    if(argc<2)
    {
        std::cerr << "Expected output-path\n";
        exit(1);
    }

    std::string outputPath = argv[1];

    DigitalSet square = Initialization::Shapes::square();
    DigitalSet flower = Initialization::Shapes::flower();

    std::cerr << "Generating Square\n";
    Image2D squareImg(square.domain());
    DIPaCUS::Representation::digitalSetToImage(squareImg,square);
    DGtal::GenericWriter<Image2D>::exportFile( outputPath + "/square.pgm", squareImg);


    std::cerr << "Generating Flower\n";
    Image2D flowerImg(flower.domain());
    DIPaCUS::Representation::digitalSetToImage(flowerImg,flower);
    DGtal::GenericWriter<Image2D>::exportFile( outputPath + "/flower.pgm", flowerImg);

    return 0;
}