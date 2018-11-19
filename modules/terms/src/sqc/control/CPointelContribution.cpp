#include "LPModel/terms/sqc/control/CPointelContribution.h"

using namespace LPModel::Terms::SquaredCurvature;

PointelContribution CPointelContribution::setContribution(const Parameters &prm,
                                                          const Constants &sqc)
{
    PointelContribution::UnaryMap um;
    PointelContribution::BinaryMap bm;
    PointelContribution::TernaryMap tm;

    BinaryHandle bh(bm,prm,sqc);
    TernaryHandle th(tm,prm,sqc);

    IteratorManager::run(prm,bh,th);


    return PointelContribution(um,bm,tm);
}

void CPointelContribution::BinaryHandle::operator()(const Point &pointel, const Point &pixel)
{
    PointMultiIndex bi;
    bi << pointel << pixel;

    if(this->bm.find(bi)==this->bm.end()) this->bm[bi]=0;
    this->bm[bi] += this->sqc.unaryContribution.at(pointel);
}

void CPointelContribution::TernaryHandle::operator()(const Point &pointel, const Point &pixel1, const Point &pixel2)
{
    PointMultiIndex ti;
    ti << pointel << pixel1 << pixel2;

    if(this->tm.find(ti)==this->tm.end()) this->tm[ti]=0;
    this->tm[ti]+=2;
}


double CPointelContribution::binaryContribution(const KSpace& kspace,
                                                const PointelContribution &pctbr,
                                                const PointMultiIndex &multiIndex)
{
    return 0;
}

double CPointelContribution::ternaryContribution(const KSpace& kspace,
                                                 const PointelContribution &pctbr,
                                                 const PointMultiIndex &multiIndex)
{
    return 0;
}