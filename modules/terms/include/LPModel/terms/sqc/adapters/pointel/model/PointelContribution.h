#ifndef LPMODEL_TERMS_SQC_POINTELCONTRIBUTION_H
#define LPMODEL_TERMS_SQC_POINTELCONTRIBUTION_H

#include <map>
#include <DGtal/helpers/StdDefs.h>
#include "LPModel/utils/model/MultiIndex.h"

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace Internal
            {
                namespace Pointel
                {
                    struct PointelContribution
                    {
                        typedef DGtal::Z2i::Point Point;
                        typedef Utils::MultiIndex<Point> PointMultiIndex;
    
                        typedef std::map<PointMultiIndex,double> UnaryMap;
                        typedef std::map<PointMultiIndex,double> BinaryMap;
                        typedef std::map<PointMultiIndex,double> TernaryMap;
    
                        PointelContribution(const UnaryMap& unaryMap,
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
    }
}

#endif //LPMODEL_TERMS_SQC_POINTELCONTRIBUTION_H
