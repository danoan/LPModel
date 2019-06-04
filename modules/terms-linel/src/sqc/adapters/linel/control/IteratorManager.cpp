#include "LPModel/terms/sqc/adapters/linel/control/IteratorManager.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;

void IteratorManager::run(const Parameters &prm, const BinaryCallback &bcbk, const TernaryCallback &tcbk)
{
    typedef SCaBOliC::Core::SpaceHandleInterface::Intersections Intersections;
    typedef SCaBOliC::Core::IntersectionAttributes IntersectionAttributes;
    typedef SCaBOliC::Energy::ISQ::LinelCoefficientsComputer LinelCoefficientsComputer;
    typedef SCaBOliC::Energy::ISQ::InputData::PenalizationMode PenalizationMode;
    typedef DGtal::Z2i::DigitalSet DigitalSet;
    typedef SCaBOliC::Energy::ISQ::CoefficientData CoefficientData;

    //TODO::Design error: lcc is also computed in CConstants.cpp
    LinelCoefficientsComputer lcc(prm.odrModel.applicationRegion,
                                  prm.odrModel.trustFRG,
                                  prm.odrModel.optRegion,
                                  &prm.handle,
                                  PenalizationMode::No_Penalization,
                                  false);

    const CoefficientData& cd = lcc.data();

    for(auto itU=cd.unary.begin();itU!=cd.unary.end();++itU)
    {
        bcbk(itU->first);
    }

    for(auto itP=cd.pairwise.begin();itP!=cd.pairwise.end();++itP)
    {
        tcbk(itP->first);
    }

}