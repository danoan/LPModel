#include "LPModel/terms/API.h"

using namespace LPModel::Terms;

void API::mergeMap(Term::GeneralMap& mergedMap,
                   const Term::GeneralMap& inputMap1,
                   const Term::GeneralMap& inputMap2)
{
    mergedMap = inputMap1;
    for(auto it=inputMap2.begin();it!=inputMap2.end();++it)
    {
        mergedMap[ it->first ] += it->second;
    }
}

Term API::merge(Term& term1, const Term& term2)
{
    Term::UnaryMap um;
    Term::BinaryMap bm;
    Term::TernaryMap tm;

    mergeMap(um,term1.unaryMap,term2.unaryMap);
    mergeMap(bm,term1.binaryMap,term2.binaryMap);
    mergeMap(tm,term1.ternaryMap,term2.ternaryMap);

    return Term(um,bm,tm);
}

