#include "LPModel/utils/compUtils.h"

using namespace LPModel;

double Utils::sumSQC(const DigitalSet& ds)
{
    using namespace GEOC::API::GridCurve;
    double h=1.0;

    DIPaCUS::Misc::Curve curve;
    DIPaCUS::Misc::computeBoundaryCurve(curve,ds);

    DGtal::Z2i::KSpace kspace;
    kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);

    Curvature::EstimationsVector ev;
    GEOC::Estimator::Standard::IICurvatureExtraData extraData(true,5);
    Curvature::identityOpen<Curvature::EstimationAlgorithms::ALG_II>(kspace,curve.begin(),curve.end(),ev,h,&extraData);

    double s = 0;
    for(auto it=ev.begin();it!=ev.end();++it)
    {
        s+=pow(*it,2);
    }

    return s;

}