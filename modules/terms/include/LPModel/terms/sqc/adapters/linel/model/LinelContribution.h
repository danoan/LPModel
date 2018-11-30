#ifndef LPMODEL_TERMS_SQC_LINELCONTRIBUTION_H
#define LPMODEL_TERMS_SQC_LINELCONTRIBUTION_H

#include <DGtal/helpers/StdDefs.h>
#include <LPModel/terms/model/MultiIndex.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace Internal
            {
                namespace Linel
                {
                    struct LinelContribution
                    {
                        typedef DGtal::Z2i::Point Point;
                        typedef Utils::MultiIndex<Point> PointMultiIndex;

                        typedef std::map<PointMultiIndex,double> UnaryMap;
                        typedef std::map<PointMultiIndex,double> BinaryMap;
                        typedef std::map<PointMultiIndex,double> TernaryMap;

                        LinelContribution(const UnaryMap& unaryMap,
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

#endif //LPMODEL_TERMS_SQC_LINELCONTRIBUTION_H
