#include "LPModel/terms/sqc/control/CTerm.h"

using namespace LPModel::Terms::SquaredCurvature;

Term CTerm::setTerm(const Parameters &prm,
                    const Grid& grid,
                    const Constants &sqc)
{
    PointelContribution pctbr = CPointelContribution::setContribution(prm,
                                                                      sqc);

    Term::BinaryMap bm;
    Term::TernaryMap tm;

    BinaryHandle bh(bm,prm,grid,pctbr);
    TernaryHandle th(tm,prm,grid,pctbr);

    IteratorManager::run(prm,bh,th);


}

void CTerm::BinaryHandle::operator()(const Point &linel, const Point &p1)
{
    CPointelContribution::PointMultiIndex pBI;
    pBI << linel << p1;

    Term::UIntMultiIndex  uiBI;
    uiBI << grid.linelMap.at(linel).linelIndex
         << grid.pixelMap.at(p1).varIndex;

    if(this->bm.find(uiBI)==this->bm.end()) this->bm[uiBI]=0;
    this->bm[uiBI] += CPointelContribution::binaryContribution(this->kspace,this->pctbr,pBI);
}

void CTerm::TernaryHandle::operator()(const Point &linel, const Point &p1, const Point &p2)
{
    CPointelContribution::PointMultiIndex pTI;
    pTI<< linel << p1 << p2;

    Term::UIntMultiIndex  uiTI;
    uiTI << grid.linelMap.at(linel).linelIndex
         << grid.pixelMap.at(p1).varIndex
         << grid.pixelMap.at(p2).varIndex;

    if(this->tm.find(uiTI)==this->tm.end()) this->tm[uiTI]=0;
    this->tm[uiTI]+=CPointelContribution::ternaryContribution(this->kspace,this->pctbr,pTI);

}