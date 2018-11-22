#ifndef LPMODEL_TERM_H
#define LPMODEL_TERM_H

#include <map>
#include "LPModel/utils/model/MultiIndex.h"

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            struct Term
            {
                typedef unsigned long Index;
                typedef Utils::MultiIndex<Index> UIntMultiIndex;

                typedef std::map<UIntMultiIndex,double> UnaryMap;
                typedef std::map<UIntMultiIndex,double> BinaryMap;
                typedef std::map<UIntMultiIndex,double> TernaryMap;

                Term(const UnaryMap& unaryMap,
                     const BinaryMap& binaryMap,
                     const TernaryMap& ternaryMap):unaryMap(unaryMap),
                                                   binaryMap(binaryMap),
                                                   ternaryMap(ternaryMap)
                {}

                const UnaryMap unaryMap;
                const BinaryMap binaryMap;
                const TernaryMap ternaryMap;
            };
        }
    }
}

#endif //LPMODEL_TERM_H
