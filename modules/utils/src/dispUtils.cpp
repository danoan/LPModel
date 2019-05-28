#include "LPModel/utils/dispUtils.h"

using namespace LPModel;

void Utils::exportODRModel(const Parameters &prm, const std::string& outputPath)
{
    SCaBOliC::Core::Display::DisplayODR(prm.odrModel,outputPath);
}

Utils::DigitalSet Utils::loadImageAsDigitalSet(const std::string& imageFilePath)
{
    typedef DIPaCUS::Representation::Image2D Image2D;
    std::cerr << "Load Image as DigitalSet\n";

    Image2D imgInput = DGtal::GenericReader<Image2D>::import(imageFilePath);
    DigitalSet ds(imgInput.domain());
    DIPaCUS::Representation::imageAsDigitalSet(ds,imgInput);

    return ds;
}



void Utils::exportImageFromDigitalSet(const DigitalSet& ds,
                                      const std::string& outputFilepath)
{
    typedef DIPaCUS::Representation::Image2D Image2D;
    typedef DGtal::Z2i::Point Point;
    typedef DGtal::Z2i::Domain Domain;

    Point lb,ub;
    ds.computeBoundingBox(lb,ub);
    lb+=Point(-10,-10);
    ub+=Point(10,10);
    DIPaCUS::Representation::Image2D image( Domain(lb,ub) );
    DIPaCUS::Representation::digitalSetToImage(image, ds);

    DGtal::GenericWriter<Image2D>::exportFile(outputFilepath, image);
}