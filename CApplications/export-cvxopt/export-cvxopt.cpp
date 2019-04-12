#include <iostream>

#include "boost/filesystem.hpp"

#include <DGtal/io/writers/GenericWriter.h>

#include <LPModel/linearization/model/Linearization.h>

#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/utils/dispUtils.h>
#include <LPModel/terms/API.h>

#include "LPModel/initialization/API.h"
#include "LPModel/initialization/Shapes.h"

#include "LPModel/terms/sqc/CSqc.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/lpwriter/LPWriter.h"

#include "InputData.h"
#include "InputReader.h"

#include "PythonExporter.h"

using namespace LPModel;
using namespace LPModel::Terms;

using namespace PythonExporter;

typedef DIPaCUS::Representation::Image2D Image2D;
typedef DGtal::Z2i::DigitalSet DigitalSet;

DigitalSet loadImageAsDigitalSet(const std::string& imageFilePath)
{
    std::cerr << "Load Image as DigitalSet\n";

    Image2D imgInput = DGtal::GenericReader<Image2D>::import(imageFilePath);
    DigitalSet ds(imgInput.domain());
    DIPaCUS::Representation::imageAsDigitalSet(ds,imgInput);

    return ds;
}

void consistentPixel(const Constraints::ClosedAndConnected::PixelIncidence& pi,
                     unsigned long int cNum,
                     SparseMatrix& C,
                     std::vector<double>& c)
{
    typedef Constraints::ClosedAndConnected::Pixel Pixel;

    if(pi.pixel.ct==Pixel::CellType::RELAXATION_AUXILIARFrg)
    {
        if(pi.posIncidence) c[cNum] += -1;
        else c[cNum] += 1;
    }
    else if(pi.pixel.ct==Pixel::CellType::Variable)
    {
        if(pi.posIncidence) C.insert(cNum,pi.pixel.varIndex,1);
        else C.insert(cNum,pi.pixel.varIndex,-1);
    }
}

void consistentEdge(const Constraints::ClosedAndConnected::EdgeIncidence& ei,
                    unsigned long int numC,
                    SparseMatrix& C,
                    std::vector<double>& c)
{
    if(ei.posIncidence) C.insert(numC,ei.edge.varIndex,1);
    else C.insert(numC,ei.edge.varIndex,-1);
}


void exportPixelPairLinearization(const std::string& outputPath,
                                  const DigitalSet& ds,
                                  const Initialization::Grid& grid,
                                  const Terms::Term& mergedTerm)
{
    typedef Linearization< Terms::Term::UIntMultiIndex,double > MyLinearization;

    unsigned long nextIndex = grid.pixelMap.size()+grid.linelMap.size()+grid.edgeMap.size();
    MyLinearization linearization(nextIndex);

    MyLinearization::PartialLinearizationPair plp(Initialization::IVariable::Pixel,Initialization::IVariable::Pixel);
    Terms::Term::BinaryMap partialL = linearization.partialLinearization(mergedTerm.ternaryMap,grid,plp);

    unsigned long int numPixelPairs = 0;
    for(auto it=linearization.ubegin();it!=linearization.uend();++it) ++numPixelPairs;

    unsigned long int numSlack = numPixelPairs*3;
    unsigned long int numVars = grid.pixelMap.size() -3 + grid.edgeMap.size() + numPixelPairs + numSlack;

    unsigned long int slackStart = nextIndex+numPixelPairs;

    std::vector<double> U(numVars);
    {
        for(auto it=U.begin();it!=U.end();++it) *it=0;
        const Term::UnaryMap& um = mergedTerm.unaryMap;
        for(auto it=um.begin();it!=um.end();++it)
        {
            Term::UIntMultiIndex mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel = *mit;

            U[pixel] = mergedTerm.unaryMap.at(mIndex);
        }
    }

    SparseMatrix P1;
    {
        const Term::BinaryMap& bm = mergedTerm.binaryMap;
        for(auto it=bm.begin();it!=bm.end();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel = *mit;++mit;
            unsigned long int edge = *mit;

            if(it->second!=0) P1.insert(pixel,edge,it->second);
        }
    }


    SparseMatrix P2;
    {
        const Term::BinaryMap& bm = partialL;
        for(auto it=bm.begin();it!=bm.end();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int edge = *mit;++mit;
            unsigned long int pixelPair = *mit;

            if(it->second!=0) P2.insert(pixelPair,edge,it->second);
        }
    }

    //RHS constants missing
    SparseMatrix Z;
    std::vector<double> z;
    {
        unsigned long int slackBaseIndex = slackStart;
        unsigned long int baseIndex = 0;
        z.resize(numSlack);
        for(auto it=linearization.ubegin();it!=linearization.uend();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel1 = *mit; ++mit;
            unsigned long int pixel2 = *mit;
            unsigned long int auxVarIndex = it->second;

            Z.insert(baseIndex*3,pixel1,-1);
            Z.insert(baseIndex*3,auxVarIndex,1);
            Z.insert(baseIndex*3,slackBaseIndex+baseIndex*3,1);
            z[slackBaseIndex+baseIndex*3]=0;

            Z.insert(baseIndex*3+1,pixel2,-1);
            Z.insert(baseIndex*3+1,auxVarIndex,1);
            Z.insert(baseIndex*3+1,slackBaseIndex+baseIndex*3+1,1);
            z[slackBaseIndex+baseIndex*3+1]=0;

            Z.insert(baseIndex*3+2,pixel1,-1);
            Z.insert(baseIndex*3+2,pixel2,-1);
            Z.insert(baseIndex*3+2,auxVarIndex,1);
            Z.insert(baseIndex*3+2,slackBaseIndex+baseIndex*3+2,-1);
            z[slackBaseIndex+baseIndex*3+2]=-1;

            if(baseIndex*3+2>=numSlack) throw std::runtime_error("");
            if(slackBaseIndex+baseIndex*3+2>=numVars) throw std::runtime_error("");
            if(auxVarIndex>=numVars) throw std::runtime_error("");
            if(pixel1>=numVars) throw std::runtime_error("");
            if(pixel2>=numVars) throw std::runtime_error("");
        }
    }

    SparseMatrix C;
    std::vector<double> c;
    {
        c.resize(grid.edgeMap.size()/2);

        Constraints::ClosedAndConnected::LinelConstraints lc;
        Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);

        typedef Constraints::ClosedAndConnected::Linel Linel;
        typedef Constraints::ClosedAndConnected::LinelIncidence LinelIncidence;
        typedef Constraints::ClosedAndConnected::PixelIncidence PixelIncidence;
        typedef Constraints::ClosedAndConnected::EdgeIncidence EdgeIncidence;

        int nc=0;
        for(auto it=lc.begin();it!=lc.end();++it,++nc)
        {
            const Linel& linel = it->first;
            const LinelIncidence li = it->second;

            consistentPixel(li.pixel1,nc,C,c);
            consistentPixel(li.pixel2,nc,C,c);
            consistentEdge(li.edge1,nc,C,c);
            consistentEdge(li.edge2,nc,C,c);

        }
    }

    std::ofstream ofs(outputPath);

    ofs << "numPixels = " << grid.pixelMap.size() - 3 << "\n";
    ofs << "numEdges = " << grid.edgeMap.size() << "\n";
    ofs << "numPixelPairs = " << numPixelPairs << "\n";
    ofs << "numSlackVars = " << numSlack << "\n";
    ofs << "numVars = " << numVars  << "\n";

    exportPython(ofs,U,P1,P2,Z,z,C,c);

    exportDigitalSetAsMatrix(ofs,ds);
    exportPixelMap(ofs,grid.pixelMap);

    ofs.close();
}


void exportPixelLinelPairLinearization(const std::string& outputPath,
                                       const DigitalSet& ds,
                                       const Initialization::Grid& grid,
                                       const Terms::Term& mergedTerm)
{
    unsigned long nextIndex = grid.pixelMap.size()+grid.linelMap.size()+grid.edgeMap.size();
    LPWriter::MyLinearization linearization(nextIndex);
    linearization.linearize(mergedTerm.binaryMap);

    unsigned long int numPixelLinelPairs = 0;
    for(auto it=linearization.ubegin();it!=linearization.uend();++it) ++numPixelLinelPairs;

    unsigned long int numSlack = numPixelLinelPairs*3;
    unsigned long int numVars = grid.pixelMap.size() -3 + grid.edgeMap.size() + numPixelLinelPairs + numSlack;

    unsigned long int slackStart = nextIndex+numPixelLinelPairs;

    std::vector<double> U(numVars);
    {
        for(auto it=U.begin();it!=U.end();++it) *it=0;
        const Term::UnaryMap& um = mergedTerm.unaryMap;
        for(auto it=um.begin();it!=um.end();++it)
        {
            Term::UIntMultiIndex mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel = *mit;

            U[pixel] = mergedTerm.unaryMap.at(mIndex);
        }
    }

    std::vector<double> V(numVars);
    {
        for(auto it=linearization.begin();it!=linearization.end();++it)
        {
            V[it->first] = it->second;
        }
    }


    SparseMatrix P;
    {
        const Term::TernaryMap& tm = mergedTerm.ternaryMap;
        for(auto it=tm.begin();it!=tm.end();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();

            unsigned long int pixel1 = *mit;++mit;
            unsigned long int pixel2 = *mit;++mit;
            unsigned long int edge = *mit;

            Term::UIntMultiIndex lpIndex;
            lpIndex << pixel1 << edge;

            unsigned long int linelPairIndex = linearization.uniqueIndexMap[lpIndex];

            if(it->second!=0) P.insert(pixel2,linelPairIndex,it->second);

        }
    }


    //RHS constants missing
    SparseMatrix Z;
    std::vector<double> z;
    {
        unsigned long int slackBaseIndex = slackStart;
        unsigned long int baseIndex = 0;
        z.resize(numSlack);
        for(auto it=linearization.ubegin();it!=linearization.uend();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel1 = *mit; ++mit;
            unsigned long int edge = *mit;
            unsigned long int auxVarIndex = it->second;

            Z.insert(baseIndex*3,pixel1,-1);
            Z.insert(baseIndex*3,auxVarIndex,1);
            Z.insert(baseIndex*3,slackBaseIndex+baseIndex*3,1);
            z[slackBaseIndex+baseIndex*3]=0;

            Z.insert(baseIndex*3+1,edge,-1);
            Z.insert(baseIndex*3+1,auxVarIndex,1);
            Z.insert(baseIndex*3+1,slackBaseIndex+baseIndex*3+1,1);
            z[slackBaseIndex+baseIndex*3+1]=0;

            Z.insert(baseIndex*3+2,pixel1,-1);
            Z.insert(baseIndex*3+2,edge,-1);
            Z.insert(baseIndex*3+2,auxVarIndex,1);
            Z.insert(baseIndex*3+2,slackBaseIndex+baseIndex*3+2,-1);
            z[slackBaseIndex+baseIndex*3+2]=-1;

            if(baseIndex*3+2>=numSlack) throw std::runtime_error("");
            if(slackBaseIndex+baseIndex*3+2>=numVars) throw std::runtime_error("");
            if(auxVarIndex>=numVars) throw std::runtime_error("");
            if(pixel1>=numVars) throw std::runtime_error("");
            if(edge>=numVars) throw std::runtime_error("");
        }
    }

    SparseMatrix C;
    std::vector<double> c;
    {
        c.resize(grid.edgeMap.size()/2);

        Constraints::ClosedAndConnected::LinelConstraints lc;
        Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);

        typedef Constraints::ClosedAndConnected::Linel Linel;
        typedef Constraints::ClosedAndConnected::LinelIncidence LinelIncidence;
        typedef Constraints::ClosedAndConnected::PixelIncidence PixelIncidence;
        typedef Constraints::ClosedAndConnected::EdgeIncidence EdgeIncidence;

        int nc=0;
        for(auto it=lc.begin();it!=lc.end();++it,++nc)
        {
            const Linel& linel = it->first;
            const LinelIncidence li = it->second;

            consistentPixel(li.pixel1,nc,C,c);
            consistentPixel(li.pixel2,nc,C,c);
            consistentEdge(li.edge1,nc,C,c);
            consistentEdge(li.edge2,nc,C,c);

        }
    }

    std::ofstream ofs(outputPath);

    ofs << "numPixels = " << grid.pixelMap.size() - 3 << "\n";
    ofs << "numEdges = " << grid.edgeMap.size() << "\n";
    ofs << "numPixelLinelPairs = " << numPixelLinelPairs << "\n";
    ofs << "numSlackVars = " << numSlack << "\n";
    ofs << "numVars = " << numVars  << "\n";

    exportPython(ofs,U,V,P,Z,z,C,c);

    exportDigitalSetAsMatrix(ofs,ds);
    exportPixelMap(ofs,grid.pixelMap);

    ofs.close();
}


int main(int argc, char* argv[])
{
    InputData in;
    in = readInput(argc,argv);

    std::cerr << "Preparing python model for convex optimization for image: " << in.pgmInputImage << "\n"
              << "with sq-weight=" << in.sqWeight << "; data-weight=" << in.dataWeight << "\n"
              << "with linearization level=" << resolveLinearizationLevelName( in.linearizationLevel ) << "\n"
              << "and optimization region width of " << in.optRegionWidth << " pixels...\n";



    DigitalSet ds = loadImageAsDigitalSet(in.pgmInputImage);


    Initialization::Parameters prm = Initialization::API::initParameters(ds,in.optRegionWidth);
    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    Terms::Term scTerm = SquaredCurvature::API::prepare(prm,grid,in.sqWeight);
    //Terms::Term dataTerm = DataFidelity::API::prepare(prm,grid,in.dataWeight);

    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);

    switch(in.linearizationLevel)
    {
        case LPModel::LINEARIZATION_PIXEL_PAIR:
        {
            exportPixelPairLinearization(in.outputPath,ds,grid,mergedTerm);
            break;
        }
        case LPModel::LINEARIZATION_PIXEL_LINEL:
        {
            exportPixelLinelPairLinearization(in.outputPath,ds,grid,mergedTerm);
            break;
        }
        default:
        {
            std::cerr << "Invalid relaxation. Such relaxation leads to a linear or third order formulation.\n";
            exit(1);
        }

    }


    return 0;
}