#include "geoc/estimator/standard/Curvature.h"
#include "LPModel/initialization/Shapes.h"

using namespace LPModel;

typedef DGtal::Z2i::DigitalSet DigitalSet;

double sumSqc(const DigitalSet& ds)
{
    typedef DGtal::Z2i::Curve Curve;
    typedef GEOC::Estimator::Standard::IICurvature<Curve::ConstIterator> MyIICurvature;

    DIPaCUS::Representation::Image2D imgDS(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(imgDS,ds);

    Curve curve;
    DIPaCUS::Misc::ComputeBoundaryCurve(imgDS,curve,1);

    double sumSQC=0;
    std::vector<double> estimations;
    MyIICurvature(curve.begin(),curve.end(),estimations,1.0);
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