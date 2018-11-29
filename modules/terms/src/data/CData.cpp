#include "LPModel/terms/data/CData.h"

using namespace LPModel::Terms::DataFidelity;

LPModel::Terms::Term API::prepare(const Parameters &prm, const Grid &grid, double weight)
{
    Term::UnaryMap um;
    Term::BinaryMap bm;
    Term::TernaryMap tm;

    typedef DGtal::Z2i::DigitalSet DigitalSet;


    //Assume that ODRInterpixels evenIteration attributed is
    //manually set to true
    const DigitalSet& optRegion = prm.odrModel.optRegion;
    DigitalSet frgInOptRegion(optRegion.domain());
    DIPaCUS::SetOperations::setIntersection(frgInOptRegion,optRegion,prm.odrModel.trustBKG);

    double unaryCoefficient = weight*(1.0 - 2.0*frgInOptRegion.size());
    for(auto it1=optRegion.begin();it1!=optRegion.end();++it1)
    {
        Term::UIntMultiIndex unaryIndex;
        unaryIndex << grid.pixelMap.at( *it1 ).varIndex;
        um.insert( Term::Element(unaryIndex,unaryCoefficient) );

        for(auto it2=it1;it2!=optRegion.end();++it2)
        {
            Term::UIntMultiIndex binaryIndex;
            binaryIndex << grid.pixelMap.at( *it1 ).varIndex;
            binaryIndex << grid.pixelMap.at( *it2 ).varIndex;

            bm.insert( Term::Element(binaryIndex,weight*2));
        }
    }

    return Term(um,bm,tm);
}
