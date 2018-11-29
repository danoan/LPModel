#include "LPModel/terms/sqc/control/CTerm.h"

using namespace LPModel::Terms;
using namespace LPModel::Terms::SquaredCurvature;

Term CTerm::setTerm(const Parameters &prm,
                    const Grid& grid,
                    const Constants &sqc,
                    double weight)
{
    PointelContribution pctbr = SquaredCurvature::Internal::Pointel::setContribution(prm,
                                                                                     sqc);

    LinelContribution lctbr = SquaredCurvature::Internal::Linel::setContribution(prm,
                                                                                 grid,
                                                                                 pctbr);


    Term::UnaryMap um;
    Term::BinaryMap bm;
    Term::TernaryMap tm;

    Internal::setUnaryMap(um,sqc,prm,grid,weight);
    Internal::setBinaryMap(bm,lctbr,grid,weight);
    Internal::setTernaryMap(tm,lctbr,grid,weight);

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

void CTerm::Internal::addBinaryElement(Term::BinaryMap& bm,
                                       const unsigned long n1,
                                       const unsigned long n2,
                                       const double value)
{
    Term::UIntMultiIndex  uiBI;
    uiBI <<  n1 << n2;

    if(bm.find(uiBI)==bm.end()) bm[uiBI]=0;
    bm[uiBI] += value;
}

void CTerm::Internal::addTernaryElement(Term::TernaryMap& tm,
                                        const unsigned long n1,
                                        const unsigned long n2,
                                        const unsigned long n3,
                                        const double value)
{
    Term::UIntMultiIndex  uiBI;
    uiBI <<  n1 << n2 << n3;

    if(tm.find(uiBI)==tm.end()) tm[uiBI]=0;
    tm[uiBI] += value;
}

void CTerm::Internal::setUnaryMap(Term::UnaryMap& um,
                                  const Constants& sqc,
                                  const Parameters& prm,
                                  const Grid& grid,
                                  double weight)
{
    unsigned long firstLinelVar = grid.pixelMap.size();
    unsigned long firstEdgeVar = firstLinelVar + grid.linelMap.size();

    DGtal::Z2i::KSpace kspace;
    kspace.init(prm.odrModel.optRegion.domain().lowerBound(),
                prm.odrModel.optRegion.domain().upperBound(),
                true);

    int edgeBaseIndex;
    double unaryValue=0;
    for(auto it=grid.linelMap.begin();it!=grid.linelMap.end();++it)
    {
        edgeBaseIndex = Initialization::CLinel::edgeBaseIndex(firstLinelVar,
                                                              firstEdgeVar,
                                                              it->second.linelIndex);

        unaryValue=0;
        DGtal::Z2i::SCells pointels = kspace.sLowerIncident( kspace.sCell(it->first,true) );
        for(auto it2=pointels.begin();it2!=pointels.end();++it2)
        {
            unaryValue+=sqc.constantContribution.at(kspace.sKCoords(*it2));
        }

        Term::UIntMultiIndex unaryIndex1,unaryIndex2;
        unaryIndex1 << edgeBaseIndex;
        unaryIndex2 << edgeBaseIndex+1;

        um[unaryIndex1]=weight*(unaryValue/2.0); //Divide by two because of two pointels contributions;
        um[unaryIndex2]=weight*(unaryValue/2.0); //Divide by two because of two pointels contributions;

    }
}

void CTerm::Internal::setBinaryMap(Term::BinaryMap& bm,
                                   const LinelContribution& lctbr,
                                   const Grid& grid,
                                   double weight)
{
    unsigned long firstLinelVar = grid.pixelMap.size();
    unsigned long firstEdgeVar = firstLinelVar + grid.linelMap.size();

    int edgeBaseIndex;

    DGtal::Z2i::Point linel,pixel;
    for(auto it=lctbr.binaryMap.begin();it!=lctbr.binaryMap.end();++it)
    {
        separate(linel,pixel,it->first);

        edgeBaseIndex = Initialization::CLinel::edgeBaseIndex(firstLinelVar,
                                                              firstEdgeVar,
                                                              grid.linelMap.at(linel).linelIndex);

        addBinaryElement(bm,edgeBaseIndex,grid.pixelMap.at(pixel).varIndex,weight*it->second);
        addBinaryElement(bm,edgeBaseIndex+1,grid.pixelMap.at(pixel).varIndex,weight*it->second);
    }

}


void CTerm::Internal::setTernaryMap(Term::TernaryMap& tm,
                                    const LinelContribution& lctbr,
                                    const Grid& grid,
                                    double weight)
{
    unsigned long firstLinelVar = grid.pixelMap.size();
    unsigned long firstEdgeVar = firstLinelVar + grid.linelMap.size();

    int edgeBaseIndex;

    DGtal::Z2i::Point linel,pixel1,pixel2;
    for(auto it=lctbr.ternaryMap.begin();it!=lctbr.ternaryMap.end();++it)
    {
        separate(linel,pixel1,pixel2,it->first);

        edgeBaseIndex = Initialization::CLinel::edgeBaseIndex(firstLinelVar,
                                                              firstEdgeVar,
                                                              grid.linelMap.at(linel).linelIndex);

        addTernaryElement(tm,edgeBaseIndex,grid.pixelMap.at(pixel1).varIndex,grid.pixelMap.at(pixel2).varIndex,weight*it->second);
        addTernaryElement(tm,edgeBaseIndex+1,grid.pixelMap.at(pixel1).varIndex,grid.pixelMap.at(pixel2).varIndex,weight*it->second);
    }

}
