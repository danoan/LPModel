#include <DGtal/helpers/StdDefs.h>
#include "geoc/api/gridCurve/Curvature.hpp"
#include "LPModel/initialization/Shapes.h"

using namespace LPModel;

typedef DGtal::Z2i::Curve Curve;
typedef GEOC::API::GridCurve::Curvature::EstimationsVector EstimationVector;
typedef DGtal::Z2i::DigitalSet DigitalSet;

template<typename T>
using ALG_CURV = GEOC::API::GridCurve::Curvature::EstimationAlgorithms::ALG_MDCA<T> ;

double sumSqc(const DigitalSet& ds)
{
    DIPaCUS::Representation::Image2D imgDS(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(imgDS,ds);

    Curve curve;
    DIPaCUS::Misc::computeBoundaryCurve(curve,imgDS,1);

    KSpace KImage;
    KImage.init( ds.domain().lowerBound(),ds.domain().upperBound(),true);

    double sumSQC=0;
    EstimationVector estimations;
    GEOC::API::GridCurve::Curvature::identityClosed<ALG_CURV>(KImage,curve.begin(),curve.end(),estimations,1.0);
    for(auto it=estimations.begin();it!=estimations.end();++it) sumSQC+=pow(*it,2);

    return sumSQC;
}

int main(int argc, char* argv[])
{


    DigitalSet square1 = Initialization::Shapes::square(1.0);
    DigitalSet square2 = Initialization::Shapes::square(0.5);
    DigitalSet square4 = Initialization::Shapes::square(0.25);

    std::cout << square1.size() << std::endl;
    std::cout << square2.size() << std::endl;
    std::cout << square4.size() << std::endl;

    std::cout << "Square (h=1.0): " << sumSqc(square1) << "\n"
              << "Square (h=0.5): " << sumSqc(square2) << "\n"
              << "Square (h=0.25): " << sumSqc(square4) << "\n";


    return 0;
}