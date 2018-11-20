#include "LPModel/terms/sqc/control/CTerm.h"

using namespace LPModel::Terms::SquaredCurvature;

Term CTerm::setTerm(const Parameters &prm,
                    const Grid& grid,
                    const Constants &sqc)
{
    PointelContribution pctbr = SquaredCurvature::Internal::Pointel::setContribution(prm,
                                                                                     sqc);

    LinelContribution lctbr = SquaredCurvature::Internal::Linel::setContribution(prm,
                                                                                 grid,
                                                                                 pctbr);

    Term::UnaryMap um;
    Term::BinaryMap bm;
    Term::TernaryMap tm;

    Internal::setBinaryMap(bm,lctbr,grid);
    Internal::setTernaryMap(tm,lctbr,grid);

    return Term(um,bm,tm);
}

bool CTerm::Internal::isPixel(const DGtal::Z2i::Point& p)
{
    return p(0)%2==1 && p(1)%2==1;
}

void CTerm::Internal::separate(DGtal::Z2i::Point& linel,
                               DGtal::Z2i::Point& pixel,
                               const LinelContribution::PointMultiIndex& pmi)
{
    for(auto it=pmi.begin();it!=pmi.end();++it)
    {
        if( isPixel(*it) ) pixel = *it;
        else linel=*it;
    }
}

void CTerm::Internal::separate(DGtal::Z2i::Point& linel,
                               DGtal::Z2i::Point& pixel1,
                               DGtal::Z2i::Point& pixel2,
                               const LinelContribution::PointMultiIndex& pmi)
{
    std::vector<Point> pixels;
    for(auto it=pmi.begin();it!=pmi.end();++it)
    {
        if( isPixel(*it) ) pixels.push_back(*it);
        else linel=*it;
    }

    pixel1 = pixels[0];
    pixel2 = pixels[1];
}

void CTerm::Internal::setBinaryMap(Term::BinaryMap& bm,
                                   const LinelContribution& lctbr,
                                   const Grid& grid)
{
    DGtal::Z2i::Point linel,pixel;
    for(auto it=lctbr.binaryMap.begin();it!=lctbr.binaryMap.end();++it)
    {
        separate(linel,pixel,it->first);

        Term::UIntMultiIndex  uiBI;
        uiBI << grid.linelMap.at(linel).linelIndex
             << grid.pixelMap.at(pixel).varIndex;

        if(bm.find(uiBI)==bm.end()) bm[uiBI]=0;
        bm[uiBI] += it->second;
    }

}


void CTerm::Internal::setTernaryMap(Term::TernaryMap& tm,
                                    const LinelContribution& lctbr,
                                    const Grid& grid)
{
    DGtal::Z2i::Point linel,pixel1,pixel2;
    for(auto it=lctbr.ternaryMap.begin();it!=lctbr.ternaryMap.end();++it)
    {
        separate(linel,pixel1,pixel2,it->first);

        Term::UIntMultiIndex  uiBI;
        uiBI << grid.linelMap.at(linel).linelIndex
             << grid.pixelMap.at(pixel1).varIndex
             << grid.pixelMap.at(pixel2).varIndex;

        if(tm.find(uiBI)==tm.end()) tm[uiBI]=0;
        tm[uiBI] += it->second;
    }

}
