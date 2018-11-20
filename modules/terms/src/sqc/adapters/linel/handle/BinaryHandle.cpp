#include "LPModel/terms/sqc/adapters/linel/handle/BinaryHandle.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;

void BinaryHandle::operator()(const Point &linel,
                              const Point &pointel,
                              const Point &pixel)
{
    PointelContribution::PointMultiIndex pointelMI;
    pointelMI << pointel << pixel;

    LinelContribution::PointMultiIndex linelMI;
    linelMI << linel << pixel;

    if(this->bm.find(linelMI)==this->bm.end()) this->bm[linelMI]=0;
    this->bm[linelMI]+=this->pctbr.binaryMap.at(pointelMI);

}