#include "LPModel/terms/sqc/control/CConstants.h"

using namespace LPModel::Terms::SquaredCurvature;

Constants CConstants::setConstants(const Parameters &prm)
{
    typedef SCaBOliC::Energy::ISQ::LinelCoefficientsComputer LinelCoefficientsComputer;
    typedef SCaBOliC::Energy::ISQ::CoefficientData CoefficientData;
    typedef SCaBOliC::Energy::ISQ::InputData::PenalizationMode  PenalizationMode;

    LinelCoefficientsComputer lcc(prm.odrModel.applicationRegion,
                                  prm.odrModel.trustFRG,
                                  prm.odrModel.optRegion,
                                  &prm.handle,
                                  PenalizationMode::No_Penalization,
                                  false);

    double F = lcc.scalingFactor();
    double R = prm.radius;
    double C = prm.handle.pixelArea()/2.0;
    double W = lcc.constantTerm();

    const CoefficientData& cd =  lcc.data();

    return Constants(W,C,F,R,cd.constants,cd.unary,cd.pairwise);
}