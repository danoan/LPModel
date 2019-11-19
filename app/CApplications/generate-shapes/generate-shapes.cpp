#include <boost/filesystem/operations.hpp>
#include <DIPaCUS/components/Transform.h>
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/writers/GenericWriter.h"

#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/base/Representation.h"

typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DIPaCUS::Representation::Image2D Image2D;

enum ShapeType{Triangle,Square,Pentagon,Heptagon,Ball,Ellipse,Flower,Wave,UserDefined,NoType};

struct Shape
{
    Shape():type(NoType),imagePath(""),name(""){}

    Shape(ShapeType type, const std::string& imagePath=""):type(type),
                                                           imagePath(imagePath)
    {
        if(type==ShapeType::Triangle) name = "triangle";
        else if(type==ShapeType::Square) name = "square";
        else if(type==ShapeType::Pentagon) name =  "pentagon";
        else if(type==ShapeType::Heptagon) name = "heptagon";
        else if(type==ShapeType::Ball) name = "ball";
        else if(type==ShapeType::Ellipse) name = "ellipse";
        else if(type==ShapeType::Flower) name = "flower";
        else if(type==ShapeType::Wave) name = "wave";
        else name = "user-defined";
    }

    ShapeType type;
    std::string imagePath;
    std::string name;
};

DigitalSet resolveShape(Shape shape,double gridStep)
{
    using namespace DGtal::Z2i;

    int radius=20;
    if(shape.type==ShapeType::Triangle) return DIPaCUS::Shapes::triangle(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Square) return DIPaCUS::Shapes::square(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Pentagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,5);
    else if(shape.type==ShapeType::Heptagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,7);
    else if(shape.type==ShapeType::Ball) return DIPaCUS::Shapes::ball(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Flower) return DIPaCUS::Shapes::flower(gridStep,0,0,radius,radius/2.0,2);
    else if(shape.type==ShapeType::Ellipse) return DIPaCUS::Shapes::ellipse(gridStep,0,0,radius,radius/2);
    else if(shape.type==ShapeType::Wave) return DIPaCUS::Shapes::wave(gridStep,1200,radius*3,radius*6,0.01);
    else
    {
        cv::Mat img = cv::imread(shape.imagePath,CV_8UC1);
        Domain domain( DGtal::Z2i::Point(0,0), DGtal::Z2i::Point(img.cols-1,img.rows-1) );
        DigitalSet ds(domain);
        DIPaCUS::Representation::CVMatToDigitalSet(ds,img,1);
        return ds;
    }
}


void exportAsImageFile(const DGtal::Z2i::DigitalSet& ds, const std::string& outputFilename)
{
    std::cerr << "Generating " << outputFilename << "\n";
    Image2D dsImg(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(dsImg,ds);
    DGtal::GenericWriter<Image2D>::exportFile( outputFilename, dsImg);
}

DigitalSet resolveShape(const std::string& shapeStr,double gridStep)
{
    Shape shape;

    if(shapeStr=="triangle") shape = Shape( ShapeType::Triangle);
    else if(shapeStr=="square") shape = Shape( ShapeType::Square );
    else if(shapeStr=="pentagon") shape = Shape( ShapeType::Pentagon);
    else if(shapeStr=="heptagon") shape = Shape( ShapeType::Heptagon);
    else if(shapeStr=="ball") shape = Shape( ShapeType::Ball);
    else if(shapeStr=="ellipse") shape = Shape( ShapeType::Ellipse);
    else if(shapeStr=="flower") shape = Shape( ShapeType::Flower);
    else if(shapeStr=="wave") shape = Shape( ShapeType::Wave);
    else shape = Shape(ShapeType::UserDefined,shapeStr);

    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin( resolveShape(shape,gridStep),DGtal::Z2i::Point(20,20) );

}

int main(int argc, char* argv[])
{

    if(argc<3)
    {
        std::cerr << "Expected output-path shape grid-step\n";
        exit(1);
    }

    std::string outputPath = argv[1];
    std::string shapeStr = argv[2];
    double h = atof(argv[3]);



    boost::filesystem::path p(outputPath);
    boost::filesystem::create_directories(p.remove_filename());

    DigitalSet shape = resolveShape(shapeStr,h);
    exportAsImageFile(shape,outputPath);


    return 0;
}