#ifndef LPMODEL_TERMS_SQC_TERM_H
#define LPMODEL_TERMS_SQC_TERM_H

#include <map>
#include "MultiIndex.h"

namespace LPModel
{
    namespace Terms
    {
        struct Term
        {
            typedef unsigned long Index;
            typedef Utils::MultiIndex<Index> UIntMultiIndex;

            typedef std::map<UIntMultiIndex,double> GeneralMap;
            
            typedef std::map<UIntMultiIndex,double> UnaryMap;
            typedef std::map<UIntMultiIndex,double> BinaryMap;
            typedef std::map<UIntMultiIndex,double> TernaryMap;

            typedef std::map<std::string,double> ConstantsMap;
            
            typedef std::pair<UIntMultiIndex,double> Element;

            Term(const UnaryMap& unaryMap,
                 const BinaryMap& binaryMap,
                 const TernaryMap& ternaryMap,
                 const ConstantsMap& constants):unaryMap(unaryMap),
                                               binaryMap(binaryMap),
                                               ternaryMap(ternaryMap),
                                             constants(constants)
            {}

            const UnaryMap unaryMap;
            const BinaryMap binaryMap;
            const TernaryMap ternaryMap;
            const ConstantsMap constants;
        };
    }
}

#endif //LPMODEL_TERMS_SQC_TERM_H
